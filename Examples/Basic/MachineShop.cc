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
 * $Id: MachineShop.cc,v 1.10 1998/08/28 14:19:49 nmcl Exp $
 */

#include <iostream>

#ifndef PROCESS_H_
#  include <ClassLib/Process.h>
#endif

#ifndef MEAN_H_
#  include <Stat/Mean.h>
#endif

#ifndef MACHINESHOP_H_
#  include "MachineShop.h"
#endif

#ifndef ARRIVALS_H_
#  include "Arrivals.h"
#endif

#ifndef MACHINE_H_
#  include "Machine.h"
#endif

#ifndef JOB_H_
#  include "Job.h"
#endif

#ifndef QUEUE_H_
#  include "Queue.h"
#endif

#ifndef BREAKS_H_
#  include "Breaks.h"
#endif

Machine* M;
Queue JobQ;
Mean meanJobs;
long TotalJobs = 0;
long ProcessedJobs = 0;
double TotalResponseTime = 0.0;
double MachineActiveTime = 0.0;
double MachineFailedTime = 0.0;

MachineShop::MachineShop (Boolean isBreaks)
                         : useBreaks(isBreaks)
{
}

MachineShop::~MachineShop () {}

void MachineShop::Body ()
{
    Breaks* B = (Breaks*) 0;
    Arrivals* A = new Arrivals(8);
    M = new Machine(8);
    Job* J = new Job;

    if (useBreaks)
    {
	B = new Breaks;
    }

    A->Activate();

    if (useBreaks)
	B->Activate();

    Scheduler::scheduler().Resume();

    while (ProcessedJobs < 1000)
    {
#ifdef DEBUG
	cout << "Processed " << ProcessedJobs << endl;
#endif
	Hold(1000);
    }

    cout << "Total number of jobs present " << TotalJobs << endl;
    cout << "Total number of jobs processed " << ProcessedJobs << endl;
    cout << "Total response time of " << TotalResponseTime << endl;

    cout << "Average response time = "
	 << (TotalResponseTime / ProcessedJobs) << endl;
    cout << "Probability that machine is working = "
	 << ((MachineActiveTime - MachineFailedTime) / CurrentTime()) << endl;
    cout << "Probability that machine has failed = "
	 << (MachineFailedTime / MachineActiveTime) << endl;
    cout << "Average number of jobs present = " << meanJobs.mean() << endl;
    
    Scheduler::scheduler().reset();
    Scheduler::scheduler().Suspend();

    A->terminate();
    M->terminate();
    
    if (useBreaks)
	B->terminate();

    if (A)
	delete A;
    if (B)
	delete B;
    if (M)
	delete M;

    /*
     * Must reactivate main thread before this thread "dies" or there
     * will be nothing waiting to "catch" us.
     */
    
    Thread::mainResume();
}

void MachineShop::Await ()
{
    Resume();
    Thread::Self()->Suspend();
}

void MachineShop::Exit ()
{
    Thread::Exit();
}
