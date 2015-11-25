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

#include <errno.h>
#include <iostream>
#include <fstream.h>
#include <iomanip.h>
#include <limits>
#include <math.h>

using namespace std;

#ifndef MEAN_H_
#  include <Stat/Mean.h>
#endif

Mean::Mean () { Mean::reset(); }

Mean::~Mean () {}

void Mean::reset ()
{
  //    _Max = -MAXFLOAT;
  //    _Min = MAXFLOAT;
  _Max = numeric_limits<float>::max();
  _Min = numeric_limits<float>::min();
    _Sum = _Mean = 0.0;
    _Number = 0;
}

void Mean::setValue (double value)
{
    if (value > _Max)
        _Max = value;
    if (value < _Min)
        _Min = value;
    _Sum += value;
    _Number++;
    _Mean = (double) (_Sum/_Number);
}

Boolean Mean::saveState (ofstream& oFile) const
{
    if (!oFile)
	return FALSE;
    
    oFile << " " << _Max << " " << _Min;
    oFile << " " << _Sum << " " << _Mean;
    oFile << " " << _Number << " ";
    return TRUE;
}

Boolean Mean::saveState (const char* fileName) const
{
    ofstream oFile(fileName, ios::out);
    Boolean result;
    
    if (!oFile)
    {
	cerr << "Mean::saveState - error " << errno << " for file " << fileName << endl;
	return FALSE;
    }
    
    result = saveState(oFile);
    oFile.close();
    return result;
}

Boolean Mean::restoreState (ifstream& iFile)
{
    if (!iFile)
	return FALSE;
    
    iFile >> _Max >> _Min;
    iFile >> _Sum >> _Mean;
    iFile >> _Number;
    return TRUE;
}

Boolean Mean::restoreState (const char* fileName)
{
    ifstream iFile(fileName, ios::in);
    Boolean result;
    
    if (!iFile)
    {
	cerr << "Mean::restoreState - error " << errno << " for file " << fileName << endl;
	return FALSE;
    }
    
    result = restoreState(iFile);
    iFile.close();
    return result;
}

ostream& Mean::print (ostream& strm) const
{
    strm << "Number of samples : " << numberOfSamples() << endl;
    strm << "Minimum           : " << min() << endl;
    strm << "Maximum           : " << max() << endl;
    strm << "Sum               : " << sum() << endl;
    strm << "Mean              : " << mean() << endl;

    return strm;
}

#ifdef NO_INLINES
#  define MEAN_CC_
#  include <Stat/Mean.n>
#  undef MEAN_CC_
#endif
