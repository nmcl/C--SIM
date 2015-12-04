Copyright 1996 Commonwealth Scientific and Industrial Research
Organisation, Australia
The Commonwealth Scientific and Industrial Research Organisation makes
no representation about the suitability of this software for any
purpose.  It is provided "as is" without express or implied warranty.
Available under the same conditions as the C++SIM public distribution.

Parts of this code
Copyright (C) 1994

Department of Computing Science,
The University,
Newcastle upon Tyne,
UK.



This is an implementation of threads using David Keppel's QuickThreads.

Reference: Keppel, David, "Tools and Techniques for Building Fast Portable
           Threads Packages," UWCSE 93-05-06, University of Washington
           Department of Computer Science and Engineering.

QuickThreads source and documentation is available through
  http://www.cs.washington.edu/homes/pardo/papers.d/thread.html
and also by ftp from:
  ftp://ftp.cs.washington.edu/tr/1993/05/UW-CSE-93-05-06.PS.Z>
for a paper describing QuickThreads, and
  ftp://ftp.cs.washington.edu/pub/qt-002.tar.gz
for the source.

As with the Sun threads interface, some problems may be worked around
by increasing the stack size.

The queue/priority system implemented here is the minimum needed for
C++SIM. If you want more priorities, you can increase QT_MAXPRIO, if
you run out of queue slots, you can increase QT_QLEN (but warning,
queue slots are statically allocated, and C++SIM doesn't need more
than what is available here).

Peter Lamb (peter.lamb@cmis.csiro.au)
Commonwealth Scientific and Industrial Research Organisation
Division of Information Technology
Canberra 2601, Australia
