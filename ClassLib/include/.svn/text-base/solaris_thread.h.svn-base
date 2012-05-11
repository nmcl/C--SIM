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
 * $Id: solaris_thread.h,v 1.4 1998/10/05 11:07:43 nmcl Exp $
 */

#ifndef SOLARIS_THREAD_H_
#define SOLARIS_THREAD_H_

#include <thread.h>
#include <synch.h>

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef THREAD_H_
#  include <ClassLib/thread.h>
#endif

class ostream;

/*
 * This is the Sun Solaris thread implementation of the Thread virtual class.
 */

class ThreadData
{
public:
    ThreadData ();
    ~ThreadData ();

    static void* Execute (void*); // This routine calls the 'main' object code

    thread_t mid;
    sema_t sp;
    sema_t waitSem;
    Boolean dead;
};

class SolarisMutex : public Mutex
{
public:
    SolarisMutex ();
    ~SolarisMutex ();

    Boolean lock ();
    Boolean unlock ();

private:
    mutex_t _theLock;
};

#endif
