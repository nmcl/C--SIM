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

#ifndef TIMEVARIANCE_H_
#define TIMEVARIANCE_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef VARIANCE_H_
#  include <Stat/Variance.h>
#endif


class TimeVariance : public Variance
{
public:
    TimeVariance ();
    ~TimeVariance ();

    virtual void reset ();

    virtual void setValue (double);
    virtual void operator+= (double);

    double timeAverage () const;

protected:
    double area () const;

    Boolean first;
    double startTime, currentValue;
    double stime, total;

private:
    void store (double);

public:
    virtual Boolean saveState (const char*) const;
    virtual Boolean saveState (ofstream&) const;
    
    virtual Boolean restoreState (const char*);
    virtual Boolean restoreState (ifstream&);
};

#include <Stat/TimeVariance.n>

#endif
