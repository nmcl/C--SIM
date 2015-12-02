/*
 * Copyright (C) 1994-1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

M. Little		Computing Laboratory
29 November 1991	University of Newcastle upon Tyne, NE1 7RU, England

This is  public release  version 1.7.4 beta   of C++SIM, a set of  C++
class definitions  that mimic the process-based  simulation facilities
of SIMULA and the SIMSET routines.

The  co-routine facility of Simula   is implemented by various threads
(and others).  Classes  are    provided  for various   random   number
distributions.

If  you  find any bugs  or make  modifications (e.g.,  ports  to other
thread packages) or port the package to other systems then let me know
so I can keep the sources up-to-date for other users.

Send to: M.C.Little@newcastle.ac.uk

With the move to github and various refactoring it has become difficult to even check that
C++SIM builds on all of the platforms we once supported, let alone run successfully. Therefore,
at the moment all we can test against is Linux and that's the only platform (with gcc) that we
can support. However, rather than remove the code for all of the other platforms we once supported,
such as HPUX, Solaris and other thread packages, we've left them in the codebase for now in case
others in the community are able to use them to check that C++SIM can still build and run.

