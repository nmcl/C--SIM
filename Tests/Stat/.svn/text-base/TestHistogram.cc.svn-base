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


#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef PHISTOGRAM_H_
#  include <Stat/PHistogram.h>
#endif

#ifndef HISTOGRAM_H_
#  include <Stat/Histogram.h>
#endif

#ifndef SHISTOGRAM_H_
#  include <Stat/SHistogram.h>
#endif

#ifndef QUANTILE_H_
#  include <Stat/Quantile.h>
#endif

#ifndef RANDOM_H_
#  include <ClassLib/Random.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <iostream.h>

enum HistType { PRECISION, NORMAL, SIMPLE, QUANTILE };

PrecisionHistogram* phist = (PrecisionHistogram*) 0;
Histogram* hist = (Histogram*) 0;
SimpleHistogram* shist = (SimpleHistogram*) 0;
Quantile *qhist = (Quantile*) 0;


int main (int argc, char** argv)
{
    HistType type = NORMAL;
    long numberOfIterations = 100;
    char* loadFileName = (char*) 0;
    char* saveFileName = (char*) 0;
    
    for (int i = 0; i < argc; i++)
    {
        if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "usage: TestHistogram -type <Precision/Normal/Simple/Quantile> [-number <iterations>] [-loadfile <datafile>] [-savefile <datafile>]" << endl;
	    return 0;
	}
	if (::strcmp(argv[i], "-type") == 0)
	{
	    if (::strcmp(argv[i+1], "Precision") == 0)
	        type = PRECISION;
	    if (::strcmp(argv[i+1], "Simple") == 0)
	        type = SIMPLE;
	    if (::strcmp(argv[i+1], "Quantile") == 0)
	        type = QUANTILE;
	}
	if (::strcmp(argv[i], "-number") == 0)
	    numberOfIterations = atol(argv[i+1]);
	if (::strcmp(argv[i], "-loadfile") == 0)
	    loadFileName = argv[i+1];
	if (::strcmp(argv[i], "-savefile") == 0)
	    saveFileName = argv[i+1];
    }

    ExponentialStream* eStream = new ExponentialStream(10);
    
    switch (type)
    {
    case PRECISION:
        phist = new PrecisionHistogram;
	if (loadFileName)
	    phist->restoreState(loadFileName);
	break;
    case SIMPLE:
	shist = new SimpleHistogram(0.0, 55.0, (long) 20);
	if (loadFileName)
	    shist->restoreState(loadFileName);	
	break;
    case QUANTILE:
	qhist = new Quantile(0.95);
	if (loadFileName)
	    qhist->restoreState(loadFileName);	
	break;
    default:
        hist = new Histogram(20);
	if (loadFileName)
	    hist->restoreState(loadFileName);	
	break;
    }

    for (int j = 0; j < numberOfIterations; j++)
    {
        switch (type)
	{
	case PRECISION:
	    (*phist) += (*eStream)();
	    break;
	case SIMPLE:
	    (*shist) += (*eStream)();
	    break;
	case QUANTILE:
	    (*qhist) += (*eStream)();
	    break;
	default:
	    (*hist) += (*eStream)();
	    break;
	}
    }

    cout << "Histogram data is:" << endl;
    switch (type)
    {
    case PRECISION:
        cout << *phist << endl;      
	break;
    case SIMPLE:
	cout << *shist << endl;
	break;
    case QUANTILE:
	cout << *qhist << endl;
	break;
    default:
        cout << *hist << endl;
	break;
    }

    if (phist)
    {
	if (saveFileName)
	    phist->saveState(saveFileName);
        delete phist;
    }
    if (shist)
    {
	if (saveFileName)
	    shist->saveState(saveFileName);	
        delete shist;
    }
    if (hist)
    {
	if (saveFileName)
	    hist->saveState(saveFileName);
        delete hist;
    }
    if (qhist)
    {
	if (saveFileName)
	    qhist->saveState(saveFileName);	
        delete qhist;
    }

    delete eStream;

    return 1;
}
