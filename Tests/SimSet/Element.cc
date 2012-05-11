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


#ifndef ELEMENT_H_
#  include "Element.h"
#endif

#ifndef SET_H_
#  include "Set.h"
#endif


Element::Element ()
                  : value(0)
{
}

Element::Element (long x)
                        : value(x)
{
}

Element::~Element ()
{
}

Boolean Element::Belongs (Set* toCheck) const
{
    Boolean found = FALSE;
    Element* temp = (Element*) toCheck->First();
    
    while ((temp) && (!found))
    {
	if (temp->GetValue() == value)
	    found = TRUE;
	else
	    temp = (Element*) temp->Suc();
    }
    
    return found;
}


#ifdef NO_INLINES
#  define ELEMENT_CC_
#  include "Element.n"
#  undef ELEMENT_CC_
#endif
