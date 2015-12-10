1.	Installation

Note: you will need a version of imake  to  build this version  of the
system.

Installation  of  C++SIM  should be  a  relatively  painless operation
accomplished in the following way:

a)
	Run the  'configure' script  and  answer all of  the  question
asked. Most questions have default  answers that you can accept simply
by pressing 'return'. If you do not want the default answer but do not
want to supply  another,   then  type  'none'.   See below  for   more
information on configure.

b)
	Type 'make -f MakefileBoot' to build the root level makefile.

c)
	Type 'make  Makefiles'  to  generate  makefiles throughout the
source tree.

d)
	Type 'make depend'  to install appropriate  dependencies  into
the newly generated makefiles.

e)
	Type 'make all' to build the system.

f)
	Type 'make install'  to install the public headers,  libraries
etc.

	If any of these steps fail check the section on known problems
for potential remedies.

	NOTE:  The programs in  the Examples and Tests directories are
not built by  default. Go into these and  type make -f MakefileBoot as
well, and follow similar steps to above.


2.	Configuration Specifics

	Here is a walkthough of the questions asked by the 'configure'
script togethor with an explaination of the required answers. Default
answers in square brackets.

	NOTE:   The  configuration  system  attempts to  automatically
generate the  code for make  to find the   necessary libraries etc. If
these are not correct  for your system  then look in the corresponding
file in  Include/Config.  Also, Configure.h contains  the  information
obtained from running configure, and if this file  exists in the local
directory or in  the installed directory, the  answers  from this file
will be  given as the   defaults  for subsequent  runs (the  installed
Configure.h is used in preference to a local version).

1)	Make is verbose (NO/YES) [NO]

	Controls how verbose the make process is. Accept the default
which is probably far too verbose as it is!

2)	Make is fast (no inter module cross checks) (NO/YES) [NO]

	Controls whether the  generated  makefiles  check  that  other
parts of the source  tree are up to  date with respect to the  current
directory. This  slows  the  make  process  down considerably but  the
default  provides  maximum safety. 

3)	Root  directory  of  source  tree [<path>]

	The  default value is  the  directory from which configure  is
run. This does not need changing.

4)	C++  compiler [CC]

	Which C++ compiler  to run to compile  the  system.

5)	C++ include directory [/usr/include/CC]

	The directory containg your C++ header files.

6)	Thread type to use [LWP_Thread]

	The type of threads package you wish to use.

7)	Scheduler queue type [ProcessList]

	The queuing implementation which the scheduler will use.

8)	C++SIM flags []

	These  flags  (actually  cpp  defines)  govern  parts  of  the
compilation of  C++SIM.  You may set  -DDEBUG.   Setting  -DNO_INLINES
causes all of the inline  code in C++SIM to be non-inlined (i.e., real
functions  are  generated  and called),  This  option  is  useful  for
debugging purposes.

9)	Additional compilation flags for optimisation and/or debugging []

	Set -g or -O or whatever here.

10)	Additional loader flags []

	Set -g or -s or -L etc. flags here.

11)	Specific libraries [-L/usr/lib -lm]

	This  will include the location of the threads library and the
maths library at least.

12)	Root of directory tree for installation [/usr/local/C++SIM] 

	The root of the tree at which C++SIM will be installed.
