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
 * $Id: Tester.cc,v 1.3 1997/03/18 13:47:30 nmcl Exp $
 */

#include <iostream>

#ifndef RESOURCE_H_
#  include <Common/Resource.h>
#endif

#ifndef TESTER_H_
#  include "Tester.h"
#endif

#ifndef DUMMYPROCESS_H_
#  include "DummyProcess.h"
#endif

long numberOfJobs = 0;

Tester::Tester (long processes, long iter, int hold)
               : number(processes),
		 iterations(iter),
		 justHold(hold),
		 dataStream(0.1, 10000.0),
                 head(0)
{
    if(justHold) {
	for (int i = 0; i < number; i++)
	{
	    DummyProcess *dp = new DummyProcess(dataStream());
	    dp->next = head;
	    head = dp;
    
	    dp->ActivateAt(dataStream());
	}
    }
}

Tester::~Tester ()
{
    if (head)
	delete head;
}

void Tester::Body ()
{

    if(!justHold) {
	for (int i = 0; i < number; i++)
	{
	    DummyProcess *dp = new DummyProcess(dataStream());
	    dp->next = head;
	    head = dp;
    
	    dp->ActivateAt(dataStream());
	}
    }

    Scheduler::scheduler().Resume();

    while (numberOfJobs < iterations)
	Hold(10000);

    Scheduler::scheduler().Suspend();
    
    Thread::mainResume();
}

void Tester::Await ()
{
    Resume();
    Thread::Self()->Suspend();
}

void Tester::Exit ()
{
    Thread::Exit();
}

