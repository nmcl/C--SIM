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
#include <fstream>
#include <iomanip>

using std::cerr;
using std::cout;
using std::endl;
using std::ios;
using std::ostream;
using std::istream;
using std::ofstream;
using std::ifstream;

#ifndef PROCESS_H_
#  include <ClassLib/Process.h>
#endif

#ifndef TIMEVARIANCE_H_
#  include <Stat/TimeVariance.h>
#endif


TimeVariance::TimeVariance () { TimeVariance::reset(); }

TimeVariance::~TimeVariance () {}

void TimeVariance::reset ()
{
    first = TRUE;
    startTime = currentValue = 0.0;
    stime = total = 0.0;
    Variance::reset();
}

double TimeVariance::timeAverage () const
{
    if (first || (Process::CurrentTime() - startTime) == 0)
        return 0.0;

    return ((total + area())/(Process::CurrentTime() - startTime));
}

void TimeVariance::store (double value)
{
    currentValue = value;
    stime = Process::CurrentTime();
}

void TimeVariance::setValue (double value)
{
    Variance::setValue(value);

    if (!first)
    {
        total += area();
        if (value == currentValue)
	    return;
    }
    else
    {
        first = FALSE;
	startTime = Process::CurrentTime();
    }

    store(value);
}

Boolean TimeVariance::saveState (ofstream& oFile) const
{
    if (!oFile)
	return FALSE;

    oFile << " " << first;
    oFile << " " << startTime << " " << currentValue;
    oFile << " " << stime << " " << total;
    
    return Variance::saveState(oFile);
}

Boolean TimeVariance::saveState (const char* fileName) const
{
    ofstream oFile(fileName, ios::out);
    Boolean result;
    
    if (!oFile)
    {
	cerr << "TimeVariance::saveState - error " << errno
		     << " for file " << fileName << endl;
	return FALSE;
    }
    
    result = saveState(oFile);
    oFile.close();
    return result;
}

Boolean TimeVariance::restoreState (ifstream& iFile)
{
    if (!iFile)
	return FALSE;
    
    iFile >> first;
    iFile >> startTime >> currentValue;
    iFile >> stime, total;
    
    return TRUE;
}

Boolean TimeVariance::restoreState (const char* fileName)
{
    ifstream iFile(fileName, ios::in);
    Boolean result;
    
    if (!iFile)
    {
	cerr << "TimeVariance::restoreState - error " << errno
		     << " for file " << fileName << endl;
	return FALSE;
    }
    
    result = restoreState(iFile);
    iFile.close();
    return result;
}

#ifdef NO_INLINES
#  define TIMEVARIANCE_CC_
#  include <Stat/TimeVariance.n>
#  undef TIMEVARIANCE_CC_
#endif
