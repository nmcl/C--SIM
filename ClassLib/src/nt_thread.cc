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
 * $Id: nt_thread.cc,v 1.14 1998/08/28 14:19:43 nmcl Exp $
 */

#ifndef NT_CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#include <iostream.h>

#ifndef NTTHREAD_H_
#  include <nt_thread.h>
#endif

static const int MaxPriority = THREAD_PRIORITY_NORMAL;

Thread* ThreadData::mainThread = 0;

ThreadData::ThreadData ()
		       : _prio(MaxPriority+1),
			 sem(CreateSemaphore(NULL, 0, 1, 0)),
			 thrHandle(0),
			 mid(0),
			 dead(FALSE)
{
}

ThreadData::~ThreadData ()
{
}

DWORD ThreadData::Execute (LPDWORD p1)
{
    Thread* _p1 = (Thread*) p1;
    
    WaitForSingleObject(_p1->_data->sem, INFINITE);

    if (!_p1->_data->dead)
    {
	_p1->Body();
	_p1->_data->dead = TRUE;
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
	LPVOID p1 = (LPVOID) this;
	_data->thrHandle = CreateThread(NULL, 0,
					(LPTHREAD_START_ROUTINE) ThreadData::Execute, p1,
					0, &_data->mid);
		
	SetThreadPriority(_data->thrHandle, _data->_prio);
	
	thread_key = (long) _data->mid;
    }
    else
    {
	_data->mid = GetCurrentThreadId();
	thread_key = _data->mid;
	ThreadData::mainThread = this;
    }

    Insert((Thread*) 0, this);
}

Thread::Thread (unsigned long stackSize)
	       : thread_key(-1),
		 _data(new ThreadData),
		 next(0),
		 prev(0)
{
    LPVOID p1 = (LPVOID) this;
    _data->thrHandle = CreateThread(NULL, stackSize,
				    (LPTHREAD_START_ROUTINE) ThreadData::Execute, p1,
				    0, &_data->mid);

    SetThreadPriority(_data->thrHandle, _data->_prio);
    thread_key = (long) _data->mid;

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

    _data->dead = TRUE;
    
    if (_data->mid == GetCurrentThreadId())
    {
	ReleaseSemaphore(_data->sem, 1, NULL);
	ExitThread(0);
    }
    else
    {
	Thread::Resume();
	WaitForSingleObject(_data->sem, INFINITE);
    }
}

long Thread::Current_Thread () const
{
    return GetCurrentThreadId();
}

void Thread::Suspend ()
{
    Thread* ptr = (Thread*) Thread::Self();

    if ((ptr == ThreadData::mainThread) && (ptr != this))
	return;

    if (ptr == this)
    {
	WaitForSingleObject(_data->sem, INFINITE);

	if (_data->dead)
	    terminateThread();
    }
}

void Thread::Resume ()
{
    Thread* ptr = (Thread*) Thread::Self();

    ReleaseSemaphore(_data->sem, 1, NULL);
}

ostream& Thread::print (ostream& strm) const
{
    strm << "Thread type is NT threads.\n";
    strm << "\nThread key: " << thread_key << "\n";

    return strm;
}

//
// Getting the main thread into the thread list...
//

class NT_Main_Thread : public Thread
{
public:
    NT_Main_Thread ();
    ~NT_Main_Thread ();

    void Body ();

    static NT_Main_Thread* mainThread;
};

NT_Main_Thread* NT_Main_Thread::mainThread = 0;


NT_Main_Thread::NT_Main_Thread ()
			       : Thread((Boolean) FALSE)
{
}

NT_Main_Thread::~NT_Main_Thread () {}

void NT_Main_Thread::Body () {}

void Thread::Initialize ()
{
    if (!_initialized)
    {
	_initialized = TRUE;
	SetThreadPriority(GetCurrentThread(), MaxPriority+1);
	NT_Main_Thread::mainThread = new NT_Main_Thread;
    }
}

void Thread::Exit (int retValue)
{
    exit(retValue);
}

void Thread::mainResume ()
{
    NT_Main_Thread::mainThread->Resume();
}


/*
 * The mutex.
 */

NT_Mutex::NT_Mutex ()
{
    _theLock = CreateMutex(NULL, 0, NULL);
}

NT_Mutex::~NT_Mutex ()
{
    CloseHandle(_theLock);
}

Boolean NT_Mutex::lock ()
{
    if (WaitForSingleObject(_theLock, INFINITE) != WAIT_FAILED)
	return TRUE;
    else
	return FALSE;
}

Boolean NT_Mutex::unlock ()
{
    if (ReleaseMutex(_theLock) == 0)
	return TRUE;
    else
	return FALSE;
}

/*
 * Now the Mutex create method.
 */

Mutex* Mutex::create ()
{
    return new NT_Mutex;
}
