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

#ifndef SET_H_
#  include "Set.h"
#endif

#ifndef ELEMENT_H_
#  include "Element.h"
#endif

#include <iostream>

using std::cerr;
using std::cout;
using std::endl;
using std::ostream;
using std::istream;


int main ()
{
    Set S1, S2;
    Set *S3 = 0;
    Element* E = (Element*) 0;
    
    for (int i = 0; i < 10; i++)
    {
	E = new Element(i);
	E->InTo(&S1);
    }
    
    for (int j = 8; j < 14; j++)
    {
	E = new Element(j);
	E->InTo(&S2);
    }

    S3 = S1.Intersect(&S2);
    
    E = (Element*) S3->First();
    
    cout << "Intersection is:" << endl;
    while (E)
    {
	cout << "value: " << E->GetValue() << endl;
	E = (Element*) E->Suc();
    }

    delete S3;
    
    return 0;
}
