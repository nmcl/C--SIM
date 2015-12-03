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

#ifndef PROCESSHEAP_H_
#  include "ProcessHeap.h"
#endif

#ifndef HEAPITERATOR_H_
#  include "HeapIterator.h"
#endif

#ifndef HEAPELEMENT_H_
#  include "HeapElement.h"
#endif

#ifndef PROCESSITERATOR_H_
#  include "ProcessIterator.h"
#endif

const int factor = 25;

ProcessHeap::ProcessHeap ()
			 : numberOfElements(0)
{
    Head = (HeapElement*) 0;
}

ProcessHeap::~ProcessHeap ()
{
    if (Head)
    {
        HeapElement *ptr = Head, *trail = (HeapElement*) 0;

	while (ptr)
	{
	    trail = ptr->cdr();
	    delete ptr;
	    ptr = trail;
	}
    }
}

// returns the next process to be activated after current

const Process* ProcessHeap::getNext (const Process* current) const
{
    // take care of boundary condition - empty list.

    if ((Head == (HeapElement*) 0) || (current == (Process*) 0))
        return (Process*) 0;

    HeapIterator h_iter(*this);
    
    for (ProcessList* q = h_iter(); q; q = h_iter())
    {
	if (h_iter.bucketName() > (long) (current->evtime()/factor)) // no 'current', so must be active
            break;

	if (h_iter.bucketName() == (long) (current->evtime()/factor))
	{
	    ProcessIterator p_iter(*q);
	    for (Process* p = p_iter(); p; p = p_iter())
	    {
		if (p == current)
		{
		    p = p_iter(); // anything else in this queue?
		    if (p)
		        return p;
		    q = h_iter(); // anything else in tree?
		    return ((q) ? (*q)[0]->car() : (Process*) 0);
		}
	    }
	}
    }
    
    /*
     * If we get here then we have not found current on the list
     * which can only mean that it is currently active.
     */

    return (*Head->car())[0]->car();
}

void ProcessHeap::Insert (Process &ToInsert, Boolean prior)
{
#ifdef DEBUG
    cout << "void ProcessHeap::Insert ( " << ToInsert.evtime() << ", "
		 << ((prior) ? "TRUE" : "FALSE") << " )" << endl;
#endif    

    // If list is empty, insert at head

    numberOfElements++;
    
    if (!Head)
    {
	Head = new HeapElement(ToInsert);
	return;
    }

    /*
     * At this level we are only concerned with getting on to the right queue.
     * We let the queue then do the work concerned with the 'prior' flag.
     */

    HeapIterator h_iter(*this);
    HeapElement* ptr = (HeapElement*) 0;

    for (ProcessList* q = h_iter(); q; ptr = h_iter.current(), q = h_iter())
    {
	if (h_iter.bucketName() >= (long) (ToInsert.evtime()/factor))
	{
	    if (h_iter.bucketName() == (long) (ToInsert.evtime()/factor)) // queue exists so use it
            {
		q->Insert(ToInsert, prior);
		return;
	    }

	    // create new queue for this element

            InsertAt(ToInsert, ptr);
	    return;
	}
    }

    // Got to insert at the end (currently pointed at by 'ptr')
    InsertAt(ToInsert, ptr);
}

Process* ProcessHeap::Remove (const Process *element)
{
    HeapIterator h_iter(*this);
    HeapElement* ptr = (HeapElement*) 0;

    // Take care of boundary condition - empty list

    if ((!Head) || (numberOfElements == 0))
	return (Process*) 0;

    // Change unspecified element to "remove head of list" request

    if (element == (Process*) 0)
	return Remove(findMinimum());

    for (ProcessList* p = h_iter(); p; ptr = h_iter.current(), p = h_iter())
    {
	if (h_iter.bucketName() == (long) (element->evtime()/factor))
	{
	    Process* oldElement = p->Remove(element);
	    if ((*p)[0] == (ProcessCons*) 0) // empty list
		RemoveFrom(ptr);

	    numberOfElements--;
	    
	    return oldElement;
	}

	if (h_iter.bucketName() > (long) (element->evtime()/factor))
	    break;
    }

    return (Process*) 0;
}

Boolean ProcessHeap::InsertBefore (Process& ToInsert, Process& Before)
{
#ifdef DEBUG
    cout << "Boolean ProcessHeap::InsertBefore ( " << ToInsert.evtime() << ", "
		 << Before.evtime() << " )" << endl;
#endif

    HeapIterator h_iter(*this);
    HeapElement* ptr = (HeapElement*) 0;

    numberOfElements++;
    
    for (ProcessList *q = h_iter(); q; ptr = h_iter.current(), q = h_iter())
    {
	if (h_iter.bucketName() == (long) (Before.evtime()/factor))  // Before must be in list
	    return q->InsertBefore(ToInsert, Before);

	if (h_iter.bucketName() > (long) (Before.evtime()/factor))
	    break;
    }

    return FALSE;
}

Boolean ProcessHeap::InsertAfter (Process &ToInsert, Process &After)
{
#ifdef DEBUG
    cout << "Boolean ProcessHeap::InsertAfter ( " << ToInsert.evtime() << ", "
		 << After.evtime() << " )" << endl;
#endif

    HeapIterator h_iter(*this);
    HeapElement* ptr = (HeapElement*) 0;

    numberOfElements++;
    
    for (ProcessList* q = h_iter(); q; ptr = h_iter.current(), q = h_iter())
    {
	if (h_iter.bucketName() == (long) (After.evtime()/factor))
	    return q->InsertAfter(ToInsert, After);

	if (h_iter.bucketName() > (long) (After.evtime()/factor))
	    break;
    }

    return FALSE;
}

Process* ProcessHeap::findMinimum () const
{
    if (!Head) return (Process*) 0;

    HeapIterator h_iter(*this);
    ProcessList* p1 = h_iter();
    Process* toRemove = (*p1)[0]->car();
	
    for (double minimum = (*p1)[0]->car()->evtime(); p1; p1 = h_iter())
    {
	if ((*p1)[0]->car()->evtime() <= minimum)
	{
	    minimum = (*p1)[0]->car()->evtime();
	    toRemove = (*p1)[0]->car();
	}
    }

    return toRemove;
}

void ProcessHeap::InsertAt (Process& ToInsert, HeapElement* insertPoint)
{
    HeapElement* newElement = (HeapElement*) 0;

    if (insertPoint == (HeapElement*) 0)  // insert at Head of list
    {
        newElement = new HeapElement(ToInsert, Head);
	Head = newElement;
    }
    else
    {
        newElement = new HeapElement(ToInsert, insertPoint->cdr());
	insertPoint->SetfCdr(newElement);
    }

    newElement->setBucketName((long) (ToInsert.evtime()/factor));
}

void ProcessHeap::RemoveFrom (HeapElement* removalPoint)
{
    HeapElement* oldElement = (HeapElement*) 0;

    if (removalPoint == (HeapElement*) 0)  // remove Head of list
    {
        oldElement = Head;
	Head = Head->cdr();
    }
    else
    {
        oldElement = removalPoint->cdr();
	removalPoint->SetfCdr(removalPoint->cdr()->cdr());
    }

    delete oldElement;
}

ostream& ProcessHeap::print (ostream& strm) const
{
    HeapIterator h_iter(*this);
    int count = 1;
    
    for (ProcessList* p = h_iter(); p; p = h_iter(), count++)
    {
        strm << "\nEvent level " << count << "\n";
	strm << *p;
    }
    
    return strm;
}

ostream& operator<< (ostream& strm, const ProcessHeap& ph)
{
    return ph.print(strm);
}
    
