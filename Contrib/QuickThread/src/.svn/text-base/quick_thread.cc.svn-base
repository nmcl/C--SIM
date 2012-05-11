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
 * The Commonwealth Scientific and Industrial Research Organisation makes
 * no representation about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
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
 * As with the Sun LWP threads interface, some problems may be worked around
 * by increasing the stack size.
 *
 * The queue/priority system implemented here is the minimum needed for
 * C++SIM. If you want more priorities, you can increase QT_MAXPRIO, if
 * you run out of queue slots, you can increase QT_QLEN (but warning,
 * queue slots are statically allocated, and C++SIM doesn't need more
 * than what is available here).
 *
 */

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#ifndef THREAD_H_
#  include <ClassLib/thread.h>
#endif

#include <iostream.h>

#ifdef USE_RZSTACKS
#ifndef RZSTACKS_H_
#  include <RZStacks.h>
#endif
#endif

#ifndef NO_INLINES
#  define INLINEF inline
#else
#  define INLINEF
#endif

#include <memory.h>

extern "C"
{
#include <qt.h>
}

#define QT_DEFAULTSTACKSIZE (4*1024)

#define QT_MINPRIO 0
#define QT_MAXPRIO 0
#define QT_QLEN 5

class ThreadData {
friend class Thread;
public:
    static long Current_Thread();
private:
		// Create thread with given minimum stack size
    ThreadData (Thread* t, unsigned long stackSize);
    
		// Create a main thread
    ThreadData ();

    ~ThreadData ();

    // Execute() and Only() are helper functions used to establish the threads
    // See "Tools and Techniques..." above.
    static void Execute (void*);    // This routine calls the 'main' object code
    static void Only (void* u, void* t, qt_userf_t *userf);

    // AbortHlp() and YeildHlp() are helper functions for terminating
    // threads and for switching to a new thread respectively.
    static void* AbortHlp (qt_t *sp, void* t, void*);
    static void* YeildHlp (qt_t *sp, void* t, void*);

    static ThreadData* nextThread();	// Get the next thread to run
    void addThread();			// Add this thread to the queues
    void removeThread();		// Take this thread out of the queues
    static int topPrio();		// Priority of highest priority task

    // Initialize the threads system
    static void Initialize();

    // Common thread creation/destruction code
    void ThreadCreate(Thread* t, unsigned long stackSize);
    void terminateThread ();

    static const int MaxPriority;            // Maximum priority of a thread
    static int NextId;			    // Next Id to use
    static ThreadData* currentThread;
    static Thread* mainThread;

    // The thread priority queues
    // -- implemented using simple fixed-maximum-size
    //    array-based circular queues.
    static struct qtq {
    	int		len;
	ThreadData**	head;
	ThreadData**	tail;
	ThreadData*	q[QT_QLEN];
    } threadQueues[QT_MAXPRIO+2];

    unsigned char*	stack_alloc;	// Block allocated for the stack
    unsigned char*	stack;		// Actual stack starting point
    					// after adjustment for alignment
    qt_t*		sp;		// Current stack pointer. This
    					// is QuickThread's handle for the
					// thread.

    int			prio;		// Thread Priority
    long		thread_key;	// Thread ID
};

const int ThreadData::MaxPriority = QT_MAXPRIO;
int ThreadData::NextId = 1;
ThreadData::qtq ThreadData::threadQueues[QT_MAXPRIO+2];
ThreadData* ThreadData::currentThread = 0;
Thread* ThreadData::mainThread = 0;

		// Create thread with given minimum stack size
ThreadData::ThreadData (Thread* t, unsigned long stackSize)
{
    ThreadCreate(t, stackSize);
}

// Create a thread from the given QuickThread thread handle/stack pointer
// Used to convert main()'s thread of control into a Thread.
// A sensible value will first be assigned to sp when control switches
// from the main thread to some other thread.

ThreadData::ThreadData ()
{
    sp = 0;
    stack_alloc = stack = 0;
    thread_key = NextId++;
    prio = QT_MAXPRIO+1;
}

