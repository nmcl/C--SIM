/*
 * Copyright (C) 1993, 1994, 1995, 1996
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id$
 */

/*
 *  Configure.h
 */

#ifndef CONFIGURE_H_
#  define CONFIGURE_H_
#include <Config/m_unknown.cf>
#include <Config/v_unknown.cf>
#include <Config/s_linux_2_0.cf>
#include <Config/c_gcc_4_0.cf>

#define OSNAME Linux
#define OSRELEASE 4.2.7-200.fc22.i686
#define OSMAJOR 4
#define OSMINOR 2
#define MACHINE_HW "i686"


#define CPLUSPLUS_CROSS_PROGRAM "g++"

#ifndef YES
#define YES 1
#endif

#ifndef NO
#define NO  0
#endif

/*
 *  Make and Source configuration
 */

#ifndef VERBOSE_MAKE
#define VERBOSE_MAKE NO
#endif

#ifndef TOPDIR_DEFAULT
#define TOPDIR_DEFAULT /home/mlittle/Desktop/C--SIM
#endif

#ifndef PURIFY_PROGRAM
#define PURIFY_PROGRAM  "exec"
#endif

#ifndef CPLUSPLUS_PROGRAM
#define CPLUSPLUS_PROGRAM  "g++"
#endif

#ifndef CPLUSPLUS_INCLUDE_DEFAULT
#define CPLUSPLUS_INCLUDE_DEFAULT /usr/include/c++/5.3.1
#endif

#ifndef CPPFLAGS_DEFAULT
#define CPPFLAGS_DEFAULT 
#endif

#ifndef CPLUSPLUSFLAGS_DEFAULT
#define CPLUSPLUSFLAGS_DEFAULT -g
#endif

#ifndef LDFLAGS_DEFAULT
#define LDFLAGS_DEFAULT  -g
#endif

#ifndef xLIBRARIES_DEFAULT
#define xLIBRARIES_DEFAULT  -lm
#endif

#ifndef BUILDFLAGS_DEFAULT
#define BUILDFLAGS_DEFAULT  -DProcessList_Queue
#endif

/*
 *  Thread system details
 */


/*
 *  Scheduler queue details
 */

#define LINEAR 1
#define HEAP 2
#define CALENDAR 3
#define HASHED 4

#ifndef Queue_Type
#define Queue_Type ProcessList
#endif

/*
 *  Thread system details
 */

#ifndef THREAD_TYPE
#define THREAD_TYPE Posix_Thread
#endif

#ifndef THREAD_CPPFLAGS_DEFAULT
#define THREAD_CPPFLAGS_DEFAULT  -DPTHREAD_DRAFT_LINUX -DPOSIX_THREAD
#endif

#ifndef COMPILER_THREAD_FLAGS_DEFAULT
#define COMPILER_THREAD_FLAGS_DEFAULT 
#endif

#ifndef THREAD_LDFLAGS_DEFAULT
#define THREAD_LDFLAGS_DEFAULT 
#endif

#ifndef THREAD_LDLIBS_DEFAULT
#define THREAD_LDLIBS_DEFAULT  -lpthread
#endif

/*
 *  Installation details
 */

#ifndef INSTALL_TOPDIR_DEFAULT
#define INSTALL_TOPDIR_DEFAULT /usr/local/C++SIM
#endif

#endif
