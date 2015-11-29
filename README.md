C++SIM has been available since 1990 and is an object-oriented simulation package. It provides discrete event process-based simulation similar to SIMULA's simulation class and libraries. A complete list of the facilities provided follows:

- the core of the system gives SIMULA-like simulation routines, random number generators, queueing algorithms, and thread package interfaces.
- entity and set manipulation facilities similar to SIMSET.
- classes allow "non-causal" events, such as interrupts, to be handled.
- various statistical gathering routines, such as histogram and variance classes.
- debugging classes.

The system also comes with complete examples and test routines which illustrate many of the issues raised in using the simulation package. It is used by many commercial and academic organisations.

NOTE Currently we only have definite support for Linux due to limitations on testing hardware. If any community members would like to see if they can confirm the old HPUX, Solaris, Windows etc. still work and/or provide fixes, then go to it.

----

The build:

./configure

make -f MakefileBoot
make Makefiles
make

Then go into Examples and:

make -f MakefileBoot
make Makefiles
make

The examples all have expected_output files. Run the examples and compare.
