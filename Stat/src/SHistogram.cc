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
 * Copyright (C) 1994-1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SHistogram.cc,v 1.10 1997/07/31 11:13:25 nmcl Exp $
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

#ifndef SHISTOGRAM_H_
#  include <Stat/SHistogram.h>
#endif

// try to ensure parameters make sense

SimpleHistogram::SimpleHistogram (double min, double max, long nbuckets)
                                 : minIndex((min < max) ? min : max),
                                   maxIndex((min < max) ? max : min),
                                   numberBuckets((nbuckets) ? nbuckets : 1)
{
    width = (max - min)/numberBuckets;
    SimpleHistogram::reset();
}

SimpleHistogram::SimpleHistogram (double min, double max, double w)
                                 : minIndex((min < max) ? min : max),
                                   maxIndex((min < max) ? max : min),
                                   width((w) ? w : 2.0)
{
    numberBuckets = (long) ((max - min)/width);
    numberBuckets = ((double) ((max - min)/width) - numberBuckets) ? numberBuckets++ : numberBuckets;
    SimpleHistogram::reset();
}

SimpleHistogram::~SimpleHistogram () {}

void SimpleHistogram::reset ()
{
    double value = minIndex;

    PrecisionHistogram::reset();

    // pre-create buckets with given width

    for (int i = 0; i < numberBuckets; value+=width, i++)
        PrecisionHistogram::create(value);
}

Boolean SimpleHistogram::sizeByName (double name, double& size)
{
    if ((name < minIndex) || (name > maxIndex))
	return FALSE;

    for (Bucket* ptr = Head; ptr; ptr = ptr->cdr())
    {
        double bucketValue = (*ptr)();

	if ((name == bucketValue) || (name <= bucketValue + width))
	{
	    size = ptr->size();
	    return TRUE;
	}
    }

    return FALSE;
}
    
void SimpleHistogram::setValue (double value)
{
    if ((value < minIndex) || (value > maxIndex))
    {
        cerr << "Value " << value
		     << " is beyond histogram range "
		     << "[ " << minIndex << ", "
		     << maxIndex << " ]" << endl;
	return;
    }

    for (Bucket* ptr = Head; ptr; ptr = ptr->cdr())
    {
        double bucketValue = (*ptr)();

	if ((value == bucketValue) || (value <= bucketValue + width))
	{
	    PrecisionHistogram::setValue((*ptr)());
	    return;
	}
    }

    // shouldn't get here!!

    cerr
		 << "SimpleHistogram::setValue - Something went wrong with "
		 << value << endl;
}

ostream& SimpleHistogram::print (ostream& strm) const
{
    strm << "Maximum index range  : " << maxIndex << "\n";
    strm << "Minimum index range  : " << minIndex << "\n";
    strm << "Number of buckets    : " << numberBuckets << "\n";
    strm << "width of each bucket : " << width << "\n";

    return PrecisionHistogram::print(strm);
}

Boolean SimpleHistogram::saveState (ofstream& oFile) const
{
    if (!oFile)
	return FALSE;

    oFile << " " << minIndex << " " << maxIndex;
    oFile << " " << width << " " << numberBuckets;
    
    return PrecisionHistogram::saveState(oFile);
}

Boolean SimpleHistogram::saveState (const char* fileName) const
{
    ofstream oFile(fileName, ios::out);
    Boolean result;
    
    if (!oFile)
    {
	cerr << "SimpleHistogram::saveState - error "
		     << errno << " for file " << fileName << endl;
	return FALSE;
    }
    
    result = saveState(oFile);
    oFile.close();
    return result;
}

Boolean SimpleHistogram::restoreState (ifstream& iFile)
{
    if (!iFile)
	return FALSE;
    
    iFile >> minIndex >> maxIndex;
    iFile >> width >> numberBuckets;

    return PrecisionHistogram::restoreState(iFile);
}

Boolean SimpleHistogram::restoreState (const char* fileName)
{
    ifstream iFile(fileName, ios::in);
    Boolean result;
    
    if (!iFile)
    {
	cerr << "SimpleHistogram::restoreState - error "
		     << errno << " for file " << fileName << endl;
	return FALSE;
    }
    
    result = restoreState(iFile);
    iFile.close();
    return result;
}

#ifdef NO_INLINES
#  define SHISTOGRAM_CC_
#  include <Stat/SHistogram.n>
#  undef SHISTOGRAM_CC_
#endif
