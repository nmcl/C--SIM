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
 * $Id: Consumer.cc,v 1.2 1998/09/30 08:07:17 nmcl Exp $
 */

#ifndef CONSUMER_H_
#  include "Consumer.h"
#endif

#ifndef JOB_H_
#  include "Job.h"
#endif

#ifndef QUEUE_H_
#  include "Queue.h"
#endif

#ifndef PRODUCER_H_
#  include "Producer.h"
#endif

extern Queue JobQueue;
extern long NumberOfJobsConsumed;

Semaphore Consumer::_semaphore(0);

Consumer::Consumer (double mean)
		   : InterArrivalTime(new ExponentialStream(mean))
{
}

Consumer::~Consumer () { delete InterArrivalTime; }

void Consumer::Body ()
{
    for (;;)
    {
	if (JobQueue.IsEmpty())
	    Consumer::_semaphore.Get(this);

	Job* work = JobQueue.Dequeue();

	Producer::_semaphore.Release();
	NumberOfJobsConsumed++;
	
	Hold((*InterArrivalTime)());
    }
}
