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
 * Copyright (C) 1991-1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: rex_thread.cc,v 1.7 1998/08/28 14:19:44 nmcl Exp $
 */

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#include <sys/types.h>
#include <stdlib.h>
#include <iostream.h>
#include <lwp.h>

#ifndef REX_THREAD_H_
#  include <rex_thread.h>
#endif

#ifndef THREAD_H_
#  include <ClassLib/thread.h>
#endif

extern struct pcb* currp;

static const int MaxPriority = MAXTPRI-1;
static const int defaultStackSize = 2048;

long ThreadData::base_key = 0;
long ThreadData::mainThreadID = -1;
sem* ThreadData::mainThread = (sem*) 0;


ThreadData::ThreadData ()
		       : to_wait(0),
			 my_block(0)
{
}

ThreadData::~ThreadData ()
{
}

void ThreadData::Execute (int prio, char**, Thread* p1)
{
    waits(p1->_data->to_wait);
    
    p1->Body();
    p1->terminateThread();
}


Thread::Thread (Boolean createThread)
	       : thread_key(-1),
		 _data(new ThreadData),
		 next(0),
		 prev(0)
{
    _data->to_wait = creats(0);
    
    thread_key = ThreadData::base_key++;

    if (createThread)
    {
	caddr_t p1 = (caddr_t) this;
	_data->my_block = creatp(MaxPriority, ThreadData::Execute, defaultStackSize, 0, 0, p1);
    }
    else
    {
	ThreadData::mainThreadID = thread_key;	
        setenvp(currp, 0);
    }

    Insert((Thread*) 0, this);    
}

Thread::Thread (unsigned long stackSize)
	       : thread_key(-1),
		 _data(new ThreadData),
		 next(0),
		 prev(0)    
{
    _data->to_wait = creats(0);
    
    caddr_t p1 = (caddr_t) this;
    thread_key = ThreadData::base_key++;
    _data->my_block = creatp(MaxPriority, ThreadData::Execute, stackSize, 0, 0, p1);

    Insert((Thread*) 0, this);    
}

Thread::~Thread ()
{
    Remove(this);
    
    if (_data->to_wait)
    {
        signals(_data->to_wait);
	delete _data->to_wait;
    }

    terminateThread();

    delete _data;
}

void Thread::terminateThread ()
{
    if (_data->my_block)
    {
	destroyp(_data->my_block);
	_data->my_block = 0;
	
	if (thread_key == Current_Thread())
	    yieldp();	// yield control
    }
}

void Thread::Suspend ()
{
    if (thread_key != 0)
    {
	// if this is not my process then set semaphore only
	if (currp != _data->my_block)
	{
	    _data->to_wait->count = -1;
	    return;
	}

	// otherwise do suspend
	waits(_data->to_wait);
    }
    else
    {
	ThreadData::mainThread = creats(0);
	waits(ThreadData::mainThread);
    }
}

void Thread::Resume ()
{
    signals(_data->to_wait);
}

long Thread::Current_Thread () const
{
    Thread* myEnv = (Thread*) getenvp(0);

    if (myEnv == (Thread*) 0)
	return ThreadData::mainThreadID;  // must be main thread
    else
	return myEnv->thread_key;
}

ostream& Thread::print (ostream& strm) const
{
    strm << "Thread type is Rex.\n";
    strm << "\nThread key: " << thread_key << "\n";

    return strm;
}

class Rex_Main_Thread : public Thread
{
public:
    Rex_Main_Thread (struct pcb*);
    ~Rex_Main_Thread ();
    
    void Body ();
};


Rex_Main_Thread::Rex_Main_Thread (struct pcb* me)
				 : Thread(FALSE)
{
    _data->my_block = me;
}

Rex_Main_Thread::~Rex_Main_Thread () {}

void Rex_Main_Thread::Body () {}

void Thread::Initialize ()
{
    if (!_initialized)
    {
	_initialized = TRUE;
	struct pcb* me = initlp(MaxPriority+1);
	setenvp(me, NULL);
	new Rex_Main_Thread(me);
    }
}

void Thread::Exit (int retValue)
{
    exit(retValue);
}

void Thread::mainResume ()
{
    signals(ThreadData::mainThread);
    Thread::Self()->Suspend();
}


/*
 * The mutex.
 */

RexMutex::RexMutex ()
		   : _theLock(creats(1))
{
}

RexMutex::~RexMutex ()
{
    delete _theLock;
}

Boolean RexMutex::lock ()
{
    waits(_theLock);
    return TRUE;
}

Boolean RexMutex::unlock ()
{
    signals(_theLock);
    return TRUE;
}

/*
 * Now the Mutex create method.
 */

Mutex* Mutex::create ()
{
    return new RexMutex;
}
