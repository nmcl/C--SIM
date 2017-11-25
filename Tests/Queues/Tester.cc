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
 * Copyright (C) 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Tester.cc,v 1.10 1996/12/04 15:08:04 nmcl Exp $
 */

#include <iostream>

#ifndef TESTER_H_
#  include "Tester.h"
#endif

#ifndef DUMMYPROCESS_H_
#  include "DummyProcess.h"
#endif

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
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
#    include "Calendar.h"
#  endif
#endif

#ifdef HashedList_Queue
#  ifndef HASHEDLIST_H_
#    include "HashedList.h"
#  endif
#endif


extern Queue_Type ReadyQueue;


Tester::Tester (long elements, StreamType st)
               : number(elements),
                 hi(1000.0),
                 lo(10.0),
                 mean(20.0),
                 std(5.0),
                 streamType(st)
{
    us = (UniformStream*) 0;
    ns = (NormalStream*) 0;
    es = (ExponentialStream*) 0;

    switch(st)
    {
    case UNIFORM:
        us = new UniformStream(lo, hi);
	break;
    case NORMAL:
	ns = new NormalStream(mean, std);
	break;
    case EXPONENTIAL:
	es = new ExponentialStream(mean);
	break;
    }
}

Tester::~Tester ()
{
    if (us)
        delete us;
    if (ns)
        delete ns;
    if (es)
        delete es;
}

/*
 * We only want to test the queue, so no need to start
 * the scheduler.
 */

void Tester::Body ()
{
    DummyProcess *dp = (DummyProcess*) 0;
    int i;

    // put half of the required elements in the list

    for (i = 0; i < number; i++)
    {
        dp = new DummyProcess;

	switch(streamType)
	{
	case UNIFORM:
	    dp->ActivateAt((*us)());
	    break;
	case NORMAL:
	    dp->ActivateAt((*ns)());
	    break;
	case EXPONENTIAL:
	    dp->ActivateAt((*es)());
	    break;
	}
    }

    Thread::mainResume();
}

void Tester::tidy ()
{
  DummyProcess* dp = 0;
#ifdef DEBUG
    cout << "The list structure is:" << ReadyQueue << endl;
#endif

    for (int i = 0; i < number; i++)
    {
	dp = (DummyProcess*) ReadyQueue.Remove();
#ifdef DEBUG
	cout << "Removed " << dp->evtime() << endl;
#endif
	if (dp)
	{
	  delete dp;
	}
    }
}

void Tester::Await ()
{
    Resume();
    Thread::Self()->Suspend();
}

void Tester::Exit ()
{
    Thread::Exit();
}

