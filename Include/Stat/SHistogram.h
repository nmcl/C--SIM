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

#ifndef SHISTOGRAM_H_
#define SHISTOGRAM_H_

#ifndef PHISTOGRAM_H_
#  include <Stat/PHistogram.h>
#endif


class SimpleHistogram : public PrecisionHistogram
{
public:
    SimpleHistogram (double min, double max, long nbuckets);
    SimpleHistogram (double min, double max, double w);
    virtual ~SimpleHistogram ();
    
    virtual void setValue (double);
    virtual void operator+= (double);

    virtual void reset ();

    virtual Boolean sizeByName (double name, double& size);
    double  Width () const;

    virtual ostream& print (ostream&) const;
    
private:
    double minIndex, maxIndex;
    double width;
    long numberBuckets;

public:
    virtual Boolean saveState (const char*) const;
    virtual Boolean saveState (ofstream&) const;
    
    virtual Boolean restoreState (const char*);
    virtual Boolean restoreState (ifstream&);
};

#include <Stat/SHistogram.n>

#endif