ThreadData::~ThreadData ()
{
    terminateThread();
}

// Common code for thread creation.

void
ThreadData::ThreadCreate(Thread* t, unsigned long stackSize)
{
    // Create an aligned stack and an aligned stack pointer
#ifdef USE_RZSTACKS
    unsigned long size = ((stackSize + QT_STKALIGN-1) & ~(QT_STKALIGN-1));
    stack_alloc = (unsigned char*)RZStacks::new_stk(size);
    int offset = 0;
#else
    unsigned long size = ((stackSize + QT_STKALIGN-1) & ~(QT_STKALIGN-1))
    			+ QT_STKALIGN-1;
    stack_alloc = new unsigned char[size];
    int offset = (unsigned long)stack_alloc & (QT_STKALIGN-1);
#endif
    memset(stack_alloc, 0, size);
    stack = stack_alloc + offset;
    sp = QT_SP(stack, (size-offset-QT_STKALIGN) & ~(QT_STKALIGN-1));

    // Initialise the stack with the helper functions needed to
    // start things off, and their arguments.
    sp = QT_ARGS(sp, t, t, Execute, Only);

    // Set the thread's id and priority.
    thread_key = NextId++;
    prio = QT_MAXPRIO;
}

// Only() is called out of QuickThreads to start the thread executing.
// It's passed a function to call (userf) and an argument to userf.
// The second argument is available as an argument for Only()'s use.
// This is all set up in the call of QT_ARGS() above.

void ThreadData::Only (void* u, void*, qt_userf_t *userf)
{
    userf(u);
    /* NOTREACHED */
}

// YeildHlp does bookkeeping for the context switch. It runs
// on the new processes's stack; when it returns, the new process
// is executing. It saves the old process's sp and sets currentThread.

void* ThreadData::YeildHlp (qt_t *sp, void* from, void* to)
{
    ((ThreadData*)from)->sp = sp;
    currentThread = ((ThreadData*)to);
    return 0;
}

// AbortHlp is called to clean up a process that's being killed
// off. t is the process being terminated, to is the next process to
// run. At this point it is assumed that there _is_ a process to run.
// This runs on to's stack, so it's safe to delete t's stack.

void* ThreadData::AbortHlp (qt_t *, void* t, void* to)
{
    ThreadData* thread = (ThreadData*)t;
    if(thread->stack_alloc)
#ifdef USE_RZSTACKS
	RZStacks::delete_stk(thread->stack_alloc);
#else
	::delete [] thread->stack_alloc;
#endif
    thread->stack_alloc = thread->stack = 0;
    thread->sp = 0;
    currentThread = ((ThreadData*)to);
    return 0;
}


// Return the first runnable process. The returned process
// is put back in the queue as the last process at that priority,
// for round-robin within a priority.

ThreadData* ThreadData::nextThread()
{
    for(qtq* qp = &threadQueues[QT_MAXPRIO+1];
			qp >= &threadQueues[0]; qp--) {
	if(qp->len > 0) {
	    ThreadData* th = *qp->head++;
	    if(qp->head >= &qp->q[QT_QLEN])
		qp->head = &qp->q[0];
	    qp->len--;
	    th->addThread();	// round-robin
	    // This is the one...
	    return th;
	}
    }
    // Nobody to run.
    return 0;
}

// Put a thread into its priority queue. Dies horribly if
// there's no room in the queue.

void ThreadData::addThread()
{
    qtq* qp = &threadQueues[prio];
    if(qp->len >= QT_QLEN) {
	cerr << "Queue overflow in ThreadData::addThread()\n";
	abort();
    }
    *qp->tail++ = this;
    if(qp->tail >= &qp->q[QT_QLEN])
	qp->tail = &qp->q[0];
    qp->len++;
}

// Remove a thread from its queue. Not an error if it's not in one.

