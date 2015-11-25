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

#ifndef BREAKS_H_
#  include "Breaks.h"
#endif

#ifndef MACHINE_H_
#  include "Machine.h"
#endif

#ifndef QUEUE_H_
#  include "Queue.h"
#endif

#include <iostream>

extern Machine* M;
extern Queue JobQ;
extern double MachineFailedTime;


Breaks::Breaks ()
{
    RepairTime = new UniformStream(10, 100);
    OperativeTime = new UniformStream(200, 500);
    interrupted_service = FALSE;
}

Breaks::~Breaks ()
{
    delete RepairTime;
    delete OperativeTime;
}

void Breaks::Body ()
{
    for(;;)
    {
        double failedTime = (*RepairTime)();

	Hold((*OperativeTime)());

	M->Broken();
	M->Cancel();

	if(!JobQ.IsEmpty())
	    interrupted_service = TRUE;

	Hold(failedTime);

	MachineFailedTime += failedTime;
	M->Fixed();
	
	if (interrupted_service)
	    M->ActivateAt(M->ServiceTime() + CurrentTime());
	else
	    M->ActivateAt();

	interrupted_service = FALSE;
    }
}
