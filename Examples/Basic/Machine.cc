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
 * $Id: Machine.cc,v 1.4 1998/08/28 14:19:48 nmcl Exp $
 */

#ifndef MEAN_H_
#  include <Stat/Mean.h>
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

#include <iostream>

extern Mean meanJobs;
extern long ProcessedJobs;
extern double MachineActiveTime;
extern Queue JobQ;

Machine::Machine (double mean)
                 : STime(new ExponentialStream(mean)),
                   operational(TRUE),
                   working(FALSE),
		   J(0)
{
}

Machine::~Machine ()
{
    delete STime;
    if (J)
	delete J;
}

void Machine::Body ()
{
    static int n = 0;
    
    double ActiveStart, ActiveEnd;

    for (;;)
    {
	working = TRUE;

	while (!JobQ.IsEmpty())
	{
	    ActiveStart = CurrentTime();

	    meanJobs += JobQ.QueueSize();

	    J = JobQ.Dequeue();

	    double serviceTime = ServiceTime();

	    Hold(serviceTime);
	    
	    ActiveEnd = CurrentTime();
	    MachineActiveTime += ActiveEnd - ActiveStart;
	    ProcessedJobs++;
	    delete J;
	    J = 0;
	}

	working = FALSE;
	Cancel();
    }
}

Boolean Machine::Processing () { return working; }

void Machine::Broken () { operational = FALSE; }

void Machine::Fixed () { operational = TRUE; }

Boolean Machine::IsOperational () { return operational; }

double Machine::ServiceTime () { return (*STime)(); }
