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

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#ifdef USE_RZSTACKS

// Define this to make a standalone test version
//#define TEST_RZSTACKS

#include <RZStacks.h>
#include <sys/types.h>
extern "C" {
#include <sys/mman.h>
}
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

extern "C" {
#ifndef __LINUX__    
    int getpagesize();
#endif    
#if defined(sun) && defined(sparc) && defined(__GNUC__)
    // The sun/g++ combination doesn't seem to want
    // know about all these things. SIG_PF seems to be
    // a test for this particular strangeness.
     void * valloc(size_t);
     int mprotect(caddr_t, unsigned int, int);
  #ifdef TEST_RZSTACKS
      caddr_t sbrk(int);
  #endif
#endif
}

/*
 * Define this here for linux, rather than including bsd.h since
 * that's the wrong place for NBBY anyway.
 */

#ifdef __LINUX__
#define NBBY 8           /* Number of bits in a byte */
#endif

static const int bits_per_ulong = sizeof (unsigned long)*NBBY;

int RZStacks::max_size = -1;
int RZStacks::page_size = getpagesize();
RZStacks::stack_block** RZStacks::blocks;

// Round up size in bytes to pages

int RZStacks::size_to_pages(unsigned long sz)
{
    return (sz+page_size-1)/page_size;
}

// Expand the size of the blocks table to accomodate a bigger block

void
RZStacks::expand_size(int sz)
{
    int exsz = (sz+1)*3/2;
    stack_block** newblocks = new stack_block*[exsz];
    if(max_size > 0) {
	// copy the old block table
	for(int i = 0; i <= max_size; i++) {
	    newblocks[i] = blocks[i];
	}
	delete [] blocks;
	blocks = 0;
    }
    for(int i = max_size+1; i < exsz; i++)
	newblocks[i] = 0;
    max_size = exsz-1;
    blocks = newblocks;
}

// Return a redzoned block

void*
RZStacks::new_stk(unsigned long sz)
{
    int npages = size_to_pages(sz);
    if(npages > max_size)
	expand_size(npages);

    // Allocate a batch of blocks if there isn't a pool for this size
    if(blocks[npages] == 0)
	blocks[npages] = new stack_block(npages, page_size, 0);

    // Try to allocate from the block pool
    void* newstk = find_stk_in_chain(blocks[npages]);
    if(newstk == 0) {
	// If all the pool was used up, allocate a new batch
	// and get the block from there
	blocks[npages] = new stack_block(npages, page_size, blocks[npages]);
	newstk = find_stk_in_chain(blocks[npages]);
    }
    return newstk;
}

// Return a block to the pool

void
RZStacks::delete_stk(void* stk)
{
    // We don't know what size it is; search...
    for(int i = 0; i <= max_size; i++) {
	if(blocks[i]) {
	    if(free_stk_in_chain(&blocks[i], stk))
		return;
	}
    }
}

void*
RZStacks::find_stk_in_chain(stack_block* sb)
{
    while(sb) {
	void* stk = sb->alloc_stack();
	if(stk)
	    return stk;
	sb = sb->next;
    }
    return 0;
}

int
RZStacks::free_stk_in_chain(stack_block** sb, void* stk)
{
    stack_block* curr;
    while((curr = *sb)) {
	if(curr->free_stack(stk)) {
	    if(curr->unused()) {
		(*sb) = curr->next;
		delete curr;
	    }
	    return 1;
	}
	sb = &(curr->next);
    }
    return 0;
}

// Create a new batch of blocks, and link it into the chain

