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
 * $Id: c_thread.h,v 1.2 1998/08/28 14:19:33 nmcl Exp $
 */

#ifndef C_THREAD_H_
#define C_THREAD_H_

extern "C"
{
#include <cthread.h>
}

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

class Thread;

/*
 * General notes: the C Threads library does not have any notion of priorities.
 */

/*
 * This is a threads interface to the C threads library.
 */

class ThreadData
{
public:
    ThreadData ();
    ~ThreadData ();
    
    cthread_t cid;
    mutex_t   mx;
    Boolean   dead;

    static void* Execute (void* p1);
    
    static long base_key;
    static Thread* mainThread;
    static long mainThreadID;
};

class C_Mutex : public Mutex
{
public:
    C_Mutex ();
    ~C_Mutex ();

    Boolean lock ();
    Boolean unlock ();

private:
    mutex_t _theLock;
};

#endif
