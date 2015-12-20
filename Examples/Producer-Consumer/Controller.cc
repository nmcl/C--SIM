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
 * Copyright (C) 1994, 1995, 1996, 1997
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Controller.cc,v 1.1 1996/12/09 10:10:04 nmcl Exp $
 */

#include <iostream>

#ifndef CONTROLLER_H_
#  include "Controller.h"
#endif

#ifndef PRODUCER_H_
#  include "Producer.h"
#endif

#ifndef CONSUMER_H_
#  include "Consumer.h"
#endif

#ifndef QUEUE_H_
#  include "Queue.h"
#endif

long TotalNumberOfJobs = 0;
long NumberOfJobsConsumed = 0;
Queue JobQueue;

Controller::Controller ()
{
}

Controller::~Controller () {}

void Controller::Body ()
{
    Producer* p = new Producer(10);
    Consumer* c = new Consumer(10);

    p->Activate();
    c->Activate();

    Scheduler::scheduler().Resume();

    Hold(10000);

    cout << "Total number of jobs present " << TotalNumberOfJobs << endl;
    cout << "Total number of jobs processed " << NumberOfJobsConsumed << endl;
    
    Scheduler::scheduler().reset();
    Scheduler::scheduler().Suspend();

    p->terminate();
    c->terminate();

    if (p)
	delete p;
    if (c)
	delete c;

    /*
     * Must reactivate main thread before this thread "dies" or there
     * will be nothing waiting to "catch" us.
     */
    
    Thread::mainResume();
}

void Controller::Await ()
{
    Resume();
    Thread::Self()->Suspend();
}

void Controller::Exit ()
{
    Thread::Exit();
}
