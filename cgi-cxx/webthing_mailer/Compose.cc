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
#include <CGI++/Escape.h>
#include "Config.h"
#include "Message.h"
#include "Register.h"
#include "Layout.h"

extern CGI cgi ;
  char _use_from, _use_org ;
/*
class Composer {
public:
  Composer(const String) ;
  void display() const ;
} ;
*/
void Composer(const String name) {

    cgi.define(14,
	"Name:input::48" ,
	"Organization:input:WebThing:48" ,
	"Signature:textarea::4:72" ,
	"Headers:select_multiple" ,
	"In-reply-to:hidden" ,
	"my_ref:hidden" ,
	"To:input::48" ,
	"Subject:input::48" ,
	"Attach:file" ,
	"BASE64:select::" ,
	"AttachType:select:attachment" ,
	"Head:textarea::4:48" ,
	"Body:textarea::25:72" ,
	"From:hidden"
    ) ;
  cgi["BASE64"][""] = "Text" ;
  cgi["BASE64"]["bin"] = "Binary" ;
  cgi["AttachType"]["inline"] = "Attach inline" ;
  cgi["AttachType"]["attachment"] = "Attachment" ;
  cgi.readfrom(config.preferences()) ;
  cgi.form() ;

    const String ref_id(www_unescape(cgi["id"].as_text())) ;
    Message ref(ref_id) ;
  //String ref_id(cgi.cookie("id")) ;
  if ( name == "reply.cgi" ) {
    ref.read() ;
    cgi["In-reply-to"].set(html_escape(ref_id)) ;
    String subj(ref.header("Subject")) ;
    Regex is_re("[ \t]*[Rr][Ee]:[ \t]") ;
    if ( subj.index(is_re) )
      subj.prepend("Re: ") ;
    cgi["Subject"].set(subj) ;
    cgi["To"].set(html_escape(ref.reply_address())) ;
    String& templ = cgi["templ"].as_text() ;
    if ( templ != "-" ) {
      cgi.readfrom(config.template_file(templ)) ;
      cgi["Body"].set(cgi["text"].as_text()) ;
      cgi["Body"].add("\n") ;
    }
    if ( cgi["qq"].as_text() == "yes" )
	cgi["Body"].add(ref.quote()) ;
  }
  String& sig = cgi["Signature"].as_text() ;
  if (sig != NULL_String ) {
	sig.prepend("\n\n--\n") ;
	cgi["Body"].add(sig) ;
  }
  cgi["my_ref"].set(config.unique_id(".")) ;

  String from(cgi["Name"].as_text()
	+ " <" + cgi.remote_user() + '@' + config.domain() + '>') ;

  StringSLBag& DefaultHeaders
	= ((form_select_multiple*) cgi["Headers"]())->value() ;
  form_textarea* Headers = (form_textarea*) cgi["Head"]() ;

  String From("From") ;
  if ( _use_from = DefaultHeaders.contains(From), !_use_from ) {
    cgi["From"].set(from) ;
  } else {
    from.prepend("From: ") ;
    from += '\n' ;
    Headers->set(from) ;
    DefaultHeaders.del(From) ;
  }

  String Org("Organization") ;
  if ( _use_org=DefaultHeaders.contains(Org), _use_org ) {
	String org = Org + ": " + cgi["Organization"].as_text() + '\n' ;
	Headers->add(org) ;
	DefaultHeaders.del(Org) ;
	cgi[Org].set(NULL_String) ;
  }
  for (Pix x = DefaultHeaders.first(); x; DefaultHeaders.next(x)) {
//cerr << "Got header " << DefaultHeaders(x) << "\n" ;
    if (DefaultHeaders(x) != NULL_String)
	Headers->add(DefaultHeaders(x) + ": \n") ;
  }
}
void Composer_display() {
  cout << cgi.start("post.cgi", "POST", "multipart/form-data") ;
  cout << cgi.render("my_ref") ;
  cout << cgi.render("In-reply-to") ;
  cout << "<h1>Message Composition</h1>"
  	"<table>" ;
  cout << "<tr><td>To</td><td>" << cgi.render("To") << "</td></tr>" ;
  cout << "<tr><td>Subject</td><td>" << cgi.render("Subject")
	<< "</td></tr>" ;
if (!_use_from)
  cout	<< "<tr><td>From</td><td>" << cgi["From"].as_html() << "</td></tr>" ;
if (!_use_org)
  cout	<< "<tr><td>Organization</td><td>" << cgi["Organization"].as_html() << "</td></tr>" ;
  cout << "<tr><td>Other Headers</td><td>" << cgi.render("Head")
	<< "</td></tr>" ;
  cout << "<tr><td>"
	<< cgi.render("AttachType")
	<< "</td><td>"
	<< cgi.render("Attach")
;cout	<< " type " << cgi.render("BASE64")
	<< "</td></tr>" ;
  cout << "<tr><td colspan=2>" << cgi.render("Body")
	<< "</td></tr>" ;
  cout << "</table>" ;
  cout << cgi.submit("Send") ;
  cout << cgi.reset("Cancel") ;
  cout << cgi.end() ;
}
void MAIN_compose_general(char* prog) {
  Composer(prog) ;
  Layout l("Mail Composition") ;
  l.print(1) ;
  Composer_display() ;
}
void MAIN_compose() { MAIN_compose_general("compose.cgi") ; }
void MAIN_reply() { MAIN_compose_general("reply.cgi") ; }
Register compose_cgi("compose.cgi", MAIN_compose) ;
Register reply_cgi("reply.cgi", MAIN_reply) ;
// Register forward_cgi("forward.cgi", MAIN_forward) ;
