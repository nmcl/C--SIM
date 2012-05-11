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
 * Copyright (C) 1994-1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TriggerQueue.cc,v 1.9 1998/09/30 08:07:16 nmcl Exp $
 */

#ifndef ENTITY_H_
#  include <Event/Entity.h>
#endif

#ifndef ENTITYLIST_H_
#  include "EntityList.h"
#endif

#ifndef TRIGGERQUEUE_H_
#  include <Event/TriggerQueue.h>
#endif

TriggerQueue::TriggerQueue ()
                            : head(new EntityList())
{
}

/*
 * If there is anything left on the queue when this goes
 * out-of-scope then we will place them back on to the
 * simulation queue, rather than doing nothing - this is the
 * safest thing to do. THIS SHOULD NOT HAPPEN THOUGH!*
 */

TriggerQueue::~TriggerQueue ()
{
    if (head)
    {
	if (head->numberOfElements() > 0)
	{
	    cerr << "TriggerQueue destructor called with non-zero list!" << endl;
	    (void) triggerAll();
	}

	delete head;
    }
}

Boolean TriggerQueue::triggerFirst (Boolean setTrigger)
{
    if (head->numberOfElements() == 0)
    {
#ifdef DEBUG
	cerr
		     << "triggerFirst called with empty list." << endl;
#endif
	return FALSE;
    }

    Entity* removed = this->remove();
    
    if (removed == (Entity*) 0)
    {
#ifdef DEBUG
	cerr
		     << "triggerFirst called and got a null entry from list!"
		     << endl;
#endif
	return FALSE;
    }
    
    if (setTrigger)
        removed->trigger();

    removed->ReActivateAt(Process::CurrentTime());

    return TRUE;
}

Boolean TriggerQueue::triggerAll ()
{
    long currentNumber = head->numberOfElements();

    if (currentNumber == 0)
    {
#ifdef DEBUG
	cerr << "TriggerAll called with empty list." << endl;
#endif
	return FALSE;
    }

    for (int i = 0; i < currentNumber; i++)
        (void) triggerFirst();

    return TRUE;
}

void TriggerQueue::insert (Entity* toAdd)
{
    if (toAdd->isWaiting())  // currently cannot be waiting for multiple events
    {
#ifdef DEBUG
	cerr
		     << "TriggerQueue::insert called for entry which is already waiting."
		     << endl;
#endif
	return;
    }

    head->add(toAdd);
}

Entity* TriggerQueue::remove ()
{
    return head->removeFirst();
}
