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
 * $Id: Queue.cc,v 1.1 1996/12/09 10:10:10 nmcl Exp $
 */

#ifndef QUEUE_H_
#  include "Queue.h"
#endif

static long MaximumQueueSize = 10;

Queue::Queue ()
	     : head(0),
	       numberOfJobs(0)
{
}

Queue::~Queue ()
{
    List* ptr = head;

    while (head)
    {
	head = ptr->next;
	delete ptr->work;
	delete ptr;
	ptr = head;
    }
}

Boolean Queue::IsEmpty () const
{
    return (Boolean) (numberOfJobs == 0);
}

Boolean Queue::IsFull () const
{
    return (Boolean) (numberOfJobs == MaximumQueueSize);
}

Job* Queue::Dequeue ()
{
    if (IsEmpty())
	return 0;

    List* ptr = head;
    head = head->next;

    numberOfJobs--;

    Job* toReturn = ptr->work;
    delete ptr;

    return toReturn;
}

void Queue::Enqueue (Job* toadd)
{
    if (!toadd)
	return;

    List* ptr = head;
    
    if (IsEmpty())
    {
	head = new List;
	ptr = head;
    }
    else
    {
	while (ptr->next != 0)
	    ptr = ptr->next;

	ptr->next = new List;
	ptr = ptr->next;
    }

    ptr->next = 0;
    ptr->work = toadd;
    numberOfJobs++;
}

    
