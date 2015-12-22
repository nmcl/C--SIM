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
 * $Id: Process.cc,v 1.39 1998/10/05 09:03:11 nmcl Exp $
 */

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#ifndef PROCESS_H_
#  include <ClassLib/Process.h>
#endif

#ifdef ProcessList_Queue
#  ifndef PROCESSLIST_H_
#    include "ProcessList.h"
#  endif
#endif

#ifdef ProcessHeap_Queue
#  ifndef PROCESSHEAP_H_
#    include "ProcessHeap.h"
#  endif
#endif

#ifdef Calendar_Queue
#  ifndef CALENDAR_H_
#    include <Calendar/include/Calendar.h>
#  endif
#endif

#ifdef HashedList_Queue
#  ifndef HASHEDLIST_H_
#    include "HashedList.h"
#  endif
#endif

#ifndef __GNUG__
static double SimulatedTime = 0.0;
#else
double SimulatedTime = 0.0;
#endif

Queue_Type ReadyQueue;  // Queue_Type is replaced by cpp

static Mutex* _theMutex = Mutex::create();

Scheduler* Scheduler::theScheduler = (Scheduler*) 0;
Boolean Scheduler::schedulerRunning = FALSE;
Process* Process::Current = (Process*) 0;
const double Process::Never = -1; // Process will never awaken.

/*
 * Note: unlike in SIMULA, an active process is removed from the simulation
 * queue prior to being activated.
 */

//
// Class Scheduler
//

// scheduler is just an object, with no associated thread.

Scheduler::Scheduler ()
{
}

Scheduler::~Scheduler ()
{
}

/*
 * This routine resets the simulation time to zero and removes all
 * entries from the scheduler queue (as their times may no longer
 * be valid). Such entries are suspended as though Cancel had been
 * called on them by a "user" process.
 *
 * Note: if this is to be used to reset the simulation environment
 * for another "run" then some means of re-initializing these queue
 * entries will be required. This is up to the user to provide and
 * invoke prior to their being used again.
 */

void Scheduler::reset () const
{
    Process* tmp = (Process*) 0;

    do
    {
	tmp = ReadyQueue.Remove();
	
	if (tmp)
	{
	    tmp->Cancel();
	    tmp->reset();  // call user-definable reset routine.
	}
	
    } while (tmp);

    SimulatedTime = 0.0;
}

double Scheduler::CurrentTime () const { return SimulatedTime; }

void Scheduler::print (ostream& strm)
{
    strm << "Scheduler queue:\n" << endl;
    
    ReadyQueue.print(strm);

    strm << "End of scheduler queue." << endl;
}

//
// Class Process
//

/*
 * Before this process is deleted, make sure it
 * is removed from the queue, or we could have
 * problems with pointer dereferencing!
 */

Process::~Process ()
{
    /*
     * We don't call Cancel or terminate here since they will
     * attempt to suspend this process if it is running, and
     * we do not want that to occur - garbage could quickly
     * build up. So, we let the destructor run to completion
     * which will implicitly suspend the process anyway, and
     * let the thread specific destructor then decide whether
     * it needs to do anything specific (e.g., reactivate the
     * scheduler) before it finishes.
     */

    if (!Terminated)
    {
	Terminated = TRUE;
	Passivated = TRUE;

	unschedule(); // remove from scheduler queue

	wakeuptime = Process::Never;

	if (this == Process::Current)
	{
	    schedule();
	    _theMutex->unlock();
	}
    }
}

double Process::CurrentTime () { return SimulatedTime; }

void Process::set_evtime (double time)
{
    if (!idle())  // error if we are not scheduled for activation
    {
	if (time >= Process::CurrentTime())
	    wakeuptime = time;
	else
	    cerr
			 << "Process::set_evtime - time " << time
			 << " invalid" << endl;
    }
    else
        cerr
		     << "Process::set_evtime called for idle process." << endl;
}

// return process to be activated after this process.

const Process* Process::next_ev () const
{
    return ((!idle()) ? ReadyQueue.getNext(this) : (Process*) 0);
}

/*
 * These routines are slightly different to the SIMULA
 * equivalents in that a process must be given, and that
 * process must be scheduled. Complete compatibility
 * may be provided in future releases.
 */

void Process::ActivateBefore (Process &p)
{
    // No op if already scheduled
    if (Terminated || !idle()) return;

    Passivated = FALSE;
    if (ReadyQueue.InsertBefore(*this, p))
	wakeuptime = p.wakeuptime;
    else
	cerr << "ActivateBefore failed because 'before' process is not scheduled" << endl;
}

void Process::ActivateAfter (Process &p)
{
    // No op if already scheduled
    if (Terminated || !idle()) return;

    Passivated = FALSE;
    if (ReadyQueue.InsertAfter(*this, p))
	wakeuptime = p.wakeuptime;
    else
	cerr << "ActivateAfter failed because 'after' process is not scheduled" << endl;
}

/*
 * These routines differ from their SIMULA counterparts in
 * that a negative AtTime is considered an error, and will
 * not cause the process to be activated at the current time.
 * This may change in a future release, when we may provide
 * a "SIMULA compatibility" mode.
 */

void Process::ActivateAt (double AtTime, Boolean prior)
{
    // No op if already scheduled
    if ((AtTime < Process::CurrentTime()) || Terminated || !idle()) return;

    Passivated = FALSE;
    wakeuptime = AtTime;

    ReadyQueue.Insert(*this, prior);
}

