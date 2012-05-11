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
 * $Id: TriggerQueue.h,v 1.3 1996/12/09 10:10:16 nmcl Exp $
 */

#ifndef TRIGGERQUEUE_H_
#define TRIGGERQUEUE_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

class Entity;
class Sempaphore;
class EntityList;

class TriggerQueue
{
    friend class Entity;
    friend class Semaphore;

public:
    TriggerQueue ();
    virtual ~TriggerQueue ();

    Boolean triggerAll ();
    Boolean triggerFirst (Boolean = TRUE);

protected:
    void insert (Entity*);
    Entity* remove ();

private:
    EntityList* head;
};

#endif
