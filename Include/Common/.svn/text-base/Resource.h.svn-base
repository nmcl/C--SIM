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
 * Copyright (C) 1993-1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Resource.h,v 1.2 1998/08/28 14:19:54 nmcl Exp $
 */

#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <stddef.h>

#ifndef BOOLEAN_H_
#   include <Common/Boolean.h>
#endif


class Resource
{
public:
    static void    ref   (Resource* resource);
    static Boolean unref (Resource* resource);

    void* operator new    (size_t amount);
    void  operator delete (void* memPtr);

protected:
    Resource ();
    virtual ~Resource ();

private:
    unsigned int   _referenceCount;
    Boolean        heapAllocated;

    static Boolean newCalled;
};

#endif
