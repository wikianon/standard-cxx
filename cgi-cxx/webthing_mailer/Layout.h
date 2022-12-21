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

#ifndef LAYOUT_H
#define LAYOUT_H
#include <String.h>

/* The global look-and-feel of HTML pages.  This has been rather brutally
   chopped, and only supports one layout (and is less-than clean)

   Order is important:
   (1)	Anything that goes in the HTML page's <HEAD> must be added
	after the constructor but before calling print()
   (2)	You must call print() before sending anything to cout.
   (3)	You must keep the Layout object in scope until you have finished
	all HTML body output (the destructor closes the page).
*/

class Layout {
  String _buf ;
  void body_netscape() const ;
  void toolbar() ;
public:
/* Constructor: title is of course the HTML <TITLE> element */
  Layout(const char* title) ;
  ~Layout() ;

/* print must be called AFTER other calls on the Layout object, but
   BEFORE writing anything else to cout.

   The arguments are rather shorn of their functionality in this
   distribution: "opt" is the layout option (only one supplied here)
   which will be read from the user's cookie if not supplied.
   cache controls output cacheing as follows:
	0 - no cache control
	1 - (same as 0 here)
	2 - private cache for GET (expiry 2 weeks)
   Cache-control: private ensures that pages are NOT cached by proxies;
   only by the end-user's browser (mail is, after all, private).
*/
  void print(const int cache) ;
  void print(const String& opt, const int cache) ;

/* Call any of these before print() to insert HTML HEAD elements */
  void meta(const char*) ;
  void made(const char*) ;
  void link(const char*) ;
  void keywords(const char*) ;
  void description(const char*) ;

} ;

#endif
