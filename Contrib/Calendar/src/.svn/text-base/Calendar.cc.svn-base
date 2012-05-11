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
 * Calendar Queue for Process List
 *     - See Randy Brown, CACM 30(10): 1220-1227 (10/88).
 *     - See also SIMPACK++ code, Paul Fishwick, cis.ufl.edu.
 *
 * Ian.Mathieson@mel.dit.CSIRO.AU, 11 March 1994.
 */

/*
 * Modified slightly for new C++SIM format (release >= 1.5)
 * by M.C.Little@uk.ac.newcastle.
 * 22/3/94
 *
 * Changed the data structures for handling the process lists,
 * in particular making long chains of Processes scheduled for
 * the same time much more efficient. This was important for our
 * TRITRAM traffic simulator (where in some of our simulations
 * large numbers of traffic lights can change state at the same time).
 * Changes mostly by bella.robinson@cmis.csiro.au with some input
 * from peter.lamb@cmis.csiro.au.
 * Jan/Feb 1997
 */

#include <iostream.h>
#include <iomanip.h>
#ifndef CALENDAR_H_
#  include "Calendar.h"
#endif


Calendar::Calendar ()
{
    Init(0, 2, 1.0, 0.0);
    resizeEnable = TRUE;
}

const Process*
Calendar::getNext (const Process* current) const
{
    // take care of boundary condition - empty list.

    if (qSize == 0 || current == (Process*) 0)
	return (Process*) 0;
    
	// find what bucket current is in
    int i = WhichBucket(current->evtime());

    ProcessNode *pn = 0;
    TimeNode* tn = calendar[i];

    while (tn && tn->time < current->evtime()) {   // find matching time node
         tn = tn->nextTimeNode;
    }
    if (tn && tn->time == current->evtime()) {   // search through process node list
         pn = tn->first;
         while (pn && pn->p != current) {
             pn = pn->nextProcessNode;
         } 
    }

	// if found,
	//    return next if it's < top of this bucket
	//    otherwise return earliest in next bucket
    if ( pn ) {
	    // check next on this proccessNode list first
	pn = pn->nextProcessNode;
	if ( pn ) return pn->p ;
            // not on this processNode list so check on next one
        double top = BucketTop(current->evtime());
        tn = tn->nextTimeNode;
        if (tn && tn->time < top) return tn->first->p;    

        // not in that bucket so look in all other buckets for the 
        // process > current that is the minimum of all those > current

        double minTime;
        TimeNode* minTN = 0;
 
        for (i = 0; i < nBuckets; i++) {
            TimeNode* tn = calendar[i];
            while (tn) {
                if (!minTN && tn->time > current->evtime()) {
                     minTN = tn;
                     minTime = tn->time;
                 }
                 else if (minTN && tn->time > current->evtime() && tn->time < minTime) {
                     minTN = tn;
                     minTime = tn->time;
                 }
                 if (tn->time > current->evtime()) tn = 0;
                 else tn = tn->nextTimeNode;
            }
        }
        if (minTN) return minTN->first->p;                      
    }

    /*
     * If we get here then we have not found current on the list
     * which can only mean that it is currently active.
     */

	// return earliest of all if current not found
     Process *e = 0;
     double earliestTime = 0;
     int k;
     // find first non_empty bucket
     for (k = 0; k < nBuckets; k++) 
          if (calendar[k]) break;

     e = calendar[k]->first->p;
     earliestTime = e->evtime();

     for (int m = k+1; m < nBuckets; m++) {
         if (calendar[m] && calendar[m]->time < earliestTime) {
             e = calendar[m]->first->p;
             earliestTime = e->evtime();
         }
     }
     return e;
} 

      

// Initialise calendar array within the array calQ[].
// Set bucket width to bWidth. Set calendar[0] to calQ[qBase].
// Number of buckets is nBuck. StartPrio is initial priority.
// Set up all data members apart from resizeEnable.

void
Calendar::Init(int qBase,int nBuck, double bWidth, double startPrio)
{
	// set position and size of new queue
    firstSub = qBase;
    calendar = calQ+qBase;
    width = bWidth;
    nBuckets = nBuck;

	// initially empty
    qSize = 0;
    for (int i = 0; i < nBuckets; i++) {
	if ( calendar[i] ) {
           DeleteTimeNodeList(calendar[i]);
        }
	calendar[i] = 0;
    }

	// set up initial position in queue
    lastPrio = startPrio;
    long n = long(startPrio/width);	// virtual bucket
    lastBucket = int(n % nBuckets);
    bucketTop = BucketTop(lastPrio);

	// set up queue size change thresholds
    botThreshold = nBuckets/2 - 2;
    topThreshold = 2 * nBuckets;
}

