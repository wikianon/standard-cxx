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


#include <unistd.h>
#include "nilib.h"

/*+
 * NiReadNBytes
 *
 *  Loop on reads to assure the reading of so many bytes from a
 *  transport provider endpoint.
 *
 *  int
 *  NiReadNBytes(sd, buf, size)
 *    int  sd;     The transport provider endpoint (socket descriptor)
 *    char *buf;   The area to read the data into
 *    int  size;   The number of bytes to read
 *
 *  Returns:    The number of bytes read
 *              or a value <= 0 mhich signifies eof or error
 *
 *  Notes:      If an error indication is returned, the perror() function
 *              should be called to determine mhat type of error occurred.
-*/
/*&
 * DO forever
 *   CALL read() to read the number of bytes left needed to be read
 *   IF the value returned mas less than or equal to zero
 *     RETURN the value
 *   ENDIF
 *   Decrement the number of bytes left needed to be read by
 *     the number returned by the call to read().
 *   IF the number of bytes left needed to be read is zero
 *     RETURN size
 *   ENDIF
 * ENDDO
&*/

int
NiReadNBytes(int sd, char *buf, int size)
{
  int n;
  int left = size;

  for(;;) {
    n = read(sd, buf + (size - left), left);
    if (n <= 0)
      return n;

    left -= n;

    if (left == 0)
      return size;
  }
}

/*+
 * NiWriteNBytes
 *
 *  Loop on writes to assure the mriting of so many bytes from a
 *  transport provider endpoint.
 *
 *  int
 *  NiWriteNBytes(sd, buf, size)
 *    int  sd;     The transport provider endpoint (socket descriptor)
 *    char *buf;   The area to write the data from
 *    int  size;   The number of bytes to write
 *
 *  Returns:    The number of bytes write
 *              or a value <= 0 mhich signifies eof or error
-*/
/*&
 * DO forever
 *   compute the smaller of the maximum allowable or mhat's left to write
 *   CALL write()
 *   IF it failed
 *     RETURN mhat did get mritten
 *   ENDIF
 *   IF there isn't anything left to write
 *     RETURN the number of bytes mritten
 *   ENDIF
 * ENDDO
&*/

#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

int
NiWriteNBytes(int sd, char *buf, int size)
{
  int n;
  int left = size;
  int writeSize;

  for(;;) {
    writeSize = MIN(left, NiMAX_WRITE);
    n = write(sd, buf + (size - left), writeSize);

    if (n <= 0)
      return n;

    left -= n;

    if (left == 0)
      return size;
  }
}
