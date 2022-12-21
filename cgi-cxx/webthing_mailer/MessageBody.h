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

#ifndef MESSAGEBODY_H
#define MESSAGEBODY_H

/* MessageBody is a buffer for mail message body text (plain or MIME). */

#include <strstream.h>
#include <String.h>

class MessageBody {
  char* as_buf ;
  char* partbuf ;
  ostrstream as_str ;
public:
  MessageBody(const char* ="") ;
  ~MessageBody() { if (partbuf) delete [] partbuf ; }
  const int length()
	{ return strlen(as_text()) ; }

  char* part(unsigned int n, const Regex& boundary) ;

/* append ends to as_str and return its contents */
  const char* as_text() ;

/* want ostrstream behaviour for char* buf */
  friend MessageBody& operator<<(MessageBody& m, const char* c)
	{ m.as_str << c ; return m ; }

/* output the whole thing as a char* buf */
  friend ostream& operator<<(ostream& o, MessageBody& m)
	{ return o << m.as_text() ; }
} ;

#endif
