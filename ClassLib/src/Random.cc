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
 * Copyright (C) 1994-1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Random.cc,v 1.7 1998/08/28 14:19:39 nmcl Exp $
 */

#include <iostream.h>
#include <math.h>

#ifndef RANDOM_H_
#  include <ClassLib/Random.h>
#endif

static Boolean _initialized = FALSE;
static double defaultSeries[128];
long startMSeed = 0;
long startLSeed = 0;

RandomStream::RandomStream (long MGSeed, long LCGSeed)
{
    // Clean up input parameters
    
    if ((MGSeed&1) == 0) MGSeed--;
    if (MGSeed<0) MGSeed = -MGSeed;
    if (LCGSeed<0) LCGSeed = -LCGSeed;

    // Initialise state
    
    MSeed = MGSeed;
    LSeed = LCGSeed;

    /*
     * For the default seeds only calculate these values once. Saves time later.
     */

    Boolean defaultSeeds = TRUE;
    
    if ((MGSeed != 772531L) || (LCGSeed != 1878892440L))
	defaultSeeds = FALSE;

    for (int i = 0; i < (sizeof(series)/sizeof(double)); i++)
    {
	if (defaultSeeds)
	{
	    if (!_initialized)
	    {
		series[i] = MGen();
		defaultSeries[i] = series[i];
	    }
	    else
		series[i] = defaultSeries[i];
	}
	else
	    series[i] = MGen();	    
    }

    if (defaultSeeds)
    {
	if (!_initialized)
	{
	    startMSeed = MSeed;
	    startLSeed = LSeed;
	
	    _initialized = TRUE;
	}
	else
	{
	    MSeed = startMSeed;
	    LSeed = startLSeed;
	}
    }
}

void RandomStream::copy (const RandomStream& toCopy)
{
    if (this == &toCopy)
	return;

    MSeed = toCopy.MSeed;
    LSeed = toCopy.LSeed;

    for (int i = 0; i < 128; i++)
	series[i] = toCopy.series[i];
}

UniformStream::UniformStream (double l, double h, int StreamSelect,
			      long MGSeed, long LCGSeed)
			     : RandomStream(MGSeed, LCGSeed)
{
    lo = l;
    hi = h;
    range = hi-lo;
    for (int ss=0; ss<StreamSelect*1000; ss++) (void) Uniform();
}

UniformStream& UniformStream::operator= (const UniformStream& toCopy)
{
    if (this == &toCopy)
	return *this;

    lo = toCopy.lo;
    hi = toCopy.hi;
    range = toCopy.range;

    RandomStream::copy(toCopy);

    return *this;
}

Draw::Draw (double p, int StreamSelect, long MGSeed, long LCGSeed)
	   : s(0, 1, StreamSelect, MGSeed, LCGSeed),
	     prob(p)
{
}

Draw& Draw::operator= (const Draw& toCopy)
{
    if (this == &toCopy)
	return *this;

    prob = toCopy.prob;
    s = toCopy.s;

    return *this;
}

ExponentialStream::ExponentialStream (double m, int StreamSelect,
				      long MGSeed, long LCGSeed)
				     : RandomStream(MGSeed, LCGSeed)
{
    Mean = m;
    for (int ss=0; ss<StreamSelect*1000; ss++) (void) Uniform();
}

ExponentialStream& ExponentialStream::operator= (const ExponentialStream& toCopy)
{
    if (this == &toCopy)
	return *this;

    Mean = toCopy.Mean;

    RandomStream::copy(toCopy);

    return *this;
}

ErlangStream::ErlangStream (double m, double s, int StreamSelect,
			    long MGSeed, long LCGSeed)
			   : RandomStream(MGSeed, LCGSeed)
{
    Mean = m;
    StandardDeviation = s;

    double z = Mean/StandardDeviation;
    k = (long) (z*z);
    for (int ss=0; ss<StreamSelect*1000; ss++) (void) Uniform();
}

ErlangStream& ErlangStream::operator= (const ErlangStream& toCopy)
{
    if (this == &toCopy)
	return *this;

    Mean = toCopy.Mean;
    StandardDeviation = toCopy.StandardDeviation;
    k = toCopy.k;

    RandomStream::copy(toCopy);

    return *this;
}

HyperExponentialStream::HyperExponentialStream (double m, double s,
						int StreamSelect,
						long MGSeed, long LCGSeed)
					       : RandomStream(MGSeed, LCGSeed)
{
    Mean = m;
    StandardDeviation = s;
    double cv,z;
    cv=StandardDeviation/Mean;
    z = cv*cv;
    p = 0.5*(1.0-sqrt((z-1.0)/(z+1.0)));
    for (int ss=0; ss<StreamSelect*1000; ss++) (void) Uniform();
}

HyperExponentialStream& HyperExponentialStream::operator= (const HyperExponentialStream& toCopy)
{
    if (this == &toCopy)
	return *this;

    Mean = toCopy.Mean;
    StandardDeviation = toCopy.StandardDeviation;
    p = toCopy.p;

    RandomStream::copy(toCopy);

    return *this;
}

NormalStream::NormalStream (double m, double s, int StreamSelect,
			    long MGSeed, long LCGSeed)
			   : RandomStream(MGSeed, LCGSeed)
{
    Mean = m;
    StandardDeviation = s;
    z = 0.0;
    for (int ss=0; ss<StreamSelect*1000; ss++) (void) Uniform();
}

NormalStream& NormalStream::operator= (const NormalStream& toCopy)
{
    if (this == &toCopy)
	return *this;

    Mean = toCopy.Mean;
    StandardDeviation = toCopy.StandardDeviation;
    z = toCopy.z;

    RandomStream::copy(toCopy);

    return *this;
}

#ifdef NO_INLINES
#  define RANDOM_CC_
#  include <ClassLib/Random.n>
#  undef RANDOM_CC_
#endif