RZStacks::stack_block::stack_block(int sz, int pgsz, RZStacks::stack_block* nxt)
{
    size = sz;
    pagesize = pgsz;
    // We have to allocate 1 page for each block, and 1 for the end of
    // the batch as redzone pages. Make the overhead for the final
    // block < 5%. For example, if we ask for a stack of size 1 page,
    // we get a batch of 11 stacks, with extra page at the end,
    // occupying 23 pages.
    float overhead = 0.05;
    nstacks = int(1/(overhead*(size+1)))+1;

    // Limit it to the size of the free-blocks mask
    if(nstacks > bits_per_ulong)
	nstacks = bits_per_ulong;

    int alloc_size = (nstacks*(size+1)+1)*pagesize;
    stacks = valloc(alloc_size);
    if(!stacks) {
	perror("Stack allocation failed in RZStacks");
	abort();
    }
    free = (1<<(nstacks)) - 1;
    next = nxt;

    // Set the protection pages to have no access
    for(int i = 0; i < alloc_size; i += (size+1)*pagesize) {
	if(mprotect((caddr_t)stacks + i, pagesize, PROT_NONE) < 0)
	    perror("Warning: red zone protection failed in RZStacks");
    }
}

RZStacks::stack_block::~stack_block()
{
    // Make sure all the pages can now be accessed
    if(mprotect((caddr_t)stacks,
    		(nstacks*(size+1)+1)*pagesize,
    		PROT_READ|PROT_WRITE|PROT_EXEC) < 0) {
	perror("Warning: can't reset red zone protection in RZStacks");
	abort();
    }
    ::free((char*)stacks);
}

// Get a new block from the batch; returns null if there's none
// available

void*
RZStacks::stack_block::alloc_stack()
{
    if(free) {
	unsigned long msk = 1;
	int i = 0;
	// Find a set bit in the free mask
	while(i < nstacks) {
	if(free & msk) {
	    // and allocate it...
	    free &= ~msk;
	    return (caddr_t)stacks
	     + (i*(size+1)+1)*pagesize;
	}
	i++;
	msk <<= 1;
	}
    }
    return 0;
}

// Try to return a block to the batch; returns 1 if the block
// belonged to the batch and was freed

int
RZStacks::stack_block::free_stack(void* stk)
{
    caddr_t stktop = (caddr_t)stacks
	    + ((nstacks*(size+1)+1)*pagesize);

    // Check that the pointer lies in the bounds of the batch
    if((caddr_t)stk >= (caddr_t)stacks
    && (caddr_t)stk < stktop) {
	int i = ((caddr_t)stk - pagesize - (caddr_t)stacks)
		/((size+1)*pagesize);
	// Mark the block free
	free |= (1<<i);
	return 1;
    }
    return 0;
}

int
RZStacks::stack_block::unused()
{
    return free == (1<<nstacks)-1;
}

#ifdef TEST_RZSTACKS

// Code to test it all

#if defined(__alpha) || (defined(sun) && defined(sparc) && defined(__GNUC__))
#  ifndef __STDC__
#    define __STDC__
#  endif

#  define SIG_PF	void (*)(int)
#endif


#include <signal.h>
#include <setjmp.h>
#include <iostream.h>

#define NREP 3
#define NSTACKS 20
#define NSIZES 10

volatile static int got_signal = 0;
static jmp_buf goback;

void
sigsegv()
{
    got_signal++;
    longjmp(goback, 1);
}

static int pgsz = getpagesize();

// Returns 1 if the access was OK, 0 if it caused a SIGSEGV

int
probe(void* stk, int offset)
{
	int sig_was = got_signal;
	if(!setjmp(goback)) {
	    ((unsigned char*)stk)[offset] = 0;
	} else
	    signal(SIGSEGV, (SIG_PF)sigsegv);
	return got_signal == sig_was;
}

int
test_redzone(int sz, int offset, void* stk, char* mess, int skip)
{
    int nfail = 0;
    int i;
    for(i = offset; i < offset+sz; i += skip) {
    	if(probe(stk, i)) {
	    cerr << "SEGV failed in stack redzone " << stk
		<< " at offset " << i << endl;
	    nfail++;
	}
    }
    if(i != offset+sz) {
	i = offset+sz-1;
    	if(probe(stk, i)) {
	    cerr << "SEGV failed in stack redzone " << stk
		<< " at offset " << i << " (upper bound)"<< endl;
	    nfail++;
	}
    }
    if(nfail)
	cerr << nfail << " redzone protection failures in "
	    << mess << endl;
    return nfail;
}

