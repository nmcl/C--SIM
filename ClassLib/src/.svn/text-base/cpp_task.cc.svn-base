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
 * $Id: cpp_task.cc,v 1.21 1998/08/28 14:19:41 nmcl Exp $
 */

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#include <unistd.h>
#include <iostream.h>

#ifndef CPP_TASK_H_
#  include <cpp_task.h>
#endif

static _semaphore globalSem;

long  ThreadData::base_key = 0;
task* ThreadData::_mainTask = (task*) 0;
long  ThreadData::mainTaskID = 0;

// semaphore class definition

_semaphore::_semaphore () { sigs = 0; }

_semaphore::~_semaphore () {}

int _semaphore::pending () { return sigs <= 0; }

void _semaphore::signal ()
{
    if (sigs++ == 0) alert();
    if (sigs > 1) sigs = 1;
}

void _semaphore::wait ()
{
    for (;;)
    {
	if (--sigs >= 0) return;
	sigs++;
	thistask->sleep(this);
    }
}

ThreadData::ThreadData ()
		       : my_block(0)
{
}

ThreadData::~ThreadData ()
{
}

/*
 * The real work.
 * The C++ task library does not allow the thread to return from the Basic_Task
 * constructor. If it does, the system will crash. So, users must prevent
 * this from happening.
 */

Basic_Task::Basic_Task (Thread* toUse, long key, int stackSize)
		       : task((char*) key, DEFAULT_MODE, stackSize)
{
    sem.wait();

    toUse->Body();

    cancel(0);
}

Basic_Task::~Basic_Task () {}


Thread::Thread (Boolean createThread)
	       : thread_key(-1),
		 _data(new ThreadData),
		 next(0),
		 prev(0)
{
    if (createThread)
    {
	thread_key = ThreadData::base_key++;
	_data->my_block = new Basic_Task(this, thread_key);

#ifdef DELAY_TASK
	if (!_mainTask)
	    ThreadData::_mainTask = object::this_task();
#endif
    }
    else
    {
	thread_key = ThreadData::base_key++;
	ThreadData::mainTaskID = thread_key;

#ifndef DELAY_TASK    
	ThreadData::_mainTask = object::this_task();
#endif
    }

    Insert((Thread*) 0, this);
}

Thread::Thread (unsigned long stackSize)
	       : thread_key(-1),
		 _data(new ThreadData),
		 next(0),
		 prev(0)    
{
    thread_key = ThreadData::base_key++;
    _data->my_block = new Basic_Task(this, thread_key, (int) stackSize);

    Insert((Thread*) 0, this);    
}

Thread::~Thread ()
{
    Remove(this);
    
    terminateThread();

    delete _data;
}

long Thread::Current_Thread () const
{
    task* currTask = object::this_task();
    
    if (currTask == ThreadData::_mainTask)
	return ThreadData::mainTaskID;
    else
	return (long) currTask->t_name;
}

void Thread::terminateThread ()
{
    if (_data->my_block)
    {
	// must terminate this task before we can remove it

	Basic_Task* ptr = _data->my_block;
	_data->my_block = 0;

	ptr->cancel(0);
	delete ptr;
    }
}

void Thread::Suspend ()
{
    if (thread_key == 0)
    {
	globalSem.wait();
	return;
    }

    if (object::this_task() == _data->my_block)
	_data->my_block->sem.wait();
}

void Thread::Resume ()
{
    _data->my_block->sem.signal();
}

ostream& Thread::print (ostream& strm) const
{
    strm << "Thread type is C++ Task.\n";
    strm << "\nThread key: " << thread_key << "\n";

    return strm;
}

//
// Getting the main thread into the thread list...
//

class CPP_Main_Task : public Thread
{
public:
    CPP_Main_Task ();
    ~CPP_Main_Task ();

    void Body ();
};

CPP_Main_Task::CPP_Main_Task ()
			     : Thread(FALSE)
{
}

CPP_Main_Task::~CPP_Main_Task () {}

void CPP_Main_Task::Body () {}

void Thread::Initialize ()
{
    if (!_initialized)
    {
	_initialized = TRUE;
	new CPP_Main_Task;
    }
}

void Thread::Exit (int retValue)
{
    if (ThreadData::_mainTask)
	ThreadData::_mainTask->resultis(retValue);
    else
	object::this_task()->resultis(retValue);
}

void Thread::mainResume ()
{
    if (!ThreadData::_mainTask)
	return;

    Thread* currentTask = (Thread*) Thread::Self();

    globalSem.signal();
    currentTask->_data->my_block->sem.wait();
}


/*
 * The mutex.
 */

CPP_Mutex::CPP_Mutex ()
		     : _theLock(1)
{
}

CPP_Mutex::~CPP_Mutex ()
{
}

Boolean CPP_Mutex::lock ()
{
    _theLock->wait();
    return TRUE;
}

Boolean CPP_Mutex::unlock ()
{
    _theLock->signal();
    return TRUE;
}

/*
 * Now the Mutex create method.
 */

Mutex* Mutex::create ()
{
    return new CPP_Mutex;
}
