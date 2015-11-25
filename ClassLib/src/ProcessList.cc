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

#ifndef PROCESSLIST_H_
#  include "ProcessList.h"
#endif

#ifndef PROCESSITERATOR_H_
#  include "ProcessIterator.h"
#endif

#include <iostream>


ProcessList::ProcessList () { Head = (ProcessCons*) 0; }

ProcessList::~ProcessList ()
{
    if (Head)
	delete Head;
}

// returns the next process to be activated after current

const Process* ProcessList::getNext (const Process* current) const
{
    // take care of boundary condition - empty list.

    if ((Head == (ProcessCons*) 0) || (current == (Process*) 0))
        return (Process*) 0;

    for (ProcessCons *ptr = Head; ptr; ptr = ptr->cdr())
    {
	if (ptr->car() == current)
	    return ((ptr->cdr() == (ProcessCons*) 0) ? (Process*) 0 : ptr->cdr()->car());
	else  // terminate search - past the point current could be
	    if (ptr->car()->evtime() > current->evtime())
	        break;
    }
    
    /*
     * If we get here then we have not found current on the list
     * which can only mean that it is currently active.
     */

    return Head->car();
}

void ProcessList::Insert (Process &p, Boolean prior)
{
    // If list is empty, insert at head
    if (!Head)
    {
	Head = new ProcessCons(p, Head);
	return;
    }

    // Try to insert before (if there is anything scheduled later)
    ProcessIterator iter(*this);
    Process* prev = (Process*) 0;
    
    for (Process* q = iter(); q; prev = q, q = iter())
    {
	if (prior)
	{
	    if (q->evtime() >= p.evtime())
	    {
		(void) InsertBefore(p, *q);
		return;
	    }
	}
	else
	    if (q->evtime() > p.evtime())
	    {
		(void) InsertBefore(p, *q);
		return;
	    }
    }

    // Got to insert at the end (currently pointed at by 'prev')
    (void) InsertAfter(p, *prev);
}

Boolean ProcessList::InsertBefore (Process &ToInsert, Process &Before)
{
    for (ProcessCons *prev=0, *p=Head; p; prev=p, p=p->cdr())
	if (p->car() == &Before) {
	    ProcessCons *newcons = new ProcessCons(ToInsert, p);
	    if (prev)
		prev->SetfCdr(newcons);
	    else 
		Head = newcons;

	    return TRUE;
	}
    return FALSE;
}

Boolean ProcessList::InsertAfter (Process &ToInsert, Process &After)
{
    for (ProcessCons *p = Head; p; p = p->cdr())
	if (p->car() == &After) {
	    ProcessCons *newcons = new ProcessCons(ToInsert, p->cdr());
	    p->SetfCdr(newcons);
	    return TRUE;
	}
    return FALSE;
}

Process *ProcessList::Remove (const Process *element)
{
    Process *p = (Process*) 0;

    // Take care of boundary condition - empty list
    if (!Head) return (Process*) 0;

    // Change unspecified element to "remove head of list" request
    if (element == (Process*) 0)
	return(Remove(Head->car()));

    for (ProcessCons *prev = (ProcessCons*) 0, *ptr = Head; ptr; prev = ptr, ptr = ptr->cdr())
    {
	if (ptr->car() == element)
	{
	    ProcessCons *oldcons = ptr;
	    // unlink the cons cell for the element we're removing
	    if (prev)
		prev->SetfCdr(ptr->cdr());
	    else
		Head = ptr->cdr();
	    // return the pointer to the process
	    p = ptr->car();
	    // flush the dead cons cell
	    delete oldcons;
	    return p;
	}
    }

    return (Process*) 0;
}

ostream& ProcessList::print (ostream& strm) const
{
    ProcessIterator iter(*this);
    int count = 1;

    for (Process* p = iter(); p; p = iter(), count++)
	strm << "Process number " << count << " with start time "
	     << p->evtime() << " and identity " << p->Identity() << "\n";
    
    return strm;
}

ProcessCons* ProcessList::operator[] (int index)
{
    ProcessCons* ptr = Head;
    for (int i = 0; i < index; i++)
        ptr = ptr->cdr();

    return ptr;
}

ostream& operator<< (ostream& strm, const ProcessList& pl)
{
    return pl.print(strm);
}
