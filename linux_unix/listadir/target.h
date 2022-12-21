/* 
 *  ptarget.h
 *
 *  Coded by Stefan Vogt, revised Feb 18, 2011.
 *  Released under the FreeBSD license.
 *  http://www.byteproject.net
 *
 *  header for compiler and OS detection
 *
 */

#if !defined(_PTARGET_H_)
	#define _PTARGET_H_
#endif

/* compilers */
#if defined(__GNUC__)
	#if defined(__llvm__)
		#if defined(__clang__)
			#define COMPILER "LLVM Clang (GNU C %d.%d.%d)"
		#elif defined(__APPLE_CC__)
			#define COMPILER "LLVM Apple GCC %d.%d.%d"
		#else
			#define COMPILER "LLVM GCC %d.%d.%d"
		#endif
	#elif defined(__APPLE_CC__)
		#define COMPILER "Apple GCC %d.%d.%d"
	#elif defined(__MINGW32__) || defined(__MINGW64__)
		#define COMPILER "MinGW GCC %d.%d.%d"
	#else
		#define COMPILER "GCC %d.%d.%d"
	#endif
	#define COMP_VERSION __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__
#endif

#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
	#if defined(__SUNPRO_C)
		#define COMPILER "Sun C compiler %x"
		#define COMP_VERSION __SUNPRO_C
	#endif
	#if defined(__SUNPRO_CC)
		#define COMPILER "Sun C++ compiler %x"
		#define COMP_VERSION __SUNPRO_CC
	#endif
#endif

#if defined(__INTEL_COMPILER)
	#define COMPILER "Intel compiler %d.%d"
	#define COMP_VERSION __INTEL_COMPILER / 100, (__INTEL_COMPILER % 100) / 10
#endif

#if defined(_MSC_VER)
	#define COMPILER "Microsoft C / Visual C++ compiler %d.%d"
	#define COMP_VERSION _MSC_VER / 100, _MSC_VER % 100
#endif

#if defined(__WATCOMC__)
	#define COMPILER "(Open) Watcom C / C++ compiler %d.%d"
	#define COMP_VERSION __WATCOMC__ / 100, __WATCOMC__ % 100
#endif

#if defined(__DMC__)
	#define COMPILER "Digital Mars C / C++ compiler %x"
	#define	COMP_VERSION __DMC__
#endif

#if defined(__PCC__)
	#define COMPILER "Portable C compiler"
#endif

#if defined(__HP_cc) || defined(__HP_aCC)
	#if defined(__HP_cc)
		#define COMPILER "HP ANSI C compiler"
	#endif
	#if defined(__HP_aCC)
		#define COMPILER "HP aC++ compiler"
	#endif
#endif

#if defined(__BORLANDC__)
	#define COMPILER "Borland / Embarcadero C++ compiler"
#endif

#if defined(KENC) || defined(KENCC)
	#define COMPILER "Plan 9 compiler suite"
#endif

#if defined(__sgi) || defined(sgi)
	#define COMPILER "MIPSpro compiler"
#endif

#if defined(__MWERKS__)
	#define COMPILER "Metrowerks CodeWarrior"
#endif

#if defined(__TURBOC__) && !defined(__BORLANDC__)
	#define COMPILER "Borland Turbo C"
#endif

#if defined(__QC)
	#define COMPILER "Microsoft Quick C"
#endif

#if defined(__ZTC__) && !defined (__SC__) && !defined(__DMC__)
	#define COMPILER "Zortech C / C++ compiler"
#endif

#if defined(__SC__) && !defined(__DMC__)
	#define COMPILER "Symantec C / C++ compiler"
#endif

/* operating systems */
#if defined(__APPLE__) && defined(__MACH__)
	#define OPERATINGSYSTEM "Mac OS X"
#endif

#if defined(__FreeBSD__)
	#define OPERATINGSYSTEM "FreeBSD"
#endif

#if defined(__NetBSD__)
	#define OPERATINGSYSTEM "NetBSD"
#endif

#if defined(__OpenBSD__)
	#define OPERATINGSYSTEM "OpenBSD"
#endif

#if defined(__DragonFly__)
	#define OPERATINGSYSTEM "DragonFly BSD"
#endif

#if defined(__linux) || defined(linux)
	#define OPERATINGSYSTEM "Linux"
#endif

#if defined(sun) || defined(__sun)
	#if defined(__SVR4) || defined(__svr4__)
		#define OPERATINGSYSTEM "Solaris"
	#else
		#define	OPERATINGSYSTEM "SunOS"
	#endif
#endif

#if defined(__HAIKU__)
	#define OPERATINGSYSTEM "Haiku"
#endif

#if defined(__WIN32__) || defined(__WINDOWS__) || defined(_MSC_VER) || \
defined (_WIN32) || defined(_WIN64) || defined(_WINDOWS)
	#define OPERATINGSYSTEM "Microsoft(R) Windows(TM)"
#endif

#if defined(__REACTOS__)
	#define OPERATINGSYSTEM "ReactOS"
#endif

#if defined(__GNU__)
	#define OPERATINGSYSTEM "GNU Hurd"
#endif

#if defined(_AIX)
	#define OPERATINGSYSTEM "AIX"
#endif

#if defined(__hpux) || defined(hpux)
	#define OPERATINGSYSTEM "HP-UX"
#endif

#if defined(__minix)
	#define OPERATINGSYSTEM "MINIX"
#endif

#if defined(PLAN9)
	#define OPERATINGSYSTEM "Plan 9"
#endif

#if defined(__QNXNTO__)
	#define OPERATINGSYSTEM "QNX Neutrino"
#endif

#if defined(__QNX__)
	#define OPERATINGSYSTEM "QNX"
#endif

#if defined(_MIPS_ISA) || defined(_SGI_SOURCE)
	#define OPERATINGSYSTEM "IRIX"
#endif

#if defined(__BEOS__)
	#define OPERATINGSYSTEM "BeOS"
#endif

#if defined(UNIXV)
	#define OPERATINGSYSTEM "UNIX - AT&T System V"
#endif

#if defined(UNIX7)
	#define OPERATINGSYSTEM "UNIX - version 7"
#endif

#if defined(NEXTSTEP)
	#define OPERATINGSYSTEM "NeXTStep"
#endif

#if defined(OPENSTEP)
	#define OPERATINGSYSTEM "OPENSTEP"
#endif

#if defined(macintosh)
	#define OPERATINGSYSTEM "Mac OS Classic"
#endif

#if defined(RHAPSODY) && !defined(MAC_OS_X_SERVER)
	#define OPERATINGSYSTEM "Rhapsody (pre Mac OS X)"
#endif

#if defined(__OS2__) || defined(OS2) || defined(_OS2)
	#define OPERATINGSYSTEM "OS2"
#endif

#if defined(AMIGA) || defined(__amigaos__)
	#define OPERATINGSYSTEM "AmigaOS"
#endif

#if defined(__MSDOS__) || defined(__DOS__) || defined(FREEDOS)
	#define OPERATINGSYSTEM "MS-DOS or FreeDOS"
#endif

/* recognition failure */
#if !defined(COMPILER)
	#define COMPILER "unknown compiler"
#endif

#if !defined(OPERATINGSYSTEM)
	#define OPERATINGSYSTEM "unknown operating system"
#endif 
