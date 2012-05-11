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
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: posix_thread.h,v 1.7 1998/10/05 12:03:51 nmcl Exp $
 */

#ifndef POSIX_THREAD_H_
#define POSIX_THREAD_H_

#include <iostream.h>
#include <pthread.h>
#include <semaphore.h>

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef THREAD_H_
#  include <ClassLib/thread.h>
#endif

#define any_t void*

/*
 * This is the Posix thread implementation of the Thread virtual class.
 */

class ThreadData
{
public:
    ThreadData ();
    ~ThreadData ();

    // This routine calls the 'main' object code

    static void* Execute (void*);

    Boolean dead;
    pthread_t _thread;
#ifndef PTHREAD_DRAFT_SOLARIS
    pthread_mutex_t _lock;
#else
    sem_t waitSem;
    sem_t _lock;
#endif
    pthread_attr_t _attr;

    static long base_key;
};


class PosixMutex : public Mutex
{
public:
    PosixMutex ();
    ~PosixMutex ();

    Boolean lock ();
    Boolean unlock ();

private:
    pthread_mutex_t _theLock;
};

#endif
