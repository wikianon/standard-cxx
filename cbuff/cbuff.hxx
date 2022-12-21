#ifndef _CBUFF_HXX
#define _CBUFF_HXX










#ifdef __cplusplus

extern "C" void cbuff(void);

#else

void cbuff(void);

#endif  //__cplusplus

#if defined(__linux__) || defined(__gnu_linux__)

 #include <stdio_ext.h>
 
#elif defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__) || \
defined(__unix__) || defined(__sun) || defined(__FreeBSD__) || \
defined(__OpenBSD__) || defined(__APPLE__) || defined(__hpux) || \
defined(__osf__) || defined(__sgi) || defined(_AIX) || defined(__QNX__)

 #include <stdio.h>

#endif


/*
__linux__       Defined on Linux
__unix__        Defined on unix
__sun           Defined on Solaris
__FreeBSD__     Defined on FreeBSD
__NetBSD__      Defined on NetBSD
__OpenBSD__     Defined on OpenBSD
__APPLE__       Defined on Mac OS X
__hpux          Defined on HP-UX
__osf__         Defined on Tru64 UNIX (formerly DEC OSF1)
__sgi           Defined on Irix
_AIX            Defined on AIX
*/






#endif  //_CBUFF_HXX