void Process::ActivateDelay (double Delay, Boolean prior)
{
    // No op if already scheduled
    if (!checkTime(Delay) || Terminated || !idle()) return;

    Passivated = FALSE;
    wakeuptime = SimulatedTime + Delay;
    ReadyQueue.Insert(*this, prior);
}

void Process::Activate ()
{
    // No op if already scheduled
    if (Terminated || !idle()) return;

    Passivated = FALSE;
    wakeuptime = CurrentTime();
    ReadyQueue.Insert(*this, TRUE);
}

/*
 * Similarly, there are four ways to reactivate
 * Note that if a process is already scheduled, the reactivate
 * will simply re-schedule the process.
 */

void Process::ReActivateBefore (Process &p)
{
    if (Terminated) return;
    
    unschedule();
    ActivateBefore(p);
    if (Process::Current == this)
        Suspend();
}

void Process::ReActivateAfter (Process &p)
{
    if (Terminated) return;
    
    unschedule();
    ActivateAfter(p);
    if (Process::Current == this)
        Suspend();
}

void Process::ReActivateAt (double AtTime, Boolean prior)
{
    if (Terminated) return;

    unschedule();
    ActivateAt(AtTime, prior);
    if (Process::Current == this)
        Suspend();
}

void Process::ReActivateDelay (double Delay, Boolean prior)
{
    if (Terminated) return;
    
    unschedule();
    ActivateDelay(Delay, prior);
    if (Process::Current == this)
        Suspend();
}

void Process::ReActivate ()
{
    if (Terminated) return;
    
    unschedule();
    Activate();
    if (Process::Current == this)
        Suspend();
}

Boolean Process::schedule ()
{
    if (Scheduler::simulationStarted())
    {
	Boolean doSuspend = TRUE;
    
	Process::Current = ReadyQueue.Remove();

	if (Process::Current == (Process*) 0)    // all done
	{
	    cout << "Scheduler queue is empty. Simulation ending" << endl;
	    Thread::Exit();
	}

	if (Process::Current->evtime() < 0)
	{
	    cerr << "Scheduler Error: Process WakeupTime "
			 << Process::Current->evtime() << " invalid.\n";
	}
	else
	    SimulatedTime = Process::Current->evtime();

#ifdef DEBUG
	cout << "Simulated time is now " << SimulatedTime << endl;
#endif

	if (Process::Current != this)
	    Process::Current->Resume();
	else
	    doSuspend = FALSE;

	return doSuspend;
    }
    else
	return FALSE;
}

// only called if process is running or on queue to be run

void Process::unschedule ()
{
    if (!idle())
    {
	if (this != Process::Current)
	    (void) ReadyQueue.Remove(this); // remove from queue
	wakeuptime = Process::Never;
	Passivated = TRUE;
    }
}

/*
 * Cancels next burst of activity, process becomes idle.
 * If the process is currently active then it is suspended,
 * and a reschedule is performed.
 */

void Process::Cancel ()
{
    /*
     * We must suspend this process either by removing it from
     * the scheduler queue (if it is already suspended) or by
     * calling suspend directly.
     */

    if (!idle())  // process is running or on queue to be run
    {
	unschedule();  // remove from queue
	
        if (this == Process::Current) // currently active, so simply suspend
	    Suspend();
    }
}

void Process::reset ()
{
#ifdef DEBUG    
    cerr << "Reset of process called." << endl;
#endif    
}

Process::Process ()
                 : wakeuptime(Process::Never),
		   Terminated(FALSE),
		   Passivated(TRUE)
{
}

Process::Process (unsigned long stackSize)
		 : Thread(stackSize),
		   wakeuptime(Process::Never),
		   Terminated(FALSE),
		   Passivated(TRUE)
{
}

// suspend current process for simulated time t

void Process::Hold (double t)
{
    if (checkTime(t))
    {
	if ((this == Process::Current) || (!Process::Current))
	{
	    wakeuptime = Process::Never;
	    ActivateDelay(t);
	    Suspend();
	}
	else
	    cerr
			 << "Process::Hold - can only be applied to active object."
			 << endl;
    }
    else
	cerr << "Process::Hold - time " << t
		     << " invalid." << endl;
}

// this works on active and scheduled processes

void Process::terminate ()
{
    if (!Terminated)
    {
	if (this != Process::Current)
	    unschedule();

	/*
	 * Don't set these until after we are finished
	 * with them!
	 */
	 
        Terminated = Passivated = TRUE;
	wakeuptime = Process::Never;
	
	if (this == Process::Current)
	{
	    schedule();
	    _theMutex->unlock();
	}

	terminateThread();
    }
}

/*
 * We add these routines because we may need to do some
 * Process specific manipulations prior to calling the
 * thread specific implementations.
 */

void Process::Suspend ()
{
    if (schedule())
    {
	_theMutex->unlock();
	Thread::Suspend();
	_theMutex->lock();
    }
}

/*
 * Make sure that Process::Current is always valid.
 */

void Process::Resume ()
{
    if (Process::Current == (Process*) 0)
    {
        Process::Current = this;
        wakeuptime = CurrentTime();
    }

    if (!Terminated)
	Thread::Resume();
}

#ifdef NO_INLINES
#  define PROCESS_CC_
#  include <ClassLib/Process.n>
#  undef PROCESS_CC_
#endif
