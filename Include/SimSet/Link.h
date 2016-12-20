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

/*
 * This class defines the elements of the linked lists within SIMSET.
 */

#ifndef LINK_H_
#define LINK_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef LINKAGE_H_
#  include <SimSet/Linkage.h>
#endif


class Head;

class Link : public Linkage
{
    friend class Head;

public:
    virtual ~Link ();

    virtual Link* Suc () const;
    virtual Link* Pred () const;

    Link* Out ();
    void  InTo (Head* list);

    void Precede (Link* toPrecede);
    void Precede (Head* list);

    void Follow  (Link* toFollow);
    void Follow  (Head* list);

    Boolean inList () const;

protected:
    Link ();             // can only derive from this class

private:
    void RemoveElement ();

    void addAfter  (Link*);
    void addBefore (Link*);

    Link    *prev, *next;
    Head    *theList;
};


#include <SimSet/Link.n>

#endif
