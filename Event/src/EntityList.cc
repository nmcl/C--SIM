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
 *
 * $Id: EntityList.cc,v 1.3 1996/12/09 10:10:00 nmcl Exp $ 
 */

#ifndef ENTITY_H_
#  include <Event/Entity.h>
#endif

#ifndef LISTELEMENT_H_
#  include <ListElement.h>
#endif

#ifndef ENTITYLIST_H_
#  include <EntityList.h>
#endif

EntityList::EntityList ()
		       : _headOfList(0),
			 _tailOfList(0),
			 _number(0)
{
}

EntityList::~EntityList ()
{
    if (_number > 0)
    {
	ListElement* ptr = _headOfList;
	ListElement* trail = ptr;

	while (ptr)
	{
	    ptr = ptr->_next;
	    delete trail;
	    trail = ptr;
	}
    }
}

long EntityList::numberOfElements () const
{
    return _number;
}

void EntityList::add (Entity* toAdd)
{
    ListElement* element = new ListElement(toAdd);

    if (!_headOfList)
    {
	_headOfList = element;
	_tailOfList = element;
    }
    else
    {
	_tailOfList->_next = element;
	_tailOfList = element;
    }

    _number++;
}

Entity* EntityList::removeFirst ()
{
    if (_number == 0)
	return (Entity*) 0;

    Entity* toReturn = _headOfList->_element;
    ListElement* ptr = _headOfList;

    if (_tailOfList == _headOfList)
	_tailOfList = _headOfList = (ListElement*) 0;
    else
	_headOfList = _headOfList->_next;

    delete ptr;
    _number--;
    
    return toReturn;
}

Boolean EntityList::remove (Entity* toRemove)
{
    ListElement* ptr = _headOfList;
    ListElement* trail = ptr;

    while (ptr)
    {
	if (ptr->_element != toRemove)
	{
	    trail = ptr;
	    ptr = ptr->_next;
	}
	else
	{
	    if (ptr == _headOfList)
	    {
		if (_tailOfList == _headOfList)
		    _tailOfList = _headOfList->_next;
		
		_headOfList = _headOfList->_next;
	    }
	    else
	    {
		trail->_next = ptr->_next;

		if (ptr == _tailOfList)
		    _tailOfList = trail;
	    }

	    delete ptr;
	    _number--;

	    return TRUE;
	}
    }

    return FALSE;
}

Boolean EntityList::find (Entity* toFind) const
{
    Boolean found = FALSE;
    ListElement* ptr = _headOfList;

    while ((ptr) && (!found))
    {
	if (ptr->_element == toFind)
	    found = TRUE;
	else
	    ptr = ptr->_next;
    }

    return found;
}

