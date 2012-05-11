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
 */

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef PROCESS_H_
#  include <ClassLib/Process.h>
#endif

#ifndef JOB_H_
#  include "Job.h"
#endif

#ifndef MACHINE_H_
#  include "Machine.h"
#endif

#ifndef QUEUE_H_
#  include "Queue.h"
#endif

#ifdef DEBUG
#  include <iostream.h>
#endif

extern Machine* M;
extern Queue JobQ;
extern long TotalJobs;
extern double TotalResponseTime;

Job::Job ()
{
    Boolean empty;

    ResponseTime = 0;
    ArrivalTime = Scheduler::scheduler().CurrentTime();
    empty = JobQ.IsEmpty();
    JobQ.Enqueue(this);
    TotalJobs++;

    if (empty && !M->Processing() && M->IsOperational())
	M->Activate();
}

Job::~Job ()
{
    ResponseTime = Scheduler::scheduler().CurrentTime() - ArrivalTime;

#ifdef DEBUG
    cout << "ArrivalTime is " << ArrivalTime << endl;
    cout << "Time now is " << Scheduler::scheduler().CurrentTime() << endl;
    cout << "ResponseTime is " << ResponseTime << endl;
#endif

    TotalResponseTime += ResponseTime;
}
