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
#include "Message.h"
#include "Layout.h"
#include "Address.h"
#include "Register.h"
#include "Post.h"
#include "Text.h"
#include "Timebuf.h"
#include <CGI++/CGI.h>
#include <CGI++/Escape.h>
#include <CGI++/FileStream.h>
#include <sys/stat.h>

extern CGI cgi ;

/* Start by reading prefs, with user's defaults
   Then read msg, and overwrite things like From/Org if appropriate
*/

void MAIN_post() {
  class post p ;
  p.new_message() ;
}
void MAIN_forward() {
  cgi.form() ;
  if (cgi["do"].as_text() == "forward") {
    post p ;
    p.forward(cgi["To"].as_text()) ;
  }
  // export to desktop deleted
}
void post::report_errors(const String& err) const {
  cout << "Please fix the errors:<br><pre>" << html_escape(err) << "</pre>" ;
}
int post::new_message() {
  static Layout l("Sending Mail") ;
  l.print(1) ;
  cgi.readfrom(config.preferences()) ;
  String real_from(cgi["Name"].as_text()) ;
  String if_unset_org(cgi["Organization"].as_text()) ;
  if ( real_from == NULL_String )
    real_from = cgi.remote_user() + '@' + config.domain() ;
  else
    real_from = real_from + " <" + cgi.remote_user()
	+ '@' + config.domain() + '>' ;

  cgi.define(3,
	"Attach:longfile",
	"AttachType:select",
	"Body:textarea"
  ) ;
  cgi.form() ;
  message.read(cgi["my_ref"].as_text()) ;

  // parse the headers supplied to us
  // add from and org if not supplied
  // check To and Subj
  Map& headers = message.parse_headers(cgi["Head"].as_text()) ;

  String hh("To") ;
  String s(cgi[hh].as_text()) ;		// strip down to bare addresses
  if (s != NULL_String) {
    int n(s.freq(',')+1) ;
    if ( n > 1 ) {
      Regex comma("[ \t]*,[ \t]*") ;
      String* ss = new String[n] ;
      split(s,ss,n,comma) ;
      while (--n >= 0) {
        s.at(ss[n]) = Address(ss[n]).email() ;
      }
      headers[hh] = s ;
      delete [] ss ;
    } else
      headers[hh] = Address(s).email() ;
  } else if (headers[hh] == NULL_String)
    message.add_error("No recipient specified!\n") ;

  hh = "Subject" ;
  if (cgi[hh].as_text() != NULL_String)
    headers[hh] = cgi[hh].as_text() ;
  else if (headers[hh] == NULL_String)
    message.add_error("No subject specified!\n") ;

  hh = "From" ;
  String hs = "Sender" ;
  if (headers[hh] == NULL_String)
    headers[hh] = real_from ;
  else if (headers[hs] == NULL_String)
    headers[hs] = real_from ;
  else {
    hh = "X-Authenticated-Sender" ;
    headers[hh] = real_from ;
  }

  hh = "Organization" ;
  if (headers[hh] == NULL_String)
    headers[hh] = if_unset_org ;

  hh = "Message-Id" ;
  hs = "<" ;
  hs += cgi["my_ref"].as_text() + "@" + cgi.server_name() + ">" ;
  headers[hh] = hs ;

  if (strlen (cgi["Attach"].as_longtext()) < 2)
    message.body(cgi["Body"].as_text()) ;
  else {
    message.start_mime() ;
    message.add_mime(cgi["Body"].as_text()) ;
    message.add_mime(cgi["Attach"], cgi["AttachType"].as_text()) ;
    message.end_mime() ;
  }
  return 0 ;
}
int post::forward(String& to) {
  static Layout l("Forwarding Mail") ;
  l.print(1) ;

  cgi.readfrom(config.preferences()) ;
  String real_from(cgi["Name"].as_text()) ;
  if ( real_from == NULL_String )
    real_from = cgi.remote_user() + '@' + config.domain() ;
  else
    real_from = real_from + " <" + cgi.remote_user()
	+ '@' + config.domain() + '>' ;

  message.read(www_unescape(cgi["id"].as_text())) ;

  message.header("X-Forwarded-By") = real_from ;
  message.header("X-Original-To") = message.header("To") ;

  int n(to.freq(',')+1) ;
  if ( n > 1 ) {
    Regex comma("[ \t]*,[ \t]*") ;
    String* ss = new String[n] ;
    split(to,ss,n,comma) ;
    while (--n >= 0) {
      to.at(ss[n]) = Address(ss[n]).email() ;
    }
    message.header("To") = Address(to).email() ;
    delete [] ss ;
  } else
    message.header("To") = Address(to).email() ;

  message.id().at("@") = ".forward@" ;

  if ( cgi["what"].as_int() == 1 ) {
    String s("(message body deleted)") ;
    message.body(s) ;
  }

  return 0 ;
}
post::~post() {
  if ( message.ok() )
    message.send() ;
  else
    report_errors(message.errormsg()) ;
}
Register post_cgi("post.cgi", MAIN_post) ;
Register forward_cgi("forward.cgi", MAIN_forward) ;
