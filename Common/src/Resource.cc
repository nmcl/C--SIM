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
 * Copyright (C) 1993,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Resource.cc,v 1.1 1995/02/28 13:26:10 nmcl Exp $
 */

#include <iostream>

using std::cerr;
using std::cout;
using std::endl;
using std::ostream;
using std::istream;

#ifndef RESOURCE_H_
#   include <Common/Resource.h>
#endif

Boolean Resource::newCalled = FALSE;


Resource::Resource ()
                   : _referenceCount(0),
                     heapAllocated(newCalled)
{
    Resource::newCalled = FALSE;
}

Resource::~Resource ()
{
    if ((_referenceCount != 0) && (heapAllocated))
        cerr << "Resource::~Resource : _referenceCount = "
	     << _referenceCount << endl;
}

/*
 * If allocated on the stack then we could simply ignore all
 * ref and unref requests. Currently we process them but
 * treat "error" conditions differently.
 */

void Resource::ref (Resource* resource)
{
    if (resource != NULL)
        resource->_referenceCount++;
}

Boolean Resource::unref (Resource* resource)
{
    if (resource != (Resource*) 0)
    {
        if (resource->_referenceCount > 0)
            resource->_referenceCount--;
        else
	    if (resource->heapAllocated)
	        cerr << "Resource::unref : _referenceCount == 0" << endl;

        if ((resource->_referenceCount == 0) && (resource->heapAllocated))
        {
            delete resource;
            resource = (Resource*) 0;
        }
    }

    if ((resource != (Resource*) 0) && (resource->heapAllocated))
        return FALSE;
    else
        return TRUE;
}

void* Resource::operator new (size_t amount)
{
    Resource::newCalled = TRUE;
    return ::new char[amount];
}

void Resource::operator delete (void* memPtr)
{
    ::delete [] memPtr;
}
