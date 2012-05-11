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
 * Copyright (C) 1994, 1995, 1996, 1997
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ProcessCons.h,v 1.3 1996/12/09 10:09:56 nmcl Exp $
 */

#ifndef PROCESSCONS_H_
#define PROCESSCONS_H_

#ifndef PROCESS_H_
#  include <ClassLib/Process.h>
#endif

class ProcessCons
{
public:
    ProcessCons (Process &p, ProcessCons *n);
    ~ProcessCons ();
    
    Process     *car ();
    ProcessCons *cdr ();
    void SetfCdr (ProcessCons *n);

private:
    Process *Proc;
    ProcessCons *Next;
};


#include "ProcessCons.n"

#endif
