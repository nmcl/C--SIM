The following classes are defined:

    Process	- An abstract class that exports the major functions
		  of the Simula class, process.  To use, derive your own
		  class from the class Process.  The pure virtual function,
		  Body, is the "main" procedure of the class.  Note that,
		  like Simula, a process is not scheduled to run when it is
		  created.  It must be explicitly 'activated'.

    ProcessList	- A list class for processes that (by default) orders
		  the elements by event time.

    ProcessIterator - An iterator class for ProcessList.

    ProcessCons - Allows LISP-like list manipulation (car & cdr).

    Random - A series of classes which provide various random number
	     streams.

    thread - The basic thread class, which defines what operations other
	     threads packages must provide. This is essentially a template
	     which allows other thread packages to be used as long as they
	     provide at least the operations necessary for this class.

    lwp_thread - This is the SunOS threads class. [If you have problems with lwp.h
		 then make a local copy of it and remove the definitions of lwp_create
		 etc. from that copy - then edit this source to use your lwp.h].
		 
    solaris_thread - This is the Solaris threads implementation.

    gnu_thread - This is the interface to Gnu's thread package.

    cpp_task - This is the interface to the C++ Task library.
    
    c_thread - This is the C threads implementation.
    
