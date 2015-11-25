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
 * $Id: ListBase.h,v 1.2 1996/12/09 10:09:56 nmcl Exp $
 */

#ifndef LISTBASE_H_
#define LISTBASE_H_

#include <iostream>

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef PROCESS_H_
#  include <ClassLib/Process.h>
#endif

// All scheduler queue implementations should be derived from this

class ListBase
{
public:
    virtual void Insert (Process&, Boolean) = 0;
    virtual Boolean InsertBefore (Process&, Process&) = 0;
    virtual Boolean InsertAfter (Process&, Process&) = 0;
    virtual Process *Remove (const Process*) = 0;
    virtual const Process* getNext (const Process*) const = 0;

    virtual ostream& print (ostream&) const = 0;

protected:
    ListBase () {};
    virtual ~ListBase () {};
};

#endif
