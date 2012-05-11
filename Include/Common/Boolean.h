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
 *
 * $Id: Boolean.h,v 1.2 1996/12/02 14:19:24 nmcl Exp $
 */

#ifndef BOOLEAN_H_
#define BOOLEAN_H_

/* first check for TRUE and FALSE definitions - if so then undefine them */

#ifdef TRUE
#  undef TRUE
#  undef FALSE
#endif

/* For C++ usage */

#ifndef HAVE_BOOLEAN_TYPE
typedef unsigned short Boolean;

extern const Boolean TRUE;
extern const Boolean FALSE;
#else

typedef bool Boolean;

#  define TRUE true
#  define FALSE false
#endif

#endif

