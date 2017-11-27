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
 * $Id: Random.h,v 1.8 1998/10/01 08:20:53 nmcl Exp $
 */

#ifndef RANDOM_H_
#define RANDOM_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  This file contains the interfaces for five different (pseudo-) random    //
//  number  generators:                                                      //
//                                                                           //
//  1) Uniform -          returns a  number drawn from a uniform distribution//
//                        with the given lower and upper bounds.             //
//                                                                           //
//  2) Exponential -      returns a number from an exponential distribution  //
//                        with the given mean                                //
//                                                                           //
//  3) Erlang -           returns a number from an Erlang distribution with  //
//                        the given mean and standard deviation              //
//                                                                           //
//  4) HyperExponential - returns a number from a hyperexpontial distribution//
//                        with the given mean and standard deviation         //
//                                                                           //
//  5) Normal -           returns a number from a normal distribution with   //
//                        the given mean and standard deviation.             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// The class RandomStream is an abstract base class from which the other     //
// distribution classes are derived.                                         //
///////////////////////////////////////////////////////////////////////////////

class RandomStream
{
public:
    virtual double operator() () = 0;
    double Error ();	// returns a chi-square error measure on the uniform
			// distribution function

    virtual ~RandomStream ();
    
protected:
    RandomStream (long MGSeed=772531L, long LCGSeed=1878892440L);

    double Uniform ();

    void copy (const RandomStream&);
    
private:
    double MGen ();
    
    double series[128];
    long MSeed, LSeed;
};

class UniformStream : public RandomStream
{
public:
    UniformStream (double lo, double hi, int StreamSelect=0,
		   long MGSeed=772531L, long LCGSeed=1878892440L);
    virtual double operator() ();

    virtual ~UniformStream ();

    UniformStream& operator= (const UniformStream& toCopy);
    
private:
    double lo, hi;
    double range;
};

class Draw
{
public:
    Draw (double p, int StreamSelect=0,
	  long MGSeed=772531L, long LCGSeed=1878892440L);
    virtual Boolean operator() ();

    Draw& operator= (const Draw& toCopy);
    
    virtual ~Draw ();

private:
    UniformStream s;
    double prob;
};

class ExponentialStream : public RandomStream
{
public:
    ExponentialStream (double Mean, int StreamSelect=0,
		       long MGSeed=772531L, long LCGSeed=1878892440L);
    virtual double operator() ();

    virtual ~ExponentialStream ();

    ExponentialStream& operator= (const ExponentialStream& toCopy);
    
private:
    double Mean;
};

class ErlangStream : public RandomStream
{
public:
    ErlangStream (double Mean, double StandardDeviation, int StreamSelect=0,
		  long MGSeed=772531L, long LCGSeed=1878892440L);
    virtual double operator() ();

    virtual ~ErlangStream ();

    ErlangStream& operator= (const ErlangStream& toCopy);
    
private:
    double Mean, StandardDeviation;
    long k;
};

class HyperExponentialStream : public RandomStream
{
public:
    HyperExponentialStream (double Mean, double StandardDeviation,
			    int StreamSelect=0,
			    long MGSeed=772531L,long LCGSeed=1878892440L);
    virtual double operator() ();

    virtual ~HyperExponentialStream ();

    HyperExponentialStream& operator= (const HyperExponentialStream& toCopy);
    
private:
    double Mean, StandardDeviation;
    double p;
};

class NormalStream : public RandomStream
{
public:
    NormalStream (double Mean, double StandardDeviation, int StreamSelect=0,
		  long MGSeed=772531L,long LCGSeed=1878892440L);
    virtual double operator() ();

    virtual ~NormalStream ();

    NormalStream& operator= (const NormalStream& toCopy);
    
private:
    double Mean, StandardDeviation;
    double z;
};

/**
 * Returns a number drawn from a triangular distribution with lower limit a, upper limit b and mode c, where a < b and a ≤ c ≤ b.
 */

class TriangularStream : public RandomStream
{
public:
  TriangularStream (double a, double b, double c, int StreamSelect = 0, long MGSeed=772531L, long LCGSeed=1878892440L);

  virtual double operator() ();

  virtual ~TriangularStream ();

  TriangularStream& operator= (const TriangularStream& toCopy);

private:
  double a, b, c;
};

#include <ClassLib/Random.n>

#endif // RANDOM_H