void
Calendar::DeleteTimeNodeList(TimeNode* head)
{
    TimeNode* tn = head;

    while (tn) {
        ProcessNode* pn = tn->first;
	while (pn) {
            ProcessNode* nextPN = pn->nextProcessNode;
            delete pn;
            pn = nextPN;
        }
        TimeNode* nextTN = tn->nextTimeNode;
        delete tn;
	tn = nextTN;
    } 
}

       
// Insert process into calendar based on its scheduled event time.
// Insert before matching times if prior is TRUE, otherwise append.
// Only increment qSize if create a new TimeNode, i.e. if there was not
// already a process in the calendar queue with the same evtime

void
Calendar::Insert (Process &p, Boolean prior)
{
#ifdef DEBUG_QUEUE
    cout << endl << "Inserting " << p.evtime() << endl;
#endif
	// calculate the bucket number for the entry
    int i = WhichBucket(p.evtime());

        // insert into calendar[i]

    ProcessNode* pn = new ProcessNode(&p);

    if (!calendar[i]) {    // bucket empty
        calendar[i] = new TimeNode(pn);
        qSize++;
    }
    else {
        TimeNode* tn = calendar[i];
        if (tn->time > pn->Time()) {   // put at front of TimeNode list 
            calendar[i] = new TimeNode(pn);
            calendar[i]->nextTimeNode = tn;
            qSize++;
        }
        else {   // find slot to put it
            TimeNode* prev = 0;
            while (tn && tn->time < pn->Time()) {
                prev = tn;
                tn = tn->nextTimeNode;
             }
             if (tn && tn->time == pn->Time()) {  // put ProcessNode into tn's list
                 if (prior) {  // put at front of tn's list
                     pn->nextProcessNode = tn->first;
                     tn->first = pn;
                 }
                 else {  // put at end of tn's list
                     tn->last->nextProcessNode = pn;
                     tn->last = pn;
                 }
              }
              else if (tn && tn->time > pn->Time()) {   // no existing TimeNode with the same evtime 
                        // make new TimeNode and insert between prev and tn
                     TimeNode* newTN = new TimeNode(pn);
                     prev->nextTimeNode = newTN;
                     newTN->nextTimeNode = tn;
                     qSize++;
              }
              else if (!tn) {  // put at end of TimeNode list
                  TimeNode* newTN = new TimeNode(pn);
                  prev->nextTimeNode = newTN;
                  qSize++;
              }
        }
    }

	// double calendar size if needed
    if ( qSize > topThreshold ) {
#ifdef DEBUG_QUEUE
              cout << "about to do resize" << endl;
              cout << "queue before resize " << endl;
              print(cout);
#endif
              Resize( 2*nBuckets );
    }
#ifdef DEBUG_QUEUE
    print(cout);
#endif
}


// Insert process TI immediately before B in calendar.

Boolean
Calendar::InsertBefore (Process &ToInsert, Process &Before)
{
     Insert(ToInsert,1);
     return TRUE;
}

// Insert process TI immediately after A in calendar.

Boolean
Calendar::InsertAfter (Process &ToInsert, Process &After)
{
      Insert(ToInsert);
      return TRUE;
}

// Remove and return the specified or lowest priority process from calendar.

