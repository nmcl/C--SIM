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
 * Copyright (C) 1994-1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: lwp_thread.cc,v 1.20 1998/08/28 14:19:42 nmcl Exp $
 */

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#include <iostream.h>

#ifndef LWP_THREAD_H_
#  include <lwp_thread.h>
#endif

extern "C"
{
#include <lwp/lwpmachdep.h>

#ifdef BROKEN_LWP_H
    int pod_setmaxpri(int);
    int lwp_stkcswset(thread_t, caddr_t);
    int lwp_setstkcache(int, int);
    int lwp_create(thread_t*, void (*func)(Thread *),
		   int, int, stkalign_t*, int, caddr_t);
    int lwp_destroy(thread_t);
    int lwp_yield(thread_t);
    int lwp_suspend(thread_t);
    int lwp_resume(thread_t);
    int lwp_setpri(thread_t, int);
    int lwp_self(thread_t*);
    int lwp_ping(thread_t);
    int lwp_sleep(struct timeval*);
    void pod_exit(int);
#endif
}

ThreadData::ThreadData ()
		       : stack(0)
{
}

ThreadData::~ThreadData ()
{
}

void ThreadData::Execute (Thread* p1)
{
    p1->Body();
    p1->terminateThread();
}

//
// Class LWP_Thread
//

static const int MaxPriority = 10;

Thread::Thread (Boolean createThread)
	       : thread_key(-1),
		 _data(new ThreadData),
		 next(0),
		 prev(0)
{
    if (createThread)
    {
	caddr_t p1 = (caddr_t) this;
    
#ifndef STATIC_STACK
	(void) lwp_create(&_data->mid, ThreadData::Execute, MaxPriority,
			  LWPSUSPEND, lwp_newstk(), 1, p1);
#else
	_data->stack = ::new stkalign_t[MINSTACKSZ];
	(void) lwp_create(&_data->mid, ThreadData::Execute, MaxPriority,
			  LWPSUSPEND, STKTOP(&_data->stack[MINSTACKSZ]), 1, p1);
#endif
    
	thread_key = _data->mid.thread_key;
	(void) lwp_suspend(_data->mid);
    }
    else
    {
	lwp_self(&_data->mid);
	thread_key = _data->mid.thread_key;
    }

    Insert((Thread*) 0, this);
}

Thread::Thread (unsigned long stackSize)
	       : thread_key(-1),
		 _data(new ThreadData),
		 next(0),
		 prev(0)    
{
    caddr_t p1 = (caddr_t) this;
    
    _data->stack = ::new stkalign_t[stackSize];

    (void) lwp_create(&_data->mid, ThreadData::Execute, MaxPriority,
		      LWPSUSPEND, STKTOP(&_data->stack[stackSize]), 1, p1);
    thread_key = _data->mid.thread_key;
    (void) lwp_suspend(_data->mid);

    Insert((Thread*) 0, this);    
}

// tidy things up before we terminate thread

Thread::~Thread ()
{
    Remove(this);
    
    terminateThread();

    delete _data;
}

void Thread::terminateThread ()
{
    /*
     * Destruction of thread should release resources.
     */

    thread_t comp;

    (void) lwp_self(&comp);

    if (SAMETHREAD(comp, _data->mid))
	_data->stack = 0;
    else
    {
	if (_data->stack)
	{
	    ::delete [] _data->stack;
	    _data->stack = 0;
	}
    }
    
    (void) lwp_destroy(_data->mid);
}

long Thread::Current_Thread () const
{
    thread_t tid;

    (void) lwp_self(&tid);

    return tid.thread_key;
}

void Thread::Suspend ()
{
    (void) lwp_suspend(_data->mid);
}

void Thread::Resume ()
{
    (void) lwp_resume(_data->mid);
}

ostream& Thread::print (ostream& strm) const
{
    strm << "Thread type is Sun's lwp.\n";
    strm << "\nThread key: " << thread_key << "\n";

    return strm;
}

//
// Getting the main thread into the thread list...
//

class LWP_Main_Thread : public Thread
{
public:
    LWP_Main_Thread ();
    ~LWP_Main_Thread ();

    void Body ();

    static LWP_Main_Thread* mainThread;
};

LWP_Main_Thread* LWP_Main_Thread::mainThread = 0;


LWP_Main_Thread::LWP_Main_Thread ()
				 : Thread(FALSE)
{
}

LWP_Main_Thread::~LWP_Main_Thread ()
{
}

void LWP_Main_Thread::Body ()
{
}

void Thread::Initialize ()
{
    if (!_initialized)
    {
	_initialized = TRUE;
	
	(void) pod_setmaxpri(MaxPriority+1);
    
#ifndef STATIC_STACK
	(void) lwp_setstkcache(2048, 10);
#endif    

	thread_t me;
	lwp_self(&me);
	(void) lwp_setpri(me, MaxPriority+1);
	LWP_Main_Thread::mainThread = new LWP_Main_Thread;
    }
}

void Thread::Exit (int retValue)
{
    pod_exit(retValue);
}

void Thread::mainResume ()
{
    LWP_Main_Thread::mainThread->Resume();
}

/*
 * The mutex.
 */

LWP_Mutex::LWP_Mutex ()
{
    mon_create(&_theLock);
}

LWP_Mutex::~LWP_Mutex ()
{
    mon_destroy(_theLock);
}

Boolean LWP_Mutex::lock ()
{
    if (mon_enter(_theLock) == 0)
	return TRUE;
    else
	return FALSE;
}

Boolean LWP_Mutex::unlock ()
{
    if (mon_exit(_theLock) == 0)
	return TRUE;
    else
	return FALSE;
}

/*
 * Now the Mutex create method.
 */

Mutex* Mutex::create ()
{
    return new LWP_Mutex;
}
