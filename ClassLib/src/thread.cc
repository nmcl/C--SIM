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
 * Copyright (C) 1991-1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: thread.cc,v 1.15 1998/08/28 14:19:45 nmcl Exp $
 */

#ifndef THREAD_H_
#  include <ClassLib/thread.h>
#endif

Thread* Thread::_head = (Thread*) 0; // Initialise head of Thread list
Boolean Thread::_initialized = FALSE;

void Thread::Insert (Thread* insertPosition, Thread* toInsert)
{
    if (insertPosition == (Thread*) 0)  // must be head of list
    {
	toInsert->next = _head;
	if (_head)
	    _head->prev = toInsert;
	_head = toInsert;
    }
    else
    {
	if (insertPosition->next)
	    insertPosition->next->prev = toInsert;
	toInsert->next = insertPosition->next;
	toInsert->prev = insertPosition;
	insertPosition->next = toInsert;
    }
}

void Thread::Remove (Thread* toRemove)
{
    if (toRemove->prev == (Thread*) 0)  // deal with head of list first
    {
	_head = toRemove->next;
	if (_head != 0)
	  _head->prev = (Thread*) 0;
    }
    else
    {
	toRemove->prev->next = toRemove->next;
	if (toRemove->next)
	    toRemove->next->prev = toRemove->prev;
    }
}

long Thread::Identity () const { return thread_key; }

Thread* Thread::Self ()
{
    if (!_head) return (Thread*) 0;

    // Use any thread object (e.g., _head) to get access to the current thread

    long my_id = _head->Current_Thread();
    Thread* marker = (Thread*) 0;
    
    for (marker = _head; marker; marker = marker->next)
	if (marker->thread_key == my_id)
	    break;

    return marker;
}

// go through list and print out information on all threads

ostream& Thread::printAll (ostream& strm)
{
    strm << "\n";
    for (Thread* marker = _head; marker; marker = marker->next)
	strm << "Thread key: " << marker->thread_key << "\n";
    
    return strm;
}

ostream& operator<< (ostream& strm, const Thread& t)
{
    return t.print(strm);
}

/*
 * All the real work is done in the thread specific
 * class.
 */

Mutex::Mutex ()
{
}

Mutex::~Mutex ()
{
}