int
test_stack(int sz, void* stk, int skip)
{
    signal(SIGSEGV, (SIG_PF)sigsegv);
    got_signal = 0;
    sz *= pgsz;
    int i;
    for(i = 0; i < sz; i += skip) {
    	if(!probe(stk, i)) {
	    cerr << "SEGV in stack " << stk
		<< " at offset " << i << endl;
	}
    }
    if(i != sz) {
    	if(!probe(stk, sz-1)) {
	    cerr << "SEGV in stack " << stk
		<< " at offset " << i << " (end of stack) " << endl;
	}
    }
    if(got_signal)
	cerr << got_signal << "SEGV errors in stack " << stk << endl;
    int nfail = got_signal;
    nfail += test_redzone(pgsz, -pgsz, stk, "lower redzone", skip);
    nfail += test_redzone(pgsz, sz, stk, "upper redzone", skip);
    signal(SIGSEGV, SIG_DFL);
    return nfail;
}

void
do_fill(int sz, void* stk, int fill)
{
    sz *= pgsz;
    for(int i = 0; i < sz; i++) {
	((unsigned char*)stk)[i] = (unsigned char)fill;
    }
}

void
test_fill(int sz, void* stk, int fill)
{
    sz *= pgsz;
    for(int i = 0; i < sz; i++) {
	if(((unsigned char*)stk)[i] != (unsigned char)fill)
	    cerr << "Fill error in stack " << stk
		<< " at offset " << i << " : "
		<< (int)(((unsigned char*)stk)[i])
		<< " should be "
		<< (int)((unsigned char)fill)
		<< endl;
    }
}

main(int argc, char** argv)
{
    int skip = 256;
    if(argc >= 2) {
	int new_skip = atoi(argv[1]);
	if(new_skip < 1) {
	    cerr << "Bad value for skip distance in redzone test. Using "
		<< 256 << endl;
	} else
	    skip = new_skip;
    }
    caddr_t mem_top = (caddr_t)sbrk(0);
    for(int n = 1; n <= NREP; n++) {
	void* stacks[NSIZES][NSTACKS];
	int nfail = 0;
	cerr << "PASS " << n << endl;
	cerr << "Creating stacks ";
	int i;
	for(i = 0; i < NSIZES; i++) {
	    cerr << ".";
	    for(int j = 0; j < NSTACKS; j++)
		stacks[i][j] = RZStacks::new_stk((i+1)*pgsz);
	}
	cerr << endl;
	cerr << "Testing redzones";
	for(i = 0; i < NSIZES; i++) {
	    cerr << ".";
	    for(int j = 0; j < NSTACKS; j++)
		nfail += test_stack(i+1, stacks[i][j], skip);
	}
	cerr << endl;
	cerr << "Testing for overwriting";
	if(!nfail) for(i = 0; i < NSIZES; i++) {
	    cerr << ".";
	    for(int j = 0; j < NSTACKS; j++)
		do_fill(i+1, stacks[i][j], i*NSTACKS+j);
	}
	if(!nfail) for(i = 0; i < NSIZES; i++) {
	    cerr << ".";
	    for(int j = 0; j < NSTACKS; j++)
		test_fill(i+1, stacks[i][j], i*NSTACKS+j);
	}
	cerr << endl;
	cerr << "Deleting stacks";
	for(i = 0; i < NSIZES; i++) {
	    cerr << ".";
	    for(int j = 0; j < NSTACKS; j++)
		RZStacks::delete_stk(stacks[i][j]);
	}
	cerr << endl;
	caddr_t new_memtop = (caddr_t)sbrk(0);
	cerr << "Memory allocated from system: " << (new_memtop-mem_top)
		<< " bytes" << endl;
	mem_top = new_memtop;
    }
    return 0;
}

#endif
#endif /* USE_RZSTACKS */
