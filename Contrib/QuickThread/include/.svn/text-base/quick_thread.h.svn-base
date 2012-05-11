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
 * Copyright 1996 Commonwealth Scientific and Industrial Research
 * Organisation, Australia
 * Available under the same conditions as the C++SIM public distribution.
 *
 * Parts of this file
 * Copyright (C) 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef QUICK_THREAD_H_
#define QUICK_THREAD_H_

extern "C"
{
#include <qt.h>
}

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef THREAD_H_
#  include "ClassLib/thread.h"
#endif

class ostream;

/*
 * This is an implementation of threads using David Keppel's QuickThreads.
 *
 * Reference: Keppel, David, "Tools and Techniques for Building Fast Portable
 *            Threads Packages," UWCSE 93-05-06, University of Washington
 *            Department of Computer Science and Engineering.
 *
 * QuickThreads source and documentation is available through
 *   http://www.cs.washington.edu/homes/pardo/papers.d/thread.html
 * and also by ftp from:
 *   ftp://ftp.cs.washington.edu/tr/1993/05/UW-CSE-93-05-06.PS.Z>
 * for a paper describing QuickThreads, and
 *   ftp://ftp.cs.washington.edu/pub/qt-002.tar.gz
 * for the source.
 *
 * As with the Sun threads interface, some problems may be worked around
 * by increasing the stack size.
 *
 * The queue/priority system implemented here is the minimum needed for
 * C++SIM. If you want more priorities, you can increase QT_MAXPRIO, if
 * you run out of queue slots, you can increase QT_QLEN (but warning,
 * queue slots are statically allocated, and C++SIM doesn't need more
 * than what is available here).
 *
 */

#define MINPRIO 0
#define QT_MAXPRIO 1
#define QT_QLEN 5

class Quick_Thread : public Thread
{
public:
    virtual void Suspend ();                // Suspend an active thread
    virtual void Resume ();                 // Resume a suspended thread

    virtual void Body () = 0;               // Body of "active" object (defined in the deriving class)
    virtual long Current_Thread () const;   // Returns current thread id


    static long Current_Quick_Thread ();   // Returns current thread id

    // Initialize must be called exactly once at the start of the program
    static void Initialize ();

    virtual ostream& print (ostream&) const;

protected:
    static const int MaxPriority;            // Maximum priority of a thread

		// Create thread with given (or maximum) priority
    Quick_Thread ();

		// Create thread with given priority and minimum stack size
    Quick_Thread (unsigned long stackSize);
    
		// This constructor should *only* be called from
		// Quick_Main_Thread::Quick_Main_Thread! See the
		// .cc file for details.
    Quick_Thread (int priority, qt_t*);

    virtual ~Quick_Thread ();

    virtual void terminateThread ();
    
private:
    // Execute() and Only() are helper functions used to establish the threads
    // See "Tools and Techniques..." above.
    static void Execute (void*);    // This routine calls the 'main' object code
    static void Only (void* u, void* t, qt_userf_t *userf);

    // AbortHlp() and YeildHlp() are helper functions for terminating
    // threads and for switching to a new thread respectively.
    static void* AbortHlp (qt_t *sp, void* t, void*);
    static void* YeildHlp (qt_t *sp, void* t, void*);

    Quick_Thread* nextThread();		// Get the next thread to run
    void addThread();			// Add a new thread to the queues
    void removeThread();		// Take a thread out of the queues
    int topPrio();			// Priority of highest priority task

    // Common thread creation code
    void ThreadCreate(unsigned long stackSize);

    static int NextId;			    // Next Id to use
    static Quick_Thread* currentThread;

    // The thread priority queues
    // -- implemented using simple fixed-maximum-size
    //    array-based circular queues.
    static struct qtq {
    	int		len;
	Quick_Thread**	head;
	Quick_Thread**	tail;
	Quick_Thread*	q[QT_QLEN];
    } threadQueues[QT_MAXPRIO+2];

    unsigned char*	stack_alloc;	// Block allocated for the stack
    unsigned char*	stack;		// Actual stack starting point
    					// after adjustment for alignment
    qt_t*		sp;		// Current stack pointer. This
    					// is QuickThread's handle for the
					// thread.

    int			prio;		// Thread Priority
};

/*
 * This implementation of threads is non-preemtive, so
 * we don't actually need a mutex here. So, this implementation
 * does nothing.
 */

class QuickMutex : public Mutex
{
public:
    QuickMutex ();
    ~QuickMutex ();

    Boolean lock ();
    Boolean unlock ();
};

#include <Contrib/QuickThread/quick_thread.n>

#endif
