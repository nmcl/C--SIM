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
 * Copyright (C) 1994-1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Semaphore.cc,v 1.8 1998/09/30 08:07:15 nmcl Exp $ 
 */

#ifndef ENTITY_H_
#  include <Event/Entity.h>
#endif

#ifndef SEMAPHORE_H_
#  include <Event/Semaphore.h>
#endif

#ifndef ENTITILIST_H_
#  include <EntityList.h>
#endif

#include <ClassLib/Process.h>

Semaphore::Semaphore ()
		     : numberWaiting(0),
		       numberOfResources(1),
		       currentResources(1),
		       haveCeiling(FALSE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_SEMAPHORE << VIS_PUBLIC;
    debug_stream << "Semaphore::Semaphore ()" << endl;
#endif    
}

Semaphore::Semaphore (long number, Boolean ceiling)
		     : numberWaiting(0),
		       numberOfResources(number),
		       currentResources(number),
		       haveCeiling(ceiling)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_SEMAPHORE << VIS_PUBLIC;
    debug_stream << "Semaphore::Semaphore ( " << number << " )" << endl;
#endif    
}

Semaphore::~Semaphore ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_SEMAPHORE << VIS_PUBLIC;
    debug_stream << "Semaphore::~Semaphore ()" << endl;
#endif
    
    if (numberWaiting != 0)
	cerr
		     << "Semaphore being removed with clients waiting." << endl;
}

long Semaphore::NumberWaiting () const
{
    return numberWaiting;
}

Semaphore::Outcome Semaphore::Get (Entity* toWait)
{
    /*
     * If no resources, wait.
     */
    
    while (currentResources == 0)
    {
	numberWaiting++;

        waitingList.insert(toWait);

	toWait->Cancel();
    }

    /*
     * Grab a resource.
     */
    
    currentResources--;

    if (currentResources < 0)
	currentResources = 0;
    
    return Semaphore::DONE;
}

Semaphore::Outcome Semaphore::TryGet (Entity* toWait)
{
    if (currentResources == 0)
	return Semaphore::WOULD_BLOCK;
    else
	return Get(toWait);
}

/*
 * A return of NOTDONE simply means that nothing was woken
 * up.
 */

Semaphore::Outcome Semaphore::Release ()
{
    /*
     * Release a resource.
     */
    
    currentResources++;

    if ((currentResources > numberOfResources) && (haveCeiling))
	currentResources = numberOfResources;

    /*
     * If any processes are waiting, wake up the first.
     */
    
    if (numberWaiting > 0)
    {
	numberWaiting--;

	// don't set trigger flag - not strictly a trigger

	waitingList.triggerFirst(FALSE);

	Process::Current->ReActivateAt(Process::CurrentTime());

	return Semaphore::DONE;
    }
    else
	return Semaphore::NOTDONE;
}
