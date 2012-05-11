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

#ifndef QUANTILE_H_
#  include <Stat/Quantile.h>
#endif


// if a bad value is given to constructor then default is used

Quantile::Quantile (double q)
		   : qProb(((q > 0.0) && (q <= 1.0)) ? q : 0.95)
{
    if ((q <= 0.0) || (q > 1.0))
	cerr << "Quantile::Quantile ( " << q << " ) : bad value." << endl;
}

Quantile::~Quantile () {}

double Quantile::operator() () const
{
    double pSamples = numberOfSamples() * qProb;
    long nEntries = 0;
    Bucket *ptr = Head, *trail = (Bucket*) 0;

    if (pSamples == 0.0)
    {
	cerr << "Quantile::operator() : percentage samples error." << endl;
	return 0.0;
    }
    
    while ((nEntries < pSamples) && (ptr))
    {
        nEntries += ptr->size();
        trail = ptr;
	ptr = ptr->cdr();
    }

    return trail->Name();
}

ostream& Quantile::print (ostream& strm) const
{
    strm << "Quantile precentage : " << qProb << "\n";
    strm << "Value below which percentage occurs " << (*this)() << "\n";
    return PrecisionHistogram::print(strm);
}


#ifdef NO_INLINES
#  define QUANTILE_CC_
#  include <Stat/Quantile.n>
#  undef QUANTILE_CC_
#endif
