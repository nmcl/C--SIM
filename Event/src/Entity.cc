/*
 * Copyright 1990-2008, Mark Little, University of Newcastle upon Tyne
 * and others contributors as indicated 
 * by the @authors tag. All rights reserved. 
 * See the copyright.txt in the distribution for a
 * full listing of individual contributors. 
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU Lesser General Public License, v. 2.1.
 * This program is distributed in the hope that it will be useful, but WITHOUT A 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License,
 * v.2.1 along with this distribution; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, 
 * MA  02110-1301, USA.
 * 
 * (C) 1990-2008,
 */
 /*
 * Copyright (C) 1994-1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Entity.cc,v 1.11 1997/08/05 17:10:33 nmcl Exp $
 */

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#ifndef PROCESSLIST_H_
#  include "ProcessList.h"
#endif

#ifndef TRIGGERQUEUE_H_
#  include <Event/TriggerQueue.h>
#endif

#ifndef SEMAPHORE_H_
#  include <Event/Semaphore.h>
#endif

#ifndef ENTITY_H_
#  include <Event/Entity.h>
#endif

Entity::~Entity ()
{
#ifdef DEBUG
    cout << "Entity::~Entity ()" << endl;
#endif
}

/*
 * Suspends the current process until the process in the parameter
 * has been terminated. The return value indicates whether or not
 * the process has been terminated, as it is possible for this
 * process to be interrupted (woken up) early.
 * If the Boolean parameter is set then the controller is reactivated
 * immediately.
 */

Boolean Entity::WaitFor (Entity& controller, Boolean reAct)
{
#ifdef DEBUG
    cout << "Boolean Entity::WaitFor (Entity& controller, Boolean reAct)" << endl;
#endif

    if (&controller == this)      // can't wait on self!
    {
        cerr << "Entity::WaitFor - cannot wait on self!" << endl;
	return FALSE;
    }

    controller._isWaiting = this;  // resume when controller terminates

    // make sure this is ready to run

    if (reAct)
	controller.ReActivateAt(Process::CurrentTime(), TRUE);
    
    _waiting = TRUE;

    // we don't go back on to queue as controller will wake us

    Cancel();

    _waiting = _interrupted = FALSE;

    // if we have been successful then terminated = TRUE

    return controller.terminated();
}

Boolean Entity::WaitForTrigger (TriggerQueue& _queue)
{
#ifdef DEBUG
    cout << "Boolean Entity::WaitForTrigger (TriggerQueue& _queue)" << endl;
#endif
    
    _queue.insert(this);
    _interrupted = FALSE;
    _waiting = TRUE;
    
    Cancel();            // remove from queue and suspend

    _waiting = FALSE;
    
    // indicate whether this was triggered successfuly or interrupted

    if (_triggered)
    {
        _triggered = FALSE;
        return TRUE;
    }

    return FALSE;
}

/*
 * Wait for specified period of time. If this process is interrupted
 * then FALSE is returned, otherwise TRUE.
 */

Boolean Entity::Wait (double waitTime)
{
#ifdef DEBUG
    cout << "Boolean Entity::Wait ( " << waitTime << " )" << endl;
#endif

    _waiting = TRUE;
    
    Hold(waitTime);

    _waiting = FALSE;

    if (_interrupted)
    {
	_interrupted = FALSE;
	return FALSE;
    }

    return TRUE;
}

/*
 * Interrupt the given process (which *must* be in Wait or WaitFor), and
 * resume it. If immediate resumption is required then this process will
 * be suspended (placed back on to the scheduler queue for "immediate"
 * resumption when the interrupted process has finished).
 */

Boolean Entity::Interrupt (Entity& toInterrupt, Boolean immediate)
{
#ifdef DEBUG
    cout << "Boolean Entity::Interrupt (Entity& toInterrupt, Boolean immediate)" << endl;
#endif
    
    if (toInterrupt.terminated())
    {
#ifdef DEBUG
	cerr << "Interrupt called on terminated object." << endl;
#endif
	return FALSE;
    }
    
    if (!toInterrupt._waiting)
    {
        cerr << "Interrupt called on object which is not in wait state." << endl;
        return FALSE;
    }
    
    toInterrupt._interrupted = TRUE;

    // remove from queue for "immediate" activation

    toInterrupt.unschedule();  // remove from queue and prepare to suspend

    // will take over when this process is suspended

    toInterrupt.ReActivateAt(Process::CurrentTime(), TRUE);

    // put "this" on to queue and suspend so that interrupted process can run

    if (immediate)
        ReActivateAt(Process::CurrentTime());

    return TRUE;
}

Entity::Entity ()
	       : Process(),
		 _isWaiting(0),
		 _interrupted(FALSE),
		 _triggered(FALSE),
		 _waiting(FALSE)
{
#ifdef DEBUG
    cout << "Entity::Entity ()" << endl;
#endif
}

Entity::Entity (unsigned long stackSize)
	       : Process(stackSize),
		 _isWaiting(0),
		 _interrupted(FALSE),
		 _triggered(FALSE),
		 _waiting(FALSE)
{
#ifdef DEBUG
    cout << "Entity::Entity ( " << stackSize << " )" << endl;
#endif
}

/*
 * Must wake up any waiting process before we "die".
 * Currently only a single process can wait on this condition, but
 * this may change to a list later.
 */

void Entity::terminate ()
{
#ifdef DEBUG
    cout << "void Entity::terminate ()" << endl;
#endif
    
    if (_isWaiting)  // resume waiting process before this one "dies".
    {
	// remove from queue for "immediate" activation

	_isWaiting->Cancel();
	_isWaiting->ReActivateAt(Process::CurrentTime(), TRUE);
	_isWaiting = (Entity*) 0;
    }

    Process::terminate();
}

#ifdef NO_INLINES
#  define ENTITY_CC_
#  include <Event/Entity.n>
#  undef ENTITY_CC_
#endif