void ThreadData::removeThread()
{
    qtq* qp = &threadQueues[prio];
    int l = qp->len;

    if(l <= 0) {
	// If the queue is empty...
	return;
    }

    ThreadData** tp = qp->head;

    if(*tp == this) {
	// If the element being removed is the head of the queue
	if(++qp->head >= &qp->q[QT_QLEN])
	    qp->head = &qp->q[0];
	qp->len--;
	return;
    }

    if(l <= 1) {
	// The queue had only one entry, but it wasn't the thread that
	// was dequeueing itself...
	return;
    }

    // Search for the entry in the queue, and copy
    // the queue elements up by one after the entry
    // has been found. NB: The loop doesn't examine the
    // last element of the queue! That's done after the loop.

    ThreadData** tpnxt = tp+1;
    if(tpnxt >= &qp->q[QT_QLEN])
	tpnxt = &qp->q[0];
    int found = 0;
    while(l > 1) {
	if(*tp == this) {
	    // This is the one
	    found = 1;
	}
	// If we've already found the element, copy the
	// remainder of the queue up one plece.
	if(found) *tp = *tpnxt;
	tp = tpnxt;
	tpnxt++;
	if(tpnxt>= &qp->q[QT_QLEN])
	    tpnxt = &qp->q[0];
	l--;
    }

    // If the element was found in the loop, or it was the last
    // in the queue, adjust the length and the position of the tail.

    if(found || *tp == this) {
	if(--qp->tail < &qp->q[0])
	    qp->tail = &qp->q[QT_QLEN-1];
	qp->len--;
    }
}

int
ThreadData::topPrio()
{
    for(qtq* qp = &threadQueues[QT_MAXPRIO+1];
			qp >= &threadQueues[0]; qp--) {
	if(qp->len > 0) {
	    return qp - &threadQueues[0];
	}
    }
    // Nobody to run.
    return -1;
}

// Kill off a thread.

void ThreadData::terminateThread ()
{
    sp = 0;
    removeThread();	// remove the thread from the queue if it's in one
    if(this == currentThread) {
	// If we're suiciding, get the next eligible thread,
	ThreadData *next = nextThread();
	if(next == 0) {
	    cerr << "No-one to run in terminateThread\n";
	    abort();
	}
	// kill off this one, and start the new thread.
	// AbortHlp has to clean up the stack and any other resources.
	QT_ABORT(AbortHlp, this, next, next->sp);
    } else {
	// If we're being murdered by another thread, we have to clean
	// up ourselves.
	if(stack_alloc)
#ifdef USE_RZSTACKS
	    RZStacks::delete_stk(stack_alloc);
#else
	    ::delete [] stack_alloc;
#endif
	stack_alloc = stack = 0;
	sp = 0;
    }
}

INLINEF long ThreadData::Current_Thread ()
{
    return currentThread ? currentThread->thread_key : 0;
}

// Run the thread's Body() function as the process's code, and
// kill off the thread if Body() ever returns.

void ThreadData::Execute (void *t)
{
    Thread* thread = (Thread*)t;
    thread->Body();
    thread->terminateThread();
}

class Dummy_Main_Thread : public Thread {
public:
    Dummy_Main_Thread();
    void Body();
};

Dummy_Main_Thread::Dummy_Main_Thread()
	: Thread(FALSE)
{
}

void Dummy_Main_Thread::Body()
{
}

//
// Getting the main thread into the thread list...
//

void ThreadData::Initialize ()
{
    // Initialise the queues
    for(int i = QT_MAXPRIO+1; i >= 0; i--) {
	qtq* qtp = &threadQueues[i];
	qtp->len = 0;
	qtp->head = qtp->tail = &qtp->q[0];
    }

    // Create the thread for main, make it the current thread, and put
    // it in the queue.
    mainThread = new Dummy_Main_Thread();
    currentThread = mainThread->_data;
    currentThread->addThread();
}

Thread::Thread (Boolean create)
	       : thread_key(-1),
		 _data(create
		 	? new ThreadData(this, QT_DEFAULTSTACKSIZE)
			: new ThreadData()),
		 next(0),
		 prev(0)
{
    // Default is a 12kB stack.

    thread_key = _data->thread_key;
    Insert((Thread*) 0, this);
}

