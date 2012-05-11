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

#ifndef VARIANCE_H_
#define VARIANCE_H_

#ifndef MEAN_H_
#  include <Stat/Mean.h>
#endif


class Variance : public Mean
{
public:
    Variance ();
    virtual ~Variance ();
     
    virtual void setValue (double);
    virtual void operator+= (double);

    virtual void reset ();

    double variance () const;
    double stdDev () const;

    double confidence (double);

    virtual ostream& print (ostream&) const;
    
protected:
    double _sqr;

public:
    virtual Boolean saveState (const char*) const;
    virtual Boolean saveState (ofstream&) const;
    
    virtual Boolean restoreState (const char*);
    virtual Boolean restoreState (ifstream&);
};


#include <Stat/Variance.n>

#endif

