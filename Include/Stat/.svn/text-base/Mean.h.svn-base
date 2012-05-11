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

#ifndef MEAN_H_
#define MEAN_H_

#include <iostream.h>
#include <fstream.h>

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif


class Mean
{
public:
    Mean ();
    virtual ~Mean ();

    virtual void setValue (double);
    virtual void operator+= (double);

    virtual void reset ();

    unsigned int numberOfSamples () const;

    double min () const;
    double max () const;
    double sum () const;
    double mean () const;

    virtual ostream& print (ostream&) const;    
    
protected:
    double _Max, _Min;
    double _Sum, _Mean;
    unsigned int _Number;

public:
    virtual Boolean saveState (const char*) const;
    virtual Boolean saveState (ofstream&) const;
    
    virtual Boolean restoreState (const char*);
    virtual Boolean restoreState (ifstream&);
};

#include <Stat/Mean.n>

#endif
