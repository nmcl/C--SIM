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
 * Copyright 1996 Commonwealth Scientific and Industrial Research
 * Organisation, Australia
 * Available under the same conditions as the C++SIM public distribution.
 */

#ifdef USE_RZSTACKS

#ifndef RZSTACKS_H_
#define RZSTACKS_H_

#include <malloc.h>

// RZStacks is a class to manage red-zoned blocks of memory.
// Each block of memory allocated by RZStacks::new_stk() is
// page-aligned, an integer number of pages long, and has
// a page of protected memory at each end, so that out-of-bounds
// accesses near the block boundary cause SIGSEGV violations.
// In order to reduce the overhead of the protection pages,
// allocation is done in batches of same-sized blocks.

// The intended use is for thread stacks, but there's nothing
// preventing the memory allocated by RZStacks::new_stk() being
// used for any other purposes.

// RZStacks::delete_stk() must be given a pointer created
// by RZStacks::new_stk(). Other values will be silently ignored.

// Requires system support for valloc(3) and mprotect(2).

class RZStacks {
public:
		// Create a red-zone protected block of memory
		// Start is page-aligned, and the size is rounded up
		// to the next page.
    static void*	new_stk(unsigned long sz);
    		// Delete a red-zone protected block of memory
		// allocated by new_stk()
    static void		delete_stk(void* stk);

    // data structure for holding info about a batch of red-zoned blocks
    // of the same size. It really shoulkd be private, but some compilers
    // complain if it is, and the Alpha compiler refuses to compile it.

    class stack_block {
	unsigned long	nstacks;	// How many blocks in the batch
	unsigned long	free;		// A bitset indicating which are free
	unsigned long	size;		// The size of the blocks in pages
	unsigned long	pagesize;	// The pagesize in bytes
	void*		stacks;		// The batch
    public:
	stack_block*	next;		// Next batch of same-sized blocks
	stack_block(int size, int pagesize, stack_block* next);
	~stack_block();
	void* alloc_stack();
	int free_stack(void* stk);
	int unused();
    };
private:
    static int max_size;		// in pages
    static int page_size;		// size of a page
    static stack_block** blocks;

    RZStacks(){}	// You can't make instances of this class
 
    static int size_to_pages(unsigned long sz);
    static void expand_size(int newsz);
    static void* find_stk_in_chain(stack_block* sb);
    static int free_stk_in_chain(stack_block** sb, void* stk);

};

#endif

#endif /* USE_RZSTACKS */
