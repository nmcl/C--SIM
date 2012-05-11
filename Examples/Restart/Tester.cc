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
#include <iostream.h>

#ifndef PROCESS_H_
#  include <ClassLib/Process.h>
#endif

#ifndef TESTER_H_
#  include "Tester.h"
#endif

#ifndef HARNESS_H_
#  include "Harness.h"
#endif


const int iter = 4;


Tester::Tester ()
	       : dummy(0)
{
}

Tester::~Tester ()
{
#ifndef NO_RESOURCE
    Resource::unref(dummy);
#else    
    if (dummy)
	delete dummy;
#endif    
}

void Tester::Body ()
{
    dummy = new Harness;

#ifndef NO_RESOURCE
    Resource::ref(dummy);
#endif    

    for (int i = 0; i < iter; i++)
    {
	cout << "Iteration " << i << endl;

	Scheduler::scheduler().Resume();
	dummy->Activate();

	cout << "Tester holding." << endl;
	Hold(100);

	cout << "Tester woken." << endl;

	Scheduler::scheduler().reset();
    }

    dummy->doPassivate();
    dummy->Activate();
    Hold(100);
    
    cout << "End of simulation reached." << endl;

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
