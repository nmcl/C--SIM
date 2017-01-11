Calendar.{cc,n,h}

Implement a calendar class of ProcessLists. Limited to 32k buckets at present, which should handle 100k processes before average list length starts to exceed 3.  Could use a dynamic array for infinite resizability.

/*
 * Calendar Queue for Process List
 *     - See Randy Brown, CACM 30(10): 1220-1227 (10/88).
 *     - See also SIMPACK++ code, Paul Fishwick, cis.ufl.edu.
 *
 * Ian.Mathieson@mel.dit.CSIRO.AU, 11 March 1994.
 */
