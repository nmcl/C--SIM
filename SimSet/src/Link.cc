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

/*
 * This class defines the elements of the linked lists within SIMSET.
 */

#include <iostream>

#ifndef LINK_H_
#  include <SimSet/Link.h>
#endif

#ifndef HEAD_H_
#  include <SimSet/Head.h>
#endif


Link::Link ()
            :
	      prev(0),
	      next(0),
	      theList(0)
{
}

Link::~Link () { RemoveElement(); }

void Link::RemoveElement ()
{
    // can't have prev and next if we are not on a list
    
    if (!theList)
	return;

    if (prev)
	prev->next = next;

    if (next)
	next->prev = prev;

    if (theList->first == this)
	theList->first = next;

    if (theList->last == this)
	theList->last = prev;

    theList = (Head*) 0;
    prev = next = (Link*) 0;
}

Link* Link::Out ()
{
    RemoveElement();
    return this;
}

void Link::InTo (Head* list)
{
    if (list)
    {
	list->AddLast(this);
	theList = list;
	return;
    }
    
    (void) Out();
}

void Link::Precede (Link* toPrecede)
{
    if ((!toPrecede) || (!toPrecede->inList()))
	(void) Out();
    else
    {
	if (inList())
	    (void) Out();
	    
	toPrecede->addBefore(this);
    }
}

void Link::Follow (Link* toFollow)
{
    if ((!toFollow) || (!toFollow->inList()))
	(void) Out();
    else
    {
	if (inList())
	    (void) Out();
	    
	toFollow->addAfter(this);
    }
}

void Link::addAfter (Link* toAdd)
{
    toAdd->prev = this;
    toAdd->theList = theList;

    if (!next)
	next = toAdd;
    else
    {
	next->prev = toAdd;
	toAdd->next = next;
	next = toAdd;
    }

    if (theList->last == this)
	theList->last = toAdd;
}

void Link::addBefore (Link* toAdd)
{
    toAdd->theList = theList;
    toAdd->next = this;

    if (!prev)
	prev = toAdd;
    else
    {
	prev->next = toAdd;
	toAdd->prev = prev;
	prev = toAdd;
    }

    if (theList->first == this)
	theList->first = toAdd;
}

void Link::Follow (Head* list)
{
    if (list)
	list->AddFirst(this);
}


#ifdef NO_INLINES
#  define LINK_CC_
#  include <SimSet/Link.n>
#  undef LINK_CC_
#endif
