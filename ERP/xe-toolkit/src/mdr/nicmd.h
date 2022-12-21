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


#ifndef _NICMDH_
#define _NICMDH_

#include <map>
#include <string>

/* length of initial packet containing ascii length */
#define NiPKT_LEN            12

/* maximum length of response string */
#define NiMAX_RESPONSE        16

/* some type of indication that the 2BIG is on the server side */
#define NiRETURN_2BIG        -11

/* is the rpc a function ? */
#define NiPROC                0
#define NiFUNC                1

/* possible last parameter to Nisend_cmd */
#define IOV_NULL               ((iovec_t *) 0)

typedef struct {
  const char *cmdCmd;
  int       (*cmdFunc)(int sd, char *vec[], iovec_t *arg,
                       iovec_t *result, const void *cmd_data);
  char        cmdReturn;
  const void *cmdData;
} NiCmdStruct;

typedef std::map<std::string, NiCmdStruct *> NiCmdMap;
typedef NiCmdMap::iterator                   NiCmdNode;

extern void NiCompileTable(NiCmdStruct *cmds, NiCmdMap &cmd_map, int count);
extern int  NiRecvCmd(int sd, NiCmdMap &cmd_map);
extern int  NiSendCmd(int sd, char *cmd, iovec_t *data,
                      iovec_t *retValue, int function);

#endif
