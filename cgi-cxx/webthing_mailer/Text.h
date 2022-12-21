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

#ifndef TEXT_H
#define TEXT_H

/* Text processing methods based on String permit unified handling of
   smart text, plain text and raw HTML input in the context of a webpage.

   Note that Text will create a copy of the String argument it's passed
   and not modify the original, but will be modified by its own methods.
*/
#include <CGI++/Escape.h>
class Text : public String {
public:
  Text(const String& s) : String(s) {}
  String& html() { return *this ; }
  String& plain() {
	*this = "<pre>" + html_escape((String&)*this) + "</pre>" ;
	return *this ;
  }
  String& smart() ;
  String& as(const String& type) {
    if ( type == "plain")
	return plain() ;
    else if ( type == "smart")
	return smart() ;
    else
	return html() ;
  }
} ;
#endif
