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
 * $Id: HashedList.h,v 1.7 1998/07/31 08:50:24 nmcl Exp $ 
 */

#ifndef HASHEDLIST_H_
#define HASHEDLIST_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef PROCESS_H_
#  include <ClassLib/Process.h>
#endif

#ifndef PROCESSLIST_H_
#  include "ProcessList.h"
#endif

#ifndef LISTBASE_H_
#  include "ListBase.h"
#endif

class ostream;

const int defaultSize = 77;

/*
 * This scheme maintains a (currently fixed) size array
 * of ProcessLists and hashes the simulation times when inserting
 * and removing Processes.
 */

class HashedList : public ListBase
{
    friend class HashIterator;
    
public:
    HashedList (int size = defaultSize);
    ~HashedList ();

    void Insert (Process&, Boolean prior = FALSE);
    Boolean InsertBefore (Process &ToInsert, Process &Before);
    Boolean InsertAfter (Process &ToInsert, Process &After);
    Process *Remove (const Process *p = 0);
    const Process* getNext (const Process*) const;

    ostream& print (ostream&) const;

private:
    Process* findMinimum (const Process* from = 0) const;
    int timeToKey (double) const;

    ProcessList** hashedList;
    int listSize;
    int numberOfEntries;
};

#include "HashedList.n"

#endif
