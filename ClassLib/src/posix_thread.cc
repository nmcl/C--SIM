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
 * $Id: posix_thread.cc,v 1.26 1998/10/05 12:03:52 nmcl Exp $
 */

#ifdef PTHREAD_DRAFT_HPUX
#  include <dce/cma_px.h>
#endif

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#ifndef POSIX_THREAD_H_
#  include <posix_thread.h>
#endif

extern "C"
{
    void _exit(int);
}

#ifndef PTHREAD_DRAFT_HPUX
#  define pthread_mutexattr_default NULL
#endif

//
// Class Posix_Thread
//

pthread_mutex_t globalMutex;
pthread_key_t _key;

static const int MaxPriority = 10;

long ThreadData::base_key = 0;

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

    if (_p1)
    {
#ifndef PTHREAD_DRAFT_SOLARIS
	(void) pthread_mutex_lock(&_p1->_data->_lock);
#else
	(void) sem_wait(&_p1->_data->_lock);
#endif	    
    
	if (!_p1->_data->dead)
	{
	    (void) pthread_setspecific(_key, (any_t) &_p1->thread_key);

	    _p1->Body();
	    _p1->terminateThread();
	}
	else
	{
#ifndef PTHREAD_DRAFT_SOLARIS
	    (void) pthread_mutex_unlock(&_p1->_data->_lock);
#else
	    sem_post(&_p1->_data->_lock);
#endif	    
	}
    }
    
    return 0;
}


Thread::Thread (Boolean create)
	       : thread_key(-1),
		 _data(new ThreadData),
		 next(0),
		 prev(0)
{
    if (create)
    {
	any_t p1 = (any_t) this;

#ifndef PTHREAD_DRAFT_HPUX	
	(void) pthread_attr_init(&_data->_attr);
#else
	(void) pthread_attr_create(&_data->_attr);
#endif

#if defined(PTHREAD_DRAFT_8) || defined(PTHREAD_DRAFT_SOLARIS) || defined(PTHREAD_DRAFT_LINUX)
	sched_param param;
#ifdef PTHREAD_DRAFT_8    
	param.prio = MaxPriority;
#else
	param.sched_priority = MaxPriority;
#endif

	(void) pthread_attr_setschedparam(&_data->_attr, &param);
	(void) pthread_attr_setdetachstate(&_data->_attr, PTHREAD_CREATE_DETACHED);
#else
#ifndef PTHREAD_DRAFT_HPUX	
	int state = 1;
    
	(void) pthread_attr_setprio(&_data->_attr, MaxPriority);
	(void) pthread_attr_setdetachstate(&_data->_attr, &state);
#endif
#endif

#ifndef PTHREAD_DRAFT_SOLARIS	
	(void) pthread_mutex_init(&_data->_lock, pthread_mutexattr_default);
	(void) pthread_mutex_lock(&_data->_lock);
#else
	(void) sem_init(&_data->_lock, 0, 0);
#endif	

#ifndef PTHREAD_DRAFT_HPUX
	(void) pthread_create(&_data->_thread, &_data->_attr, ThreadData::Execute, p1);
#else	
	(void) pthread_create(&_data->_thread, _data->_attr, ThreadData::Execute, p1);
	pthread_setprio(_data->_thread, MaxPriority);
#endif	
    }
    else
    {
	pthread_t me = pthread_self();
	
	(void) pthread_mutex_init(&globalMutex, pthread_mutexattr_default);
	(void) pthread_mutex_lock(&globalMutex);

#ifndef PTHREAD_DRAFT_HPUX	
	(void) pthread_attr_init(&_data->_attr);
#else
	(void) pthread_attr_create(&_data->_attr);
#endif	
	(void) pthread_setspecific(_key, (any_t) &thread_key);
    
#if defined(PTHREAD_DRAFT_8) || defined(PTHREAD_DRAFT_SOLARIS) || defined(PTHREAD_DRAFT_LINUX)
	sched_param param;
#ifdef PTHREAD_DRAFT_8    
	param.prio = MaxPriority+1;    
#else
	param.sched_priority = MaxPriority+1;
#endif    
	(void) pthread_setschedparam(me, 0, &param);
#else
#ifndef PTHREAD_DRAFT_HPUX	
	(void) pthread_getschedattr(me, &_data->_attr);
	(void) pthread_attr_setprio(&_data->_attr, MaxPriority+1);
	(void) pthread_setschedattr(me, _data->_attr);
#else
	pthread_setprio(me, MaxPriority+1);
#endif	
#endif
    }
    
    thread_key = ThreadData::base_key++;
    
    Insert((Thread*) 0, this);
}

