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

#include <iostream>
#include <string.h>

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#ifndef PROCESS_H_
#  include <ClassLib/Process.h>
#endif

#ifndef MACHINESHOP_H_
#  include "MachineShop.h"
#endif


int main (int, char**)
{
    Thread::Initialize(); // Initialize the threads package.

    /*
     * We don't need to call Resource::ref on m as it is created on
     * the stack. However, it would not cause any problems to do so,
     * and if objects are passed by pointer we may not know that one
     * has not been created on the heap. So, whenever we have a pointer
     * we will always call ref and unref before use.
     */

    MachineShop m;   // Start up the real main body of the simulation.
    m.Await();       // Suspend main's thread (NOTE: this MUST be done by all applications.)
    m.Exit();        // Reactivate main thread and then exit
    
    return 0;
}
