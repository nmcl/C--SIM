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
 * $Id: Process.h,v 1.24 1998/08/28 14:19:51 nmcl Exp $
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#include <iostream>
#include <ostream>

using std::cerr;
using std::cout;
using std::endl;
using std::ostream;
using std::istream;

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef THREAD_H_
#  include <ClassLib/thread.h>
#endif

/*
 * This is the main class for the simulation package. All objects which are to
 * possess independent threads of control but which are to be controlled by the
 * simulation scheduler MUST be derived from the Process class. The Scheduler,
 * which is also an object which possesses an independent thread and which
 * exists within the simulation environment, is shown here, but is only derived
 * from the Thread_Type class and not Process as it is the central controller
 * for the simulation environment and as such runs independently of everything
 * else. If debugging is turned on (-DDEBUG added to CFLAGS for the Makefile)
 * then the scheduler will print out the current simulation time and other
 * information.
 *
 * Thread_Type is a means of "transparently" changing the thread base class
 * implementation.
 */

#ifdef __GNUG__
extern double SimulatedTime;
#endif

class Scheduler
{
public:
    double CurrentTime () const;

    void reset () const;  // reset the simulation time

    static Scheduler& scheduler ();
    static void       terminate ();

    void Suspend ();
    void Resume ();

    static Boolean simulationStarted ();

    static void print (ostream&);

protected:
    Scheduler ();
    virtual ~Scheduler ();

private:
    static Scheduler* theScheduler;
    static Boolean schedulerRunning;
};

class Process : public Thread
{
    friend class Scheduler;

public:
    static const double Never;

    virtual ~Process ();

    /* The following two methods return the current simulation time */

    static double CurrentTime ();       // C++SIM version
    double Time () const;	        // SIMULA version

    double evtime () const;  // time at which process is scheduled to be active

    /*
     * The following method returns a reference to the next process to be run
     * by the scheduler *if* this object is active or scheduled to run.
     */

    const Process* next_ev () const;

    /*
     * There are five ways to activate a process:
     *   1) at the current simulation time
     *   2) before another process,
     *   3) after another process,
     *   4) at a specified (simulated) time, or
     *   5) after a specified (simulated) delay
     */

    void Activate ();
    void ActivateBefore (Process &);
    void ActivateAfter  (Process &);

#ifndef __GNUG__
    void ActivateAt     (double AtTime = CurrentTime(), Boolean prior = FALSE);
    void ActivateDelay  (double AtTime = CurrentTime(), Boolean prior = FALSE);
#else
    void ActivateAt     (double AtTime = SimulatedTime, Boolean prior = FALSE);
    void ActivateDelay  (double AtTime = SimulatedTime, Boolean prior = FALSE);
#endif

    /*
     * Similarly, there are five ways to reactivate
     * Note that if a process is already scheduled, the reactivate
     * will simply re-schedule the process.
     */

    void ReActivate ();
    void ReActivateBefore (Process &);
    void ReActivateAfter  (Process &);

#ifndef __GNUG__
    void ReActivateAt     (double AtTime = CurrentTime(), Boolean prior = FALSE);
    void ReActivateDelay  (double AtTime = CurrentTime(), Boolean prior = FALSE);
#else
    void ReActivateAt     (double AtTime = SimulatedTime, Boolean prior = FALSE);
    void ReActivateDelay  (double AtTime = SimulatedTime, Boolean prior = FALSE);
#endif

    void    Cancel ();	         // cancels next burst of activity, process becomes idle
    Boolean idle () const;	 // TRUE if process is not awake or not scheduled to wake up

    Boolean passivated () const; // returns whether or not the object has been passivated
    Boolean terminated () const; // returns whether or not the object has been terminated

    virtual void terminate ();   // terminate the process - no going back!

    static const Process* current ();  // returns current process

    /*
     * The pure virtual function, Body, defines the code that executes in
     * the process.
     */

    virtual void Body () = 0;

    /*
     * This method is called whenever a simulation is reset. Default does
     * nothing.
     */

    virtual void reset  ();

    static Process* Current;

protected:
    Process ();
    Process (unsigned long stackSize);

    void Hold (double t);     // suspend current process for simulated time t
    void Passivate ();	      // suspend current process (i.e., make idle)

    void set_evtime (double); // set wakeuptime (used by Scheduler)

    // remove from scheduler queue and prepare for passivation
    void unschedule ();

private:
    Boolean schedule ();
    Boolean checkTime (double) const;

    double  wakeuptime;
    Boolean Terminated;
    Boolean Passivated;

public:
    virtual void Suspend ();
    virtual void Resume ();
};

#include <ClassLib/Process.n>

#endif // PROCESS_H