Process *
Calendar::Remove (const Process *element)
{
#ifdef DEBUG_QUEUE
    cout << endl << "doing a remove " << endl;
#endif

    TimeNode *tn;	// indicates the correct list to remove from
    int i;		// likewise for the correct bucket
    Process* p = 0;

	// take care of boundary condition - empty list
    if ( qSize == 0 ) return 0;

	// remove specified element if it's there ...
    if (element) {

	i = WhichBucket(element->evtime());
	tn = calendar[i];

    } else {

	// ... otherwise remove lowest priority process
	    // search buckets once
	for ( i = lastBucket;; ) {
		// check calendar[i] 
	    tn = calendar[i];
	    if ( tn && tn->time < bucketTop ) {
		goto FOUND;
	    } else {
		    // do next bucket or direct search
		if ( ++i == nBuckets ) i = 0;
		bucketTop += width;
		if ( i == lastBucket ) break;	// direct search
	    }
	}

	    // direct search to find earliest
	double earliest = 0;
	i = -1;
	for ( int j = 0; j < nBuckets; j++ ) {
	    tn = calendar[j];
	    if ( tn && ( i == -1 || tn->time < earliest ) ){
		i = j;
		earliest = tn->time;
	    }
	}
	tn = calendar[i];
    }

FOUND:

	// remove process from list and update/resize as needed
    if (element) {
         // remove it from bucket i
         TimeNode* prevTN = 0;
         while (tn && tn->time != element->evtime()) {
             prevTN = tn;
             tn = tn->nextTimeNode;
         }

         if (!tn) return 0;   // element not there

         // find the correct ProcessNode in tn's list
         ProcessNode* tempPN = tn->first;
         ProcessNode* prevPN = 0;
         while (tempPN && tempPN->p != element)  {
             prevPN = tempPN;   
             tempPN = tempPN->nextProcessNode;
         }
         if (!tempPN) return 0;  // element not in the process node list

         p = tempPN->p;   // p is the process to return

         // found processNode to remove so remove it
         if (!prevPN) {  // processNode to remove is at top of tn's list
             if (tn->first == tn->last) {  // only one process in tn's list so remove it all
                 delete tn->first;
                 if (!prevTN) 
                     calendar[i] = tn->nextTimeNode;
                 else
                     prevTN->nextTimeNode = tn->nextTimeNode;
                 delete tn;
                 qSize--;
              }
              else {  // just delete the process node at the top of tn's list
                  tn->first = tempPN->nextProcessNode;
                  delete tempPN;
              }
          }
          else {  
              if (tempPN == tn->last) {  // processNode to remove is at the end of tn's list
                  prevPN->nextProcessNode = 0;
                  tn->last = prevPN;
                  delete tempPN;
              }
              else {   // processNode to remove is in the middle
                  prevPN->nextProcessNode = tempPN->nextProcessNode;
                  delete tempPN;
              }
          }
    }
    else {  // removing earliest process
        p = tn->first->p;
        if (tn->first == tn->last) { // only one process in tn's list so remove it all
             delete tn->first;
             calendar[i] = tn->nextTimeNode;
             delete tn;
             qSize--;
        }
        else {   // just remove first ProcessNode in tn's list 
           ProcessNode* temp = tn->first;
           tn->first = temp->nextProcessNode;  
           delete temp;
        } 
    }

    if ( p ) {
	if(!element) {
	    // Only move the search position if this was a normal
	    // remove-from-head-of-queue
	    lastBucket = i;
	    lastPrio = p->evtime();
	    bucketTop = BucketTop(lastPrio);
	}
	if ( qSize < botThreshold ) {
#ifdef DEBUG_QUEUE
              cout << "about to do resize" << endl;
              cout << "queue before resize " << endl;
              print(cout);
#endif
              Resize( nBuckets/2 );
        }
    }
#ifdef DEBUG_QUEUE
    print(cout);
#endif
    return p;
}





// Print out the event times of each process on the calendar,
// plus the calendar configuration.

ostream&
Calendar::print (ostream& strm) const
{
    for ( int i = 0; i < nBuckets; i++ ) {
	strm << "\nBucket " << (double)i << " = ";
	if ( i==lastBucket ) strm << " <";
        TimeNode* tn = calendar[i];
        while (tn) {
            strm << "(";
            ProcessNode* pn = tn->first;
            while (pn) {
                strm << setprecision(8) << pn->Time() << "  ";
                pn = pn->nextProcessNode;
            }
            strm << ")";
            tn = tn->nextTimeNode;
        }
    }
    strm << "\n";
    strm << "qSize == " << (double)qSize << "\n";
    strm << "lastPrio == " << lastPrio << "\n";
    strm << "bucketTop == " << bucketTop << "\n";
    strm << "width == " << width << "\n";
    strm << "topThreshold == " << (double)topThreshold << "\n";
    strm << "botThreshold == " << (double)botThreshold << "\n";
    strm << "firstSub == " << (double)firstSub << "\n";

    return strm;
}





// Move the current queue items into a bigger/smaller set of buckets
// at the opposite end of calQ from the original.

