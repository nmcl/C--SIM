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
 */

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#include <iostream.h>
#include <unistd.h>

#ifndef SOLARIS_THREAD_H_
#  include <solaris_thread.h>
#endif

/*
 * These are the Sun C routines which give access to threads.
 */

extern "C"
{
    size_t thr_min_stack();
    int    thr_setprio(thread_t, int);
    int    thr_getprio(thread_t, int*);
    void   thr_exit(void*);
    int    thr_suspend(thread_t);
    int    thr_continue(thread_t);
}

static const int MaxPriority = 10;

ThreadData::ThreadData ()
		       : mid(0),
			 dead(FALSE)
{
    sema_init(&sp, 0, USYNC_THREAD, NULL);
    sema_init(&waitSem, 0, USYNC_THREAD, NULL);    
}

ThreadData::~ThreadData ()
{
    sema_destroy(&sp);
    sema_destroy(&waitSem);
}

void* ThreadData::Execute (void* p1)
{
    Thread* _p1 = (Thread*) p1;

    if (!_p1)
	return 0;

    sema_wait(&_p1->_data->sp);
    
    if (!_p1->_data->dead)
    {
	_p1->Body();
    }
    else
    {
	sema_post(&_p1->_data->waitSem);
    }
    
    return 0;
}

Thread::Thread (Boolean createThread)
	       : thread_key(-1),
		 _data(new ThreadData),
		 next(0),
		 prev(0)
{
    if (createThread)
    {
	caddr_t p1 = (caddr_t) this;

	(void) thr_create(NULL, 0, ThreadData::Execute, p1,
			  THR_DETACHED, &_data->mid);
    
	(void) thr_setprio(_data->mid, MaxPriority);
	thread_key = _data->mid;
    }
    else
    {
	thread_key = thr_self();
	_data->mid = thread_key;
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
    size_t minStackSize = thr_min_stack()*5;
    size_t sizeToUse = ((stackSize < minStackSize) ? minStackSize : (unsigned int) stackSize);

    if (stackSize < minStackSize)
	cerr << "Stack size " << stackSize
		     << " is too small. Using " << minStackSize << endl;

    (void) thr_create(NULL, sizeToUse, ThreadData::Execute,
		      p1, THR_DETACHED, &_data->mid);

    (void) thr_setprio(_data->mid, MaxPriority);
    thread_key = _data->mid;

    Insert((Thread*) 0, this);
}

Thread::~Thread ()
{
    Remove(this);

    terminateThread();

    delete _data;
}

void Thread::terminateThread ()
{
    if (!_data->dead)
    {
	_data->dead = TRUE;

	if (thr_self() != _data->mid)
	{
	    if (sema_post(&_data->sp) == 0)
	    {
		sema_wait(&_data->waitSem);
		thr_join(_data->mid, 0, 0);
	    }
	}
	else
	    thr_exit(0);
    }
}

void Thread::Suspend ()
{
    sema_wait(&_data->sp);

    if (_data->dead)
    {
	sema_post(&_data->waitSem);
	thr_exit(0);
    }
}

void Thread::Resume ()
{
    sema_post(&_data->sp);
}

long Thread::Current_Thread () const
{
    return thr_self();
}

ostream& Thread::print (ostream& strm) const
{
    strm << "Thread type is Solaris threads.\n";
    strm << "\nThread key: " << thread_key << "\n";

    return strm;
}

//
// Getting the main thread into the thread list...
//

class Solaris_Main_Thread : public Thread
{
public:
    Solaris_Main_Thread ();
    ~Solaris_Main_Thread ();

    void Body ();

    static Solaris_Main_Thread* mainThread;
};

Solaris_Main_Thread* Solaris_Main_Thread::mainThread = 0;


Solaris_Main_Thread::Solaris_Main_Thread ()
					 : Thread(FALSE)
{
}

Solaris_Main_Thread::~Solaris_Main_Thread () {}

void Solaris_Main_Thread::Body () {}

void Thread::Initialize ()
{
    if (!_initialized)
    {
	_initialized = TRUE;
	
	int prio = MaxPriority+1;

	(void) thr_setprio(thr_self(), prio);
    
	Solaris_Main_Thread::mainThread = new Solaris_Main_Thread();
    }
}

void Thread::Exit (int retValue)
{
    _exit(retValue);
}

void Thread::mainResume ()
{
#ifdef DEBUG
    debug_stream << FAC_THREAD << FUNCTIONS << VIS_PUBLIC << endl;
    debug_stream << "Thread::mainResume" << endl;
#endif
    
    Solaris_Main_Thread::mainThread->Resume();
}

/*
 * The mutex.
 */

SolarisMutex::SolarisMutex ()
{
    mutex_init(&_theLock, 0, 0);
}

SolarisMutex::~SolarisMutex ()
{
    mutex_destroy(&_theLock);
}

Boolean SolarisMutex::lock ()
{
    if (mutex_lock(&_theLock) == 0)
	return TRUE;
    else
	return FALSE;
}

Boolean SolarisMutex::unlock ()
{
    if (mutex_unlock(&_theLock) == 0)
	return TRUE;
    else
	return FALSE;
}

/*
 * Now the Mutex create method.
 */

Mutex* Mutex::create ()
{
    return new SolarisMutex;
}