Thread::Thread (unsigned long size)
	       : thread_key(-1),
		 _data(new ThreadData),
		 next(0),
		 prev(0)
{
    any_t p1 = (any_t) this;

#ifndef PTHREAD_DRAFT_HPUX    
    (void) pthread_attr_init(&_data->_attr);
#else
    (void) pthread_attr_create(&_data->_attr);
#endif    

#if defined(PTHREAD_DRAFT_8) || defined(PTHREAD_DRAFT_SOLARIS) || defined(PTHREAD_DRAFT_LINUX)
    sched_param param;
#ifdef PTHREAD_DRAFT_8    
    param.prio = MaxPriority;
#else
    param.sched_priority = MaxPriority;
#endif    

    (void) pthread_attr_setschedparam(&_data->_attr, &param);
    (void) pthread_attr_setdetachstate(&_data->_attr, PTHREAD_CREATE_DETACHED);    
#else
#ifndef PTHREAD_DRAFT_HPUX    
    int state = 1;
    
    (void) pthread_attr_setprio(&_data->_attr, MaxPriority);
    (void) pthread_attr_setdetachstate(&_data->_attr, &state);
#endif    
#endif

#ifndef PTHREAD_DRAFT_LINUX    
    (void) pthread_attr_setstacksize(&_data->_attr, (unsigned int) size);
#endif    

#ifndef PTHREAD_DRAFT_SOLARIS    
    (void) pthread_mutex_init(&_data->_lock, pthread_mutexattr_default);
    (void) pthread_mutex_lock(&_data->_lock);
#else
    (void) sem_init(&_data->_lock, 0, 0);
#endif    

#ifndef PTHREAD_DRAFT_HPUX    
    (void) pthread_create(&_data->_thread, &_data->_attr, ThreadData::Execute, p1);
#else    
    (void) pthread_create(&_data->_thread, _data->_attr, ThreadData::Execute, p1);
    pthread_setprio(_data->_thread, MaxPriority);
#endif
    
    thread_key = ThreadData::base_key++;

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
     * If one thread terminates another then we must switch contexts
     * to the terminating thread for it to tidy itself up. Then that
     * thread must switch back to us.
     */

    if (_data->dead)
	return;
    
    _data->dead = TRUE;

    if (pthread_equal(pthread_self(), _data->_thread) == 0)
    {
	Thread::Resume();

#ifdef PTHREAD_DRAFT_HPUX	
	pthread_yield();
#endif	
	
#ifndef PTHREAD_DRAFT_SOLARIS
	pthread_mutex_lock(&_data->_lock);
#else
	sem_wait(&_data->_lock);
#endif	

	/*
	 * Really join on this thread. We assume only one thread can be
	 * trying to delete the Thread object, so join should work!
	 */

	pthread_join(_data->_thread, 0);

#ifndef PTHREAD_DRAFT_SOLARIS
	pthread_mutex_unlock(&_data->_lock);
	pthread_mutex_destroy(&_data->_lock);
	
#ifndef PTHREAD_DRAFT_HPUX
	pthread_attr_destroy(&_data->_attr);
#else
	pthread_attr_delete(&_data->_attr);
#endif	
#else
	sem_destroy(&_data->_lock);
#endif	
    }
    else
    {
#ifndef PTHREAD_DRAFT_SOLARIS
	pthread_mutex_unlock(&_data->_lock);
	pthread_mutex_destroy(&_data->_lock);

#ifndef PTHREAD_DRAFT_HPUX	
	pthread_attr_destroy(&_data->_attr);
#else
	pthread_attr_delete(&_data->_attr);
#endif	
#else
	sem_destroy(&_data->_lock);
#endif	
	pthread_exit(0);
    }
}

