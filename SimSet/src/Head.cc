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
 * This class essentially defines the linked list manager used by the SIMSET
 * class in SIMULA.
 */

#include <iostream>

#ifndef HEAD_H_
#  include <SimSet/Head.h>
#endif

#ifndef LINK_H_
#  include <SimSet/Link.h>
#endif


Head::Head ()
            : first(0),
	      last(0)
{
}

Head::~Head () { Clear(); }

Link* Head::Suc () const
{
    return first;
}

Link* Head::Pred () const
{
    return last;
}

void Head::AddFirst (Link* element)
{
    if (!element)
	return;
    
    if (!first)
    {
	if (element->inList())
	    element->Out();
	
	first = element;
	last = element;
	element->theList = this;
    }
    else
    {
	element->Precede(first);
	first = element;
    }
}

void Head::AddLast (Link* element)
{
    if (!element)
	return;

    if (last)
    {
	element->Follow(last);
	last = element;
    }
    else
    {
	if (element->inList())
	    element->Out();
	
	first = element;
	last = element;
	element->theList = this;
    }
}

long Head::Cardinal () const 
{
    long numberOfElements = 0;
    Link *tempPtr = first;
    
    while (tempPtr)
    {
	numberOfElements++;
	tempPtr = tempPtr->Suc();
    }
    
    return numberOfElements;
}

void Head::Clear ()
{
    Link *tempPtr = first, *marker = 0;
    
    while (tempPtr)
    {
	marker = tempPtr;
	tempPtr = tempPtr->Suc();
	delete marker;
    }

    first = last = 0;
}


#ifdef NO_INLINES
#  define HEAD_CC_
#  include <SimSet/Head.n>
#  undef HEAD_CC_
#endif

