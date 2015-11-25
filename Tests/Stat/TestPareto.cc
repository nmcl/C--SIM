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
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <string.h>
#include <iostream>
#include <stdlib.h>

#ifndef PARETO_H_
#  include <Stat/Pareto.h>
#endif

int main (int argc, char** argv)
{
    double k = 1.0;
    double gamma = 1.0;
    
    for (int i = 0; i < argc; i++)
    {
        if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << " [-gamma <value>] [-k <value>] [-help]" << endl;
	    return 0;
	}
	if (::strcmp(argv[i], "-k") == 0)
	    k = atof(argv[i+1]);
	if (::strcmp(argv[i], "-gamma") == 0)
	    gamma = atof(argv[i+1]);
    }

    Pareto p(gamma, k);

    for (int j = 1; j < 10; j++)
	cout << j << " " << p.cdf(j) << endl;

    return 0;
}
