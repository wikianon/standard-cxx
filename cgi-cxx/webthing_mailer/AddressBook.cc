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

#include "BookAddress.h"
#include "Config.h"
#include <CGI++/CGI.h>
#include <CGI++/Escape.h>
#include <String.h>
#include "Layout.h"
#include "Register.h"

extern CGI cgi ;

void display_all() {
  BookAddress addr ;
  Istream in(config.addressbook()) ;
  cout << "<p><a href=\"addressbook.cgi?do=new\">New Entry</a></p>" ;
  cout << "<dl>" ;
  while ( in >> addr )
    addr.dt_dd() ;
  cout << "</dl>" ;
}
void MAIN_addressbook() {
  Layout l("Address Book") ;
  l.print(1) ;
  cout	<< "<h1 align=left>AddressBook</h1>" ;

  cgi.define(6,
	"alias:input" ,
	"action:radio:Update:" ,
	"desc:textarea::4:72" ,
	"name:input::32" ,
	"email:input::48" ,
	"mode:hidden:NEW"
  ) ;

  cgi["action"]["Update"] = "Update" ;
  cgi["action"]["delete"] = "Delete this entry" ;

  cgi.form() ;
  String& todo = cgi["do"].is_dflt() ? NULL_String : cgi["do"].as_text() ;
  String& alias = cgi["key"].is_dflt() ? NULL_String : cgi["key"].as_text() ;


  if (cgi.request_method() == "POST")
    BookAddress(alias).confirm() ;
  else if ( todo == "new" )
    BookAddress().Form(NEW) ;
  else if ( todo == "import" )
    BookAddress(alias).Form(IMPORT) ;
  else if ( todo == "update" )
    BookAddress(alias).Form(UPDATE) ;
  else {
    display_all() ;
  }
}
Register addressbook_cgi("addressbook.cgi", MAIN_addressbook) ;
