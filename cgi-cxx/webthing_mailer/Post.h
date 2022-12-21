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

#ifndef POST_H
#define POST_H

/* Class post is really a top-level posting program rather than a class.
   It constructs a Message from CGI (new_message()) or from an existing
   message (forward()).  In either case it will use the user's Preferences
   file to add sender information, and ensure it is traceable.
*/

#include "Message.h"

class post {
  Message message ;
  void report_errors(const String&) const ;
public:
  post() {}
  ~post() ;
  int new_message() ;
  int forward(String& to) ;
} ;
#endif

