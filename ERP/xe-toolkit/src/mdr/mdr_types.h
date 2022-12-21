/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the mdr library of Captivity.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE-GPL.txt contained within the
** same package as this file. This software is subject to a
** dual-licensing mechanism, the details of which are outlined in
** file LICENSE-DUAL.txt, also contained within this package. Be sure
** to use the correct license for your needs. To view the commercial
** license, read LICENSE-COMMERCIAL.txt also contained within this
** package.
**
** If you do not have access to these files or are unsure which license
** is appropriate for your use, please contact the sales department at
** sales@captivemetrics.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/


#ifndef _MDR_TYPES_H_
#define _MDR_TYPES_H_

#if defined(_LINUX)
# include <sys/types.h>
typedef unsigned int uint_t;
typedef unsigned long ulong_t;
# ifdef _LP64
typedef unsigned long uint64_t;
# else
typedef unsigned long long uint64_t;
# endif
# if defined(_LINUX)
typedef int int32_t;
typedef unsigned int uint32_t;
# endif
#endif

#if defined(_DARWIN)
# include <sys/types.h>
typedef unsigned char uchar_t;
typedef unsigned short ushort_t;
typedef unsigned int uint_t;
typedef unsigned long ulong_t;
typedef long long longlong_t;
typedef unsigned long long u_longlong_t;
typedef unsigned long long uint64_t;
#endif

#if defined(_FREEBSD)
typedef unsigned char uchar_t;
typedef unsigned short ushort_t;
typedef unsigned int uint_t;
typedef unsigned long ulong_t;
typedef long long longlong_t;
typedef unsigned long long u_longlong_t;
typedef unsigned long long uint64_t;
#endif

#if defined _AIX
# include <sys/types.h>
#endif

#endif
