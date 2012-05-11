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


#ifndef HEAPELEMENT_H_
#  include "HeapElement.h"
#endif

#ifndef PROCESSLIST_H_
#  include "ProcessList.h"
#endif


HeapElement::HeapElement (Process& p, HeapElement* n)
                                                    : next(n),
						      rangeTime(0)
{
    proc = new ProcessList;
    proc->Insert(p);
}

HeapElement::~HeapElement ()
{
    if (proc)
        delete proc;
}


#ifdef NO_INLINES
#  define HEAPELEMENT_CC_
#  include "HeapElement.n"
#  undef HEAPELEMENT_CC_
#endif
