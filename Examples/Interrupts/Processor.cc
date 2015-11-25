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
 */

#include <iostream>

#ifndef PROCESSOR_H_
#  include "Processor.h"
#endif

#ifndef QUEUE_H_
#  include "Queue.h"
#endif

#ifndef JOB_H_
#  include "Job.h"
#endif


extern Queue JobQ;
extern Queue SignalQ;
extern long ProcessedJobs;
extern long SignalledJobs;


Processor::Processor (double mean)
                                 : sTime(new ExponentialStream(mean))
{
}

Processor::~Processor ()
{
    if (sTime)
        delete sTime;
}

void Processor::Body ()
{
    Job* j = (Job*) 0;

    for (;;)
    {
	if (Wait((*sTime)()))
	{
	    if (!JobQ.IsEmpty())
	    {
	        j = JobQ.Dequeue();
		delete j;
		ProcessedJobs++;
	    }
	}
	else
	{
	    if (SignalQ.IsEmpty())
	        cerr << "Error - signal caught, but no message given!" << endl;
	    else
	    {
	        j = SignalQ.Dequeue();
		delete j;
		SignalledJobs++;
	    }
	}

	if (SignalledJobs == 2)
	    terminate();
    }
}
