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
 * $Id: lwp_thread.h,v 1.2 1998/08/28 14:19:35 nmcl Exp $
 */

#ifndef LWP_THREAD_H_
#define LWP_THREAD_H_

extern "C"
{
#include <lwp/lwp.h>
}

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef THREAD_H_
#  include <ClassLib/thread.h>
#endif

extern "C"
{
#include <lwp/stackdep.h>
}

class ostream;

/*
 * This is the Sun thread implementation of the Thread virtual class. It
 * provides an implementation for all of the pure virtual functions declared
 * in the Thread base class.
 *
 * Note: if any problems occur when using the Sun thread package, try
 * increasing the stack size and/or the number of stacks to create in
 * Initialize.
 */

class ThreadData
{
public:
    ThreadData ();
    ~ThreadData ();
    
    // This routine calls the 'main' object code
   
    static void Execute (Thread*);
    
    stkalign_t* stack;

    thread_t mid;
};

class LWP_Mutex : public Mutex
{
public:
    LWP_Mutex ();
    ~LWP_Mutex ();

    Boolean lock ();
    Boolean unlock ();

private:
    mon_t _theLock;
};

#endif
