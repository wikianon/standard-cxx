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


#ifndef _NILIB_H_
#define _NILIB_H_

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/uio.h>

#ifndef _SOLARIS
typedef struct iovec iovec_t;
#endif

#if defined(_DARWIN)
#define socklen_t int
#endif

/* tell the server not to fork */
#define NiNOFORK              1

/* success and failure codes */
#define NiSUCCESS             1
#define NiFAILURE             0

/* possible values of NiErrno */
#define NiNOT_INITIALIZED     -1
#define NiSOCKET_ERROR        -2
#define NiBIND_ERROR          -3
#define NiLISTEN_ERROR        -4
#define NiHOSTNAME_ERROR      -5
#define NiHOSTBYNAME_ERROR    -6
#define NiFORK_ERROR          -7
#define NiCONNECT_ERROR       -8
#define NiSERVBYNAME_ERROR    -9
#define NiFCNTL_ERROR         -10

/* maximum length to be written in one shot */
#define NiMAX_WRITE   4096

/* a value not provided by the include file */
#define FD_NULL       ((fd_set *) 0)

/* a network context */
typedef struct {
  int                ncSocket;
  int                ncAcceptSocket;
  void             (*ncCallback)(int sd);
  struct sockaddr_in ncSocketAddress;
} NiNetworkContext;

extern NiNetworkContext *NiInitialize(void);

extern char *NiHostName(void);
extern char *NiPeerHost(int sd);
extern void  NiError(const char *preamble);
extern char *NiStringError(const char *preamble);
extern void  NiTerminate(NiNetworkContext *nch);
extern int   NiBindService(NiNetworkContext *nch, int port, int count);
extern int   NiAcceptConnection(NiNetworkContext *nch);
extern int   NiSpawnServer(NiNetworkContext *nch, int flags);
extern int   NiServerInit(char *progName, int port, int count, int flags);
extern int   NiServerMain(int argc, char *argv[], int port, int count,
                          void (*func)(int sd), int flags);
extern int   NiConnectToService(NiNetworkContext *nch, char *node, int port);
extern int   NiClientInit(char *progName, char *node, int port);
extern int   NiClientMain(int argc, char *argv[], char *node,
                          int port, void (*func)(int sd));
extern short NiPortNumber(int sd);
extern int   NiPing(const char *node);
extern int   NiGetportbyname(const char *name);
extern int   NiWritevNBytes(int sd, iovec_t *v, int cnt);
extern int   NiReadNBytes(int sd, char *buf, int size);
extern int   NiWriteNBytes(int sd, char *buf, int size);

extern char *NiProgramName;
extern int   NiSocket;
extern int   NiErrno;

#endif