long Thread::Current_Thread () const
{
#if defined(PTHREAD_DRAFT_8) || defined(PTHREAD_DRAFT_SOLARIS) || defined(PTHREAD_DRAFT_LINUX)
    return *(long*) pthread_getspecific(_key);
#else
    any_t p = (any_t) 0;
    
    (void) pthread_getspecific(_key, &p);
    return *(long*) p;
#endif
}

void Thread::Suspend ()
{
    if (thread_key == 0)
    {
	pthread_mutex_lock(&globalMutex);
	return;
    }
    
    if (pthread_equal(_data->_thread, pthread_self()) == 0)
	return;

#ifndef PTHREAD_DRAFT_SOLARIS
    pthread_mutex_lock(&_data->_lock);
#else
    sem_wait(&_data->_lock);
#endif    

    if (_data->dead)
    {
#ifndef PTHREAD_DRAFT_SOLARIS
	pthread_mutex_unlock(&_data->_lock);
#else
	sem_post(&_data->_lock);
#endif	
	pthread_exit(0);
    }
}

void Thread::Resume ()
{
#ifndef PTHREAD_DRAFT_SOLARIS
    pthread_mutex_unlock(&_data->_lock);
#else
    sem_post(&_data->_lock);
#endif    
}

ostream& Thread::print (ostream& strm) const
{
    strm << "Thread type is pthreads.\n";
    strm << "\nThread key: " << thread_key << "\n";

    return strm;
}

//
// Getting the main thread into the thread list...
//

class Posix_Main_Thread : public Thread
{
public:
    Posix_Main_Thread ();
    ~Posix_Main_Thread ();

    void Body ();

    static Posix_Main_Thread* mainThread;
};

Posix_Main_Thread* Posix_Main_Thread::mainThread = 0;


Posix_Main_Thread::Posix_Main_Thread ()
				     : Thread(FALSE)
{
}

Posix_Main_Thread::~Posix_Main_Thread () {}

void Posix_Main_Thread::Body () {}

void Thread::Initialize ()
{
    if (!_initialized)
    {
	_initialized = TRUE;
#if !defined(PTHREAD_DRAFT_SOLARIS) && !defined(PTHREAD_DRAFT_LINUX) && !defined(PTHREAD_DRAFT_HPUX)
	pthread_init();  // isn't this part of the standard?
#endif
#ifndef PTHREAD_DRAFT_HPUX	
	(void) pthread_key_create(&_key, 0);
#else
	(void) pthread_keycreate(&_key, 0);
#endif	

	Posix_Main_Thread::mainThread = new Posix_Main_Thread();
    }
}

void Thread::Exit (int retValue)
{
    _exit(retValue);
}

void Thread::mainResume ()
{
    pthread_mutex_unlock(&globalMutex);
}


/*
 * The mutex.
 */

PosixMutex::PosixMutex ()
{
    pthread_mutex_init(&_theLock, pthread_mutexattr_default);
}

PosixMutex::~PosixMutex ()
{
    pthread_mutex_unlock(&_theLock);
    pthread_mutex_destroy(&_theLock);
}

Boolean PosixMutex::lock ()
{
    if (pthread_mutex_lock(&_theLock) == 0)
	return TRUE;
    else
	return FALSE;
}

Boolean PosixMutex::unlock ()
{
    if (pthread_mutex_unlock(&_theLock) == 0)
	return TRUE;
    else
	return FALSE;
}

/*
 * Now the Mutex create method.
 */

Mutex* Mutex::create ()
{
    return new PosixMutex;
}