Thread::Thread (unsigned long stackSize)
	       : thread_key(-1),
		 _data(new ThreadData(this, stackSize)),
		 next(0),
		 prev(0)
{
    thread_key = _data->thread_key;
    Insert((Thread*) 0, this);
}

// tidy things up before we terminate thread

Thread::~Thread ()
{
    Remove(this);
    terminateThread();
}

void Thread::terminateThread ()
{
    if(_data) {
	_data->terminateThread();
	delete _data;
	_data = 0;
    }
}

// Make the thread un-runnable. If it was the current thread,
// run the highest priority runnable thread.

void Thread::Suspend ()
{
    ThreadData* td = _data;
    td->removeThread();
    if(td == ThreadData::currentThread) {
	ThreadData *next = ThreadData::nextThread();
	if(next == 0) {
	    cerr << "No-one to run in Suspend\n";
	    abort();
	}
	// It's not possible that this == next, because
	// removeThread() took this out of the queues before
	// nextThread() was called, so the QT_BLOCK is safe.
	QT_BLOCK(ThreadData::YeildHlp, td, next, next->sp);
    }
}

// Make the thread runnable. If there is now a higher-priority
// thread to run, run it, otherwise just return.

void Thread::Resume ()
{
    ThreadData* td = _data;
    td->addThread();
    if(ThreadData::topPrio() > ThreadData::currentThread->prio) {
	ThreadData *next = ThreadData::nextThread();
	if(next == 0) {
	    cerr << "No-one to run in Resume\n";
	    abort();
	}
	if(next != ThreadData::currentThread)
	    QT_BLOCK(ThreadData::YeildHlp, ThreadData::currentThread,
	    		next, next->sp);
    }
}


ostream& Thread::print (ostream& strm) const
{
    strm << "Thread type is Quick Thread.\n";
    strm << "    Thread key: " << thread_key << "\n";
    if(_data) {
	strm << hex
	     << "    stack allocation: 0x"
		<< ((unsigned long)(_data->stack_alloc))
	     << " stack: 0x" << ((unsigned long)(_data->stack))
	     << " stack pointer: 0x" << ((unsigned long)(_data->sp))
	     << dec
	     << " priority: " << _data->prio << "\n";;
    } else {
	strm << "    The thread has been terminated\n";
    }
    return strm;
}

void Thread::Initialize ()
{
    ThreadData::Initialize();
}

void Thread::Exit (int retValue)
{
    exit(retValue);
}

void Thread::mainResume ()
{
    ThreadData::mainThread->Resume();
}

long Thread::Current_Thread(void) const
{
    return ThreadData::Current_Thread();
}

/* The rites of Purification of threads */

#ifdef HAVE_PURE_THREADS_H_
extern "C"
{
#include <pure_threads.h>
}

int pure_thread_switch_protocol = PURE_THREAD_PROTOCOL_NOTICE_STACK_CHANGE;

int pure_thread_init_protocol = PURE_THREAD_INIT_IMPLICIT;

unsigned int
pure_thread_id_size(void)
{
  return sizeof (long);
}

void
pure_thread_id(void* tid_p)
{
  *(long*)tid_p = ThreadData::Current_Thread();
}

int
pure_thread_id_equal(void* tid_p0, void* tid_p1)
{
  return *(long*)tid_p0 == *(long*)tid_p1;
}

#endif

/*
 * The mutex.
 */

QuickMutex::QuickMutex ()
{
    mutex_init(&_theLock, 0, 0);
}

QuickMutex::~QuickMutex ()
{
    mutex_destroy(&_theLock);
}

Boolean QuickMutex::lock ()
{
    return TRUE;
}

Boolean QuickMutex::unlock ()
{
    return TRUE;
}

/*
 * Now the Mutex create method.
 */

Mutex* Mutex::create ()
{
    return new QuickMutex;
}
