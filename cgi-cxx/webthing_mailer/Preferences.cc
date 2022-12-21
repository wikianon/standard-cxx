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

#include <CGI++/CGI.h>
#include "Config.h"
#include "Layout.h"
#include "Register.h"

extern CGI cgi ;

typedef enum { NEW, READ, UPDATE } prefs_status ;

void initialise() ;
void p_display() ;
void defaults() ;
const prefs_status status(const char* const) ;

void MAIN_preferences() {

  const String& prefsfile = config.preferences() ;

  const prefs_status update = status(prefsfile) ;

  initialise() ;		// setup

  switch ( update ) {
    case NEW: defaults() ; break ;
		// set system defaults
    case READ: cgi.readfrom(prefsfile) ; break ;
		// read what's set by default
    case UPDATE: cgi.form() ; break ;
		// read any changes we've just been given
  }

  p_display() ;		// display the form

  if ( update != READ )
    cgi.saveto(prefsfile) ;	// save whatever's been sent us
  
}
const prefs_status status(const char* const filename) {
  int fd ;
  if ( cgi.request_method() == "POST")
	return UPDATE ;

  if ( fd = open (filename, O_RDONLY) , fd == -1 )
	return NEW ;

  close(fd) ;
  return READ ;
}
void initialise() {
    cgi.define(4,
	"Name:input::48" ,
	"Organization:input:WebThing:48" ,
	"Signature:textarea::4:72" ,
	"Headers:select_multiple"
    ) ;

  form_select_multiple* headers = (form_select_multiple*) cgi["Headers"]() ;
  (*headers)["Cc"] = "Cc" ;
  (*headers)["Organization"] = "Organization" ;
  //(*headers)["Comments"] = "Comments" ;
  (*headers)["Keywords"] = "Keywords" ;
  (*headers)["From"] = "From" ;
  (*headers)["Bcc"] = "Bcc" ;
  (*headers)["Reply-to"] = "Reply-to" ;
}
void defaults() {
  
  form_select_multiple* headers = (form_select_multiple*) cgi["Headers"]() ;
  headers->set("Name") ;
  headers->set("To") ;
  headers->set("Cc") ;
  headers->set("Subject") ;

  cgi["frames"].set("on") ;
  cgi["cookies"].set("on") ;
  cgi["javascript"].set("on") ;

}
void p_display() {
//  cout	<< "Content-type: text/html\n\n" ;
  Layout l("Mail Composition Preferences") ;
  l.print(1) ;
  cout
	<< "<h1>Mail Composition Preferences</h1>"
	"<p>All values entered here will be inserted as default values "
	"in your message composition forms.   The forms permit you to "
	"change the defaults for individual messages.</p>"

	<< cgi.start("preferences.cgi")
	<< "<hr noshade><h3>Personal Data</h3>"
	<< "<table>"
	"<tr><td>Name:</td><td>" << cgi.render("Name") << "</td></tr>"
;cout	<< "<tr><td>Organization:</td><td>"
	<< cgi.render("Organization")
	<< "</td></tr></table>"
	"<b>Signature</b> (please stay within the box)<br>"
	<< cgi.render("Signature")
	<< "<hr noshade><h3>Headers</h3>"
	"<table><tr><td><p>Please select which headers you would like "
	"to be displayed <em>by default</em> when composing new messages. "
	"Note that you can always add or delete headers in the "
	"Composition Form.</p><p>The <b>To</b> and <b>Subject</b> "
	"headers are always displayed, and <b>From</b> and "
	"<b>Organization</b> are set by the system (unless you explicitly "
	"change them).</td>"

	"<td>"
	<< cgi.render("Headers")
	<< "</td></tr>"

	<< "</table><hr noshade>"
	<< cgi.submit("Set Preferences")
	<< cgi.reset("Cancel") << cgi.end() ;
}
Register preferences_cgi("preferences.cgi", MAIN_preferences) ;
