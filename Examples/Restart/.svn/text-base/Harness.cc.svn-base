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

#ifndef RESET_H_
#  include <ClassLib/reset.h>
#endif

#ifndef HARNESS_H_
#  include "Harness.h"
#endif


Harness::Harness ()
		 : status(FALSE),
		   _doPassivate(FALSE)
{
}

Harness::~Harness () {}

void Harness::Body ()
{
    status = TRUE;
    RESET_POINT;   // the ';' is optional

    if (_doPassivate)
    {
	cout << "\tHarness passivated." << endl;
	Passivate();
    }
    
    cout << "\tHarness holding." << endl;
    Hold(10);

    cout << "\tHarness holding and checking." << endl;
    MONITOR_RESET(Hold(1000), status, FALSE); // the ';' is optional

    /*
     * The following is just as valid as the previous
     * MONITOR_RESET
     *
     * Hold(1000);
     * CHECK_RESET(status, FALSE);
     */
    
    cout << "\tHarness passivated." << endl;
    Passivate();
}

void Harness::reset ()
{
    cout << "\tHarness reset function called." << endl;
    
    status = FALSE;
}

void Harness::doPassivate ()
{
    _doPassivate = TRUE;
}
