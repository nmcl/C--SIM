C++SIM is an object-oriented simulation package which has been under development and available since 1990. It provides discrete event process-based simulation similar to SIMULA's simulation class and libraries. A complete list of the facilities provided follows:

- the core of the system gives SIMULA-like simulation routines, random number generators, queueing algorithms, and thread package interfaces.
- entity and set manipulation facilities similar to SIMSET.
- classes allow "non-causal" events, such as interrupts, to be handled.
- various statistical gathering routines, such as histogram and variance classes.

The system also comes with complete examples and test routines which illustrate many of the issues raised in using the simulation package. It is used by many commercial and academic organisations.

The co-routine facility of Simula is implemented by operating system thread packages, such as pthreads. Classes are provided for various random number distributions.

Thanks to all the people who took the time and effort to port C++ to systems and compilers that we do not have access to and reporting the problems (and fixes) that were needed.

Specific thanks for contributions of code and bug reports to:

Peter Lamb peter.lamb@cmis.csiro.au

Ian Mathieson Ian.Mathieson@mel.dit.CSIRO.AU

Sze-Yao Ni nee@axp1.csie.ncu.edu.tw

NOTE: With the move to github and various refactoring it has become difficult to even check that C++SIM builds on all of the platforms we once supported, let alone run successfully. Therefore, at the moment all we can test against is Linux and that's the only platform (with g++ or clang++) that we can support. However, rather than remove the code for all of the other platforms we once supported, such as HPUX, Solaris and other thread packages, we've left them in the codebase for now in case others in the community are able to use them to check that C++SIM can still build and run.

Information about specific releases, along with binary distributions, can be found in the official releases section https://github.com/nmcl/C--SIM/releases

----

You can find more details of how to build and install the system in the README_BUILD directory. A summary is below ...

NOTE: the current system depends heavily on imake. You may need to install this first if it is not normally available.

The build:

./configure

make -f MakefileBoot

make Makefiles all

Then go into Examples and:

make -f MakefileBoot

make Makefiles all

The examples all have expected_output files. Run the examples and compare.

----

NOTE, historical versions of C++SIM can be found in the sandbox repository, e.g., https://github.com/nmcl/sandbox/tree/master/Process
