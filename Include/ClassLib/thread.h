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
 * $Id: thread.h,v 1.14 1998/08/28 14:19:53 nmcl Exp $
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <iostream>
#include <ostream>

using std::cerr;
using std::cout;
using std::endl;
using std::ostream;
using std::istream;

#ifndef RESOURCE_H_
#  include <Common/Resource.h>
#endif

/*
 * Because not every thread package provides an easy way of identifying and
 * locating threads, the Thread class does provide such a scheme through the
 * use of the Identify and Self operations. A linked list of threads is formed
 * and added to whenever a new thread is created.
 */

class ThreadData;

class Thread : public Resource
{
    friend class ThreadData;

public:
    virtual void Suspend (); // How to suspend a thread
    virtual void Resume ();  // How to resume a suspended thread

    virtual void Body () = 0;    // The 'main' part of the code

    // Should return some unique thread identity key

    virtual long Current_Thread () const;

    virtual long Identity () const; // Returns the identify of this thread

    static Thread* Self ();         // Returns the current thread

    // Exit program

    static void Exit (int = 0);

    // Need routine to explicitly resume main.

    static void mainResume ();

    // Initialize must be called exactly once at the start of the program

    static void Initialize ();

    virtual ostream& print (ostream&) const;
    static  ostream& printAll (ostream&);

protected:
    Thread (Boolean create = TRUE);
    Thread (unsigned long stackSize);
    virtual ~Thread ();

    virtual void terminateThread (); // terminate and release resources

    long thread_key;    // key which must be set in derived class.
    ThreadData* _data;

private:
    void Insert (Thread*, Thread*);
    void Remove (Thread*);

    Thread* next;
    Thread* prev;

    static Thread* _head;
    static Boolean _initialized;
};


class Mutex : public Resource
{
public:
    virtual Boolean lock () = 0;
    virtual Boolean unlock () = 0;

    static Mutex* create ();

protected:
    Mutex ();
    virtual ~Mutex ();
};

extern ostream& operator<< (ostream& strm, const Thread& t);

#endif
