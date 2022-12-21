/*
WebThing Source code: copyright WebThing Ltd, 1997.

All distribution, modification and use of this software is subject
to the terms and conditions described in the accompanying LICENSE file,
which also tells you how to obtain SUPPORT and WARRANTY.

The software as distributed carries NO WARRANTY.  Please see file
COPYING for details.

Further information and updates are available from
<URL:http://www.webthing.com/software/>
*/

#include "MessageBody.h"
#include <String.h>
#include <string.h>

MessageBody::MessageBody(const char* x) : as_buf(0) , partbuf(0)
	{ as_str << x ; }

const char* MessageBody::as_text() {
  if ( !as_buf ) {
    as_str << ends ;
    as_buf = as_str.str() ;
  }
  return as_buf ;
}
char* MessageBody::part(unsigned int n, const Regex& boundary) {
  as_text();	// make sure text buf is initialised
  
  int mlen, offs ;
  for ( offs=0; n; --n) {
    if (offs = boundary.search(as_buf, length()-offs, mlen, offs), offs > 0)
      offs += mlen ;

    else		// no such part - return whole thing
      return as_buf ;
  }
  offs = Regex("\n[ \t\r]*\n").search(as_buf, length()-offs, mlen, offs) ;
  if (offs > 0)
    offs += mlen ;
  else
    return as_buf ;

  int partlen = boundary.search(as_buf, length()-offs, mlen, offs) - offs ;
  if ( partlen <= 0)
    return as_buf ;

  if (partbuf)
    delete [] partbuf ;
  partbuf = new char[partlen] ;
  strncpy(partbuf, as_buf+offs, partlen) ;
  partbuf[partlen-1] = 0 ;
  return partbuf ;
}