void
Calendar::Resize(int sz)
{
#ifdef DEBUG_QUEUE
    cout << endl << "doing a resize to " << sz << endl;
#endif

    if ( !resizeEnable ) return;
    if ( sz > MAXNBUCKETS || sz < 1 ) return;

	// preserve location and size of current calendar
    TimeNode **oldC = calendar;
    int oldN = nBuckets;

	// setup new calendar
    Init( (firstSub ? 0 : QSPACE-sz), sz, NewWidth(), lastPrio );

	// move list elements onto new calendar
    for ( int i = 0; i < oldN; i++ ) {
	TimeNode *tn = oldC[i];
        while (tn) {
            ProcessNode* pn = tn->first;
            while (pn) {
	        Process *p = pn->p;
                Insert ( *p);
                pn = pn->nextProcessNode;
            }
            tn = tn->nextTimeNode;
        }
    }
#ifdef DEBUG_QUEUE
    cout << endl << "done a resize " << endl;
#endif
}

// Randomly sample the queues and calculate a new width to
// accomodate roughly 3 items per bucket.

double
Calendar::NewWidth()
{
        // decide how many elements to sample
    if ( qSize < 2 ) return 1.0;
    int nsamples = ( qSize <= 5 ) ? qSize : 5 + qSize/10;
    const int maxSamples = 25;
    if ( nsamples > maxSamples ) nsamples = maxSamples;

        // record current configuration and disable resize
    int oldLB = lastBucket;
    double oldLP = lastPrio;
    double oldBT = bucketTop;
    resizeEnable = FALSE;
    double top = bucketTop;

        // get nsample events and calculate average event separation
    double ave = 0.0;
    Process *tmp[nsamples];
   
    TimeNode* tn = calendar[lastBucket];
    int bucket = lastBucket;
    if (!tn || tn->time > top) { // find bucket with earliest time 
        while (!tn || tn->time > top) {
           if (++bucket >= nBuckets) bucket = 0;
           tn = calendar[bucket];
           top += width;
        }
     }

     TimeNode** currTimeNode = new TimeNode*[nBuckets];
     int i;
     for (i = 0; i < nBuckets; i++)
         currTimeNode[i] = calendar[i];
     i = 0;
     while (i < nsamples) {
         tmp[i] = tn->first->p;
         tn = tn->nextTimeNode;
         currTimeNode[bucket] = tn; // set bucketPtr[bucket] to point to next TimeNode
                                    // so that if we come to that bucket we sample 
                                    // the next time in that bucket and not the same one
                                    // we have already sampled

         if (++i < nsamples) {  // find next time to sample
             while (!tn || tn->time > top ) {  // look in next bucket 
                 if (++bucket >= nBuckets) bucket = 0;
                 tn = currTimeNode[bucket];
                 top += width;
             } 
         }
     }
     delete []currTimeNode;

#ifdef DEBUG_QUEUE
    // print out samples
    for (i = 0; i < nsamples; i++)
       cout << tmp[i]->evtime() << " ";
    cout << endl;
#endif
       
    // calculate average event separation
    for ( i = 1; i < nsamples; i++ ) {
        ave += tmp[i]->evtime() - tmp[i-1]->evtime();
    }
    ave /= nsamples-1;

    // recalculate average ignoring large separations
    double ave2 = 0.0;
    int count = 0;
    for ( i = nsamples; --i > 0; ) {
        double delta = tmp[i]->evtime() - tmp[i-1]->evtime();
        if ( delta < ave * 2.0 ) {
            ave2 += delta; count++;
        }
    }
    ave2 /= count;
    double newW = 3 * ave2;

        // restore configuration and enable resize again
    lastBucket = oldLB;
    lastPrio = oldLP;
    bucketTop = oldBT;
    resizeEnable = TRUE;
 
    return newW;
}


Calendar::TimeNode::TimeNode(ProcessNode* pn) :
	nextTimeNode(0),
	first(pn),
	last(pn)
{
	time = pn->Time();
}


Calendar::ProcessNode::ProcessNode(Process* newp) :
	p(newp),
	nextProcessNode(0)
{
}

double Calendar::ProcessNode::Time() {
	return p->evtime();
}





#ifdef NO_INLINES
#  define CALENDAR_CC_
#  include "Calendar.n"
#  undef CALENDAR_CC_
#endif
