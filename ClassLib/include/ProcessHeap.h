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

#ifndef PROCESSHEAP_H_
#define PROCESSHEAP_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef PROCESS_H_
#  include <ClassLib/Process.h>
#endif

#ifndef LISTBASE_H_
#  include "ListBase.h"
#endif


class HeapElement;

/*
 * A heap-based queue for maintaining simulation processes.
 * This is an implementation of the scheme described in the
 * C++SIM paper.
 * We keep the number of event lists to a minimum by comparing
 * approximate times (event times as a long rather than as a
 * double).
 * Note: due to an oversight, this was not shipped in versions
 * prior to 1.5.
 */

class ProcessHeap : public ListBase
{
    friend class HeapIterator;
public:
    ProcessHeap ();
    ~ProcessHeap ();

    void Insert (Process&, Boolean prior = FALSE);
    Boolean InsertBefore (Process &ToInsert, Process &Before);
    Boolean InsertAfter (Process &ToInsert, Process &After);
    Process *Remove (const Process *p = 0);
    const Process* getNext (const Process*) const;

    ostream& print (ostream&) const;

private:
    Process* findMinimum () const;

    void InsertAt (Process&, HeapElement*);
    void RemoveFrom (HeapElement*);

    HeapElement* Head;
    int numberOfElements;
};

extern ostream& operator<< (ostream& strm, const ProcessHeap& ph);

#endif
