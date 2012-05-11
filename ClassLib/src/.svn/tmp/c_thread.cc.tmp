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
 * $Id: c_thread.cc,v 1.8 1998/08/28 14:19:41 nmcl Exp $
 */

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#include <setjmp.h>
#include <stdlib.h>
#include <iostream.h>

#ifndef C_THREAD_H_
#  include <c_thread.h>
#endif

#include <ClassLib/thread.h>

static mutex_t globalMutex;

long    ThreadData::base_key = 0;
Thread* ThreadData::mainThread = (Thread*) 0;
long    ThreadData::mainThreadID = 0;

ThreadData::ThreadData ()
		       : dead(FALSE)
{
}

ThreadData::~ThreadData ()
{
}

void* ThreadData::Execute (void* p1)
{
    Thread* _p1 = (Thread*) p1;

    cthread_set_data(_p1->_data->cid, (any_t) &_p1->thread_key);
    mutex_lock(_p1->_data->mx);

    if (!_p1->_data->dead)
	_p1->Body();
    else
	mutex_unlock(globalMutex);
    
    cthread_exit(NULL);
}


Thread::Thread (Boolean create)
	       : thread_key(-1),
		 _data(new ThreadData),
		 next(0),
		 prev(0)
{
#ifdef DEBUG
    debug_stream << FAC_THREAD << CONSTRUCTORS << VIS_PROTECTED << endl;
    debug_stream << "Thread::Thread" << endl;
#endif

    thread_key = ThreadData::base_key++;

    if (mutex_alloc(&_data->mx, N_CURRENT) != T_SUCCEED)
    {
	cerr << FATAL
		     << "C_Thread error - cannot allocate thread mutex!"
		     << endl;
	abort();
    }

    mutex_lock(_data->mx);

    if (create)
    {
	any_t p1 = (any_t) this;

	_data->cid = cthread_fork(ThreadData::Execute, p1, N_CURRENT);
	(void) cthread_detach(_data->cid);
    }
    
    Insert((Thread*) 0, this);
}

Thread::Thread (unsigned long)
	       : thread_key(-1),
		 _data(new ThreadData),
		 next(0),
		 prev(0)    
{
#ifdef DEBUG
    debug_stream << FAC_THREAD << CONSTRUCTORS << VIS_PROTECTED << endl;
    debug_stream << "Thread::Thread" << endl;
#endif
    
    thread_key = ThreadData::base_key++;

    if (mutex_alloc(&_data->mx, N_CURRENT) != T_SUCCEED)
    {
	cerr << FATAL
		     << "C_Thread error - cannot allocate thread mutex!"
		     << endl;
	abort();
    }

    mutex_lock(_data->mx);

    any_t p1 = (any_t) this;

    _data->cid = cthread_fork(ThreadData::Execute, p1, N_CURRENT);
    (void) cthread_detach(_data->cid);
    
    Insert((Thread*) 0, this);
}

Thread::~Thread ()
{
#ifdef DEBUG
    debug_stream << FAC_THREAD << DESTRUCTORS << VIS_PROTECTED << endl;
    debug_stream << "Thread::~Thread" << endl;
#endif

    Remove(this);
    
    if (!_data->dead)
    {
	mutex_clear(_data->mx);
	cthread_yield();  // let threads wake up
	mutex_free(_data->mx);
	cthread_set_data(_data->cid, NULL);

	terminateThread();
    }

    delete _data;
}

long Thread::Current_Thread () const
{
    any_t p1 = cthread_data(cthread_self());

    return *(long*) p1;
}

void Thread::terminateThread ()
{
    /*
     * If one thread terminates another then we must switch contexts
     * to the terminating thread for it to tidy itself up. Then that
     * thread must switch back to us.
     */

    _data->dead = TRUE;

    if (_data->cid != cthread_self())
    {
	Thread::Resume();

	mutex_lock(globalMutex);

	void** p = 0;
	
	_data->cid->join(p);
    }
    else
	cthread_exit(NULL);
}

void Thread::Suspend ()
{
    if (cthread_self() == _data->cid)
	mutex_lock(_data->mx);

    if (_data->dead)
    {
	mutex_unlock(globalMutex);
	cthread_exit(NULL);
	cthread_yield();
    }
}

void Thread::Resume ()
{
    mutex_unlock(_data->mx);
}

ostream& Thread::print (ostream& strm) const
{
    strm << "Thread type is C Thread.\n";
    strm << "\nThread key: " << thread_key << "\n";

    return strm;
}

//
// Getting the main thread into the thread list...
//

class Main_CThread : public Thread
{
public:
    Main_CThread ();
    ~Main_CThread ();

    static void dummyMain ();

    void Body ();

    static jmp_buf env;
};

jmp_buf Main_CThread::env;

Main_CThread::Main_CThread ()
			   : Thread(FALSE)
{
    (void) mutex_alloc(&globalMutex, N_CURRENT);
    mutex_lock(globalMutex);

    _data->cid = cthread_self();
    cthread_set_data(_data->cid, (any_t) &thread_key);

    ThreadData::mainThreadID = thread_key;
    ThreadData::mainThread = this;
}

Main_CThread::~Main_CThread () {}

void Main_CThread::Body () {}

/*
 * We have to do this because the cthreads approach to making "main"
 * a thread is to force it (the first thread) into a separate procedure
 * for it to run. When it returns from this procedure the application ends.
 * This is different to all of the other threads packages, which simply
 * treat the main thread as automatically running within "main". Rather
 * than make all of the other packages conform to cthreads, we can force
 * cthreads to work in the manner of the other scheme by making the main
 * thread jump back to the calling point of cthread_init using setjmp/longjmp.
 * This is messy, but it allows us to still write applications which do not
 * have to be specific to a given threads package.
 * Don't try this at home, folks!
 */

void Main_CThread::dummyMain ()
{
    longjmp(Main_CThread::env, 1);
}

/*
 * Here is the thread specific initialization code.
 */

void Thread::Initialize ()
{
    if (!_initialized)
    {
	_initialized = TRUE;	
	if (setjmp(Main_CThread::env) == 0)
	    (void) cthread_init(1, Main_CThread::dummyMain);

	new Main_CThread();
    }
}

void Thread::Exit (int ret)
{
    exit(ret);
}

void Thread::mainResume ()
{
    if (!ThreadData::mainThread)
	return;

    Thread* currentThread = (Thread*) Thread::Self();

    mutex_unlock(ThreadData::mainThread->_data->mx);
    cthread_yield();
    mutex_lock(currentThread->_data->mx);	
}


/*
 * The mutex.
 */

C_Mutex::C_Mutex ()
{
    mutex_alloc(&_theLock, N_CURRENT);
    mutex_init(_theLock);
}

C_Mutex::~C_Mutex ()
{
    mutex_free(_theLock);
}

Boolean C_Mutex::lock ()
{
    if (mutex_lock(_theLock) == 0)
	return TRUE;
    else
	return FALSE;
}

Boolean C_Mutex::unlock ()
{
    if (mutex_unlock(_theLock) == 0)
	return TRUE;
    else
	return FALSE;
}

/*
 * Now the Mutex create method.
 */

Mutex* Mutex::create ()
{
    return new C_Mutex;
}
