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

#include <math.h>
#include <iostream.h>

#ifndef PARETO_H_
#  include <Stat/Pareto.h>
#endif

Pareto::Pareto (double gamma, double k)
	       : _gamma(gamma),
		 _k(k)
{
    _kToGamma = pow(_k, _gamma);
}

double Pareto::pdf (double x)
{
    if (x < _k)
    {
	cerr
		     << "Pareto::pdf - invalid value for x." << endl;
	return 0;
    }
    
    return (double) (_kToGamma / pow(x, _gamma+1));
}

double Pareto::cdf (double x)
{
    if (x < _k)
    {
	cerr
		     << "Pareto::cdf - invalid value for x." << endl;
	return 0;
    }

    return (double) (1 - pow(_k/x, _gamma));
}
