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
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Main.cc,v 1.2 1998/09/30 08:07:18 nmcl Exp $
 */

#include <iostream>
#include <string.h>

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef CONTROLLER_H_
#  include "Controller.h"
#endif


int main (int, char**)
{
    Thread::Initialize(); // Initialize the threads package.

    Controller c; // Start up the real main body of the simulation.

    c.Await();               // Suspend main's thread (NOTE: this MUST be done
                             // by all applications.)

    c.Exit();                // Reactivate main thread and then exit
    
    return 0;
}
