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
 * $Id: Consumer.h,v 1.1 1996/12/09 10:10:04 nmcl Exp $
 */

#ifndef CONSUMER_H_
#define CONSUMER_H_

#ifndef ENTITY_H_
#  include <Event/Entity.h>
#endif

#ifndef RANDOM_H_
#  include <ClassLib/Random.h>
#endif

#ifndef SEMAPHORE_H_
#  include <Event/Semaphore.h>
#endif

/* Controls the rate at which Jobs arrive at the Machine */

class Consumer : public Entity
{
public:
    Consumer (double);
    virtual ~Consumer ();

    virtual void Body ();

    static Semaphore _semaphore;
    
private:
    ExponentialStream* InterArrivalTime;
};

#endif
