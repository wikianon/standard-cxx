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

#include "Config.h"
#include <CGI++/CGI.h>
#include <CGI++/FileStream.h>
#include "Layout.h"
#include <String.h>
#include <iostream.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "Timebuf.h"

extern CGI cgi ;

void Layout::body_netscape() const {
  cout << "<body bgcolor=\"#e8e8e8\" text = \"#0f0f0f\"\
	background=\"http://www.webthing.com/img/background.gif\"
	 link=\"#a82828\" vlink=\"#204080\">" ;
}
void Layout::toolbar() {
  cout	<< "<table width=\"100%\" border=0 cellpadding=6>"
	"<tr><th><a href=\"http://www.webthing.com/\"><img border=0 "
	"alt=\"Web&#222;ing\" height=59 width=241 "
	"src=\"http://www.webthing.com/img/wt3.gif\"></a></th>"
	"</th><td align=center valign=middle><hr noshade>"

"[<a href=/>Front</a>] "
"[<a href=\"http://www.webthing.com/tour/\">Help</a>] "
"[<a href=\"http://www.webthing.com/page.cgi/about\">About</a>] "

"[<a href=\"login.cgi\">Login</a>] "
"[<a href=\"folder.cgi/INTRAY\">Intray</a>] "
"[<a href=\"foldermgr.cgi\">Folders</a>] "
"[<a href=\"compose.cgi\">Compose</a>] "
"[<a href=\"preferences.cgi\">Personal</a>] "
"[<a href=\"template.cgi\">Templates</a>] "
"[<a href=\"addressbook.cgi\">AddressBook</a>] "

"<hr noshade> </td></tr> </table><p>" << endl ;

  _buf = "</body></html>" ;
}

Layout::~Layout() { cout << _buf ; }

Layout::Layout(const char* title) {
  _buf = config.base() ;
  _buf += "<link rev=\"made\" href=\"mailto:webthing@webthing.com\">" ;
  if ( title ) _buf = _buf + "<title>" + title + "</title>" ;
}
void Layout::meta(const char* base) {
  _buf = _buf + "<meta " + base + "\">" ;
}
void Layout::keywords(const char* kw) {
  _buf = _buf
	+ "<meta name=\"Keywords\" http-equiv=\"Keywords\" content=\""
	+ kw
	+ "\">"
  ;
}
void Layout::description(const char* desc) {
  _buf = _buf
	+ "<meta name=\"Description\" content=\""
	+ desc
	+ "\">"
  ;
}
void Layout::link(const char* base) {
  _buf = _buf + "<link " + base + "\">" ;
}
void Layout::print(const String&, const int cache) {
  if ( ( cache == 2 ) && ( cgi.request_method() != "POST" ) ) { // || ( (cache==1)&&(opt.contains("c")) ) ) {
    Timebuf mtime(time(0)) ;
    Timebuf exptime(time(0) + 1209600) ;	// now + 2 weeks
    cout << "Last-modified: " << mtime << "\n"
	"Cache-Control: private\n"
	"Expires: " << exptime << "\n" ;
  }
  cout
	<< "Content-Type: text/html\n\n" ;
  cout << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2//EN\">\n" ;
  if ( _buf != NULL_String )
    cout << "<html><head>" << _buf << "</head>" ;
  body_netscape() ;
    toolbar() ;
}
void Layout::print(const int cache) {
  String opt (cgi.cookie("layout")) ;
  if ( opt == NULL_String )
    opt = "top" ;
  print(opt, cache) ;
}
void Layout::made(const char* by) {
  _buf = _buf + "<link rev=made href=\"mailto:" + by
	+ "@" + config.domain() + "\">" ;
}
