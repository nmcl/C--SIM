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

#ifndef PHISTOGRAM_H_
#define PHISTOGRAM_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef VARIANCE_H_
#  include <Stat/Variance.h>
#endif

#include <iostream.h>


class Bucket
{
    friend class PrecisionHistogram;
    friend class Histogram;
    
public:
    Bucket (double, long = 1);
    Bucket (const Bucket&);
    ~Bucket ();

    // some useful operators

    Boolean operator== (double) const;
    Boolean operator> (double) const;
    Boolean operator>= (double) const;
    Boolean operator< (double) const;
    Boolean operator<= (double) const;

    double operator() () const;
    void   operator+= (long);

    long   size () const;
    double Name () const;

    Bucket* cdr () const;
    void    setCdr (Bucket*);
    
private:
    long   numberOfEntries;
    double name;
    
    Bucket* next;
};


/*
 * This is the base histogram class which keeps an exact tally of
 * all values input, i.e., a bucket is created for each new value.
 * This can take up a lot of space in a given simulation, so other
 * (less precise) histogram classes are also provided.
 */

class PrecisionHistogram : public Variance
{
public:
    PrecisionHistogram ();
    virtual ~PrecisionHistogram ();
    
    virtual void setValue (double);
    virtual void operator+= (double);

    virtual void reset ();

    long numberOfBuckets () const;

    /*
     * There are two ways of getting the number of entries in a bucket:
     * (i) give the index number of the bucket, or
     * (ii) give the name of the bucket.
     * If the bucket is not present then FALSE is returned.
     */

    virtual Boolean sizeByIndex (long index, double& size);
    virtual Boolean sizeByName  (double name, double& size);

    virtual Boolean bucketName (long index, double& name);
    
    virtual ostream& print (ostream&) const;

protected:
    virtual void add (Bucket*, double, Boolean = FALSE);
    virtual void create (double);

    Boolean isPresent (double);
    
    long length;
    Bucket* Head;

public:
    virtual Boolean saveState (const char*) const;
    virtual Boolean saveState (ofstream&) const;
    
    virtual Boolean restoreState (const char*);
    virtual Boolean restoreState (ifstream&);
};

#include <Stat/PHistogram.n>

#endif
