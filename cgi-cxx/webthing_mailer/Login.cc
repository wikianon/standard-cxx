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
#include "Layout.h"
#include "Register.h"

void MAIN_login() {
  CGI cgi ;

  cgi.set_template("uid:input::12") ;
  cgi.form() ;

  String& req_user = cgi["uid"].as_text() ;

  if ( req_user == NULL_String || req_user == cgi.remote_user() ) {
    Layout l("Web&#222;ing Login") ;
    l.print(2) ;
    cout <<
	"<h1>Login</h1>"
"<p>This form is not required to login to Web&#222;ing: as soon as you "
"select a restricted function, your browser will present you the usual "
"username/password dialogue.   Indeed, you won't even see this page until "
"you are logged in.</p>"
"<h3>Re-Login</h3>"
"<p>If you need to <em>re-login</em> - for example to change "
"between a personal and a group userid, enter the <em>new userid</em> "
"in the box below.   Your browser will prompt you for the new username "
"and password.</p>"
"<h3>Logout</h3>"
"<p>If you are using Web&#222;ing from a public place - such as an Internet "
"Caf&eacute; or Public Library, you obviously don't want to leave yourself "
"logged in.   To logout, simply change your userid to <em>guest</em> "
"with password <em>guest</em>.</p>"
"<p><strong>Note</strong> some browsers will give you the <em>old</em> username "
"in the dialogue box: if this happens, you'll have to change it.</p>"
;

    cgi["uid"].set(cgi.remote_user()) ;
    cout << cgi.start("login.cgi")
	<< "Userid: " << cgi.render("uid") << cgi.submit()
	<< cgi.end() ;
  } else
    cgi.authenticate("Basic", "WebThing") ; //, ErrorDoc) ;
}
Register login_cgi("login.cgi",MAIN_login) ;
