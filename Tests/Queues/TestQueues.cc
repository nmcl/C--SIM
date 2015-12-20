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
 *
 * $Id: TestQueues.cc,v 1.9 1997/03/18 13:47:29 nmcl Exp $
 */

#ifndef PROCESS_H_
#  include <ClassLib/Process.h>
#endif

#ifndef TESTER_H_
#  include "Tester.h"
#endif

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/time.h>
#include <unistd.h>

using std::cerr;
using std::cout;
using std::endl;
using std::ostream;
using std::istream;

#ifdef SYSV
#  include <sys/times.h>
#  include <limits.h>
#endif

#ifdef __GNUG__
extern "C"
{
    int gettimeofday (struct timeval*, struct timezone*);
}
#endif

#ifdef SYSV
const double clockTick = (1.0/sysconf(_SC_CLK_TCK));
#endif

int main (int argc, char** argv)
{
    Tester::StreamType st = Tester::UNIFORM;
    long numberOfElements = 100;

#ifdef SYSV
    clock_t currentTime = 0, startTime = 0;
    struct tms ClockBuffer;
#else
    unsigned long start_ms, finish_ms;
    struct timeval stime, ftime;
#endif

    for (int i = 0; i < argc; i++)
    {
        if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "usage: TestQueues -stream [u, n, e] -number" << endl;
	    return 0;
	}
	if (::strcmp(argv[i], "-number") == 0)
	    numberOfElements = atol(argv[i+1]);
	if (::strcmp(argv[i], "-stream") == 0)
	{
	    char opt = argv[i+1][0];
	    switch(opt)
	    {
	    case 'u':
	        st = Tester::UNIFORM;
		break;
	    case 'n':
		st = Tester::NORMAL;
		break;
	    case 'e':
		st = Tester::EXPONENTIAL;
		break;
	    default:
		cout << "Error: Unknown stream type " << opt << endl;
		return -1;
	    }
	}
    }

    Thread::Initialize();
   
    Tester t(numberOfElements, st);

#ifdef SYSV
    startTime = times(&ClockBuffer);
    double convertTime = 0.0;
#else
    gettimeofday(&stime, 0);
    start_ms = stime.tv_sec*1000000 + stime.tv_usec;
#endif

    t.Await();
    
    t.tidy();
    
#ifdef SYSV
    currentTime = times(&ClockBuffer) - startTime;
    convertTime = currentTime * clockTick * 1000000;
#else
    gettimeofday(&ftime, 0);
    finish_ms = ftime.tv_sec * 1000000 + ftime.tv_usec;
#endif

    cout << "Time taken to add and remove " << numberOfElements << " list elements was ";

#ifdef SYSV
    cout << (long) (convertTime/1000);
#else
    cout << (long) ((finish_ms - start_ms)/1000);
#endif

    cout << " milliseconds" << endl;

    t.Exit();
    return 0;
}
