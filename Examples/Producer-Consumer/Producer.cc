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
 * $Id: Producer.cc,v 1.2 1998/09/30 08:07:24 nmcl Exp $
 */

#ifndef PRODUCER_H_
#  include "Producer.h"
#endif

#ifndef JOB_H_
#  include "Job.h"
#endif

#ifndef QUEUE_H_
#  include "Queue.h"
#endif

#ifndef CONSUMER_H_
#  include "Consumer.h"
#endif

extern Queue JobQueue;
extern long TotalNumberOfJobs;

Semaphore Producer::_semaphore(0);

Producer::Producer (double mean)
		   : InterArrivalTime(new ExponentialStream(mean))
{
}

Producer::~Producer () { delete InterArrivalTime; }

void Producer::Body ()
{
    for (;;)
    {
	Job* work = new Job();

	if (JobQueue.IsFull())
	    Producer::_semaphore.Get(this);

	TotalNumberOfJobs++;
	JobQueue.Enqueue(work);
	Consumer::_semaphore.Release();
	
	Hold((*InterArrivalTime)());
    }
}
