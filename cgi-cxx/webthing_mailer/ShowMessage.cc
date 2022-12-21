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
#include <CGI++/FileStream.h>
#include <CGI++/Escape.h>
#include "Message.h"
#include "Text.h"
#include "Address.h"
#include "BookAddress.h"
#include "Config.h"
#include "Layout.h"
#include "Register.h"
#include <dirent.h>

extern  CGI cgi ;
extern Config config ;

void MAIN_message(char* prog) {
  cgi.define(6,
	"what:select:2" ,
	"To:input::16" ,
	"do:select" ,
  	"qq:select:no" ,
  	"templ:select:none" ,
    	"id:hidden"
  ) ;
  cgi.form() ;

  //String me(cgi.script_name().after('/',-1)) ;
  String me(prog) ;
  String& message_id = cgi["id"].as_text() ;

  Istream msg(config.message(message_id)) ;

  if ( me == "raw.cgi" ) {
    cout << "Content-Type: text/plain\n\n" ;
    char c ;
    while ( msg.get(c) )
      cout << c ;
  } else if ( me == "download.cgi" ) {
    cout << "Content-Type: application/octet-stream\n\n" ;
    char c ;
    while ( msg.get(c) )
      cout << c ;
  } else if ( me == "rfc822.cgi" ) {
    cout << "Content-Type: message/rfc822\n\n" ;
    char c ;
    while ( msg.get(c) )
      cout << c ;
//    cout << Message(msg) ;
/*
    Message m(msg) ;
    String& ct(m.header("Content-Type")) ;
    ct.downcase() ;
    if (ct == NULL_String || ct.index("text") == 0 )
	cout << "Content-Type: message/rfc822\n\n" ;
    cout << m ;
*/
/*
    String headers, line ;
    int have_content_type = 0 ;
    Regex ct("Content-Type: .+",1) ;
    while ( msg >> line ) {
      headers += line ;
      if ( 
      line.before(':').capitalize() ;
      if (line.matches(ct)) {
	have_content_type = 1 ;
	break ;
      }
    }
*/
  } else {
    //cout << "Content-Type: text/html\n\n" ;
    Layout l("Message Display") ;
    l.print(1) ;
    Message m(msg) ;
    Address a(m.reply_address()) ;
    String addr_msg(a.name()) ;
    addr_msg.prepend("Check addressbook at ") ;
    cout // << "<hr noshade><table cellspacing=5><tr><td>"
//	<< "<table width=\"100%\"><tr><td><big>Message</big></td><td>"
	<< "<table align=right><tr><td><pre><b>Date:</b>    "
	<< m.header("Date", 40)
	<< "<br><b>From:</b>    "
	<< a.name()
	<< "<br><b>Subject:</b> "
	<< m.header("Subject", 40)
	//<< "</td></tr><tr><td colspan=2><hr noshade></td></tr><tr><td>"
	<< "\n</pre></td></tr></table>"
  ;
    cout << "<h2>Message Text</h2><br clear=right>"
	<< Text(m.part(1)).smart()
//	<< "<pre>" << html_escape(m.part(1)) << "</pre>"
	<< "<hr noshade>" ;
/*
	<< "</td><td bgcolor=\"#000000\">"
	<< "&#160;" // </td><td align=right>" ;
*/
    String& id = www_escape(message_id) ;
    cgi["id"].set(message_id) ;
    cgi["qq"]["no"] = "No" ;
    cgi["qq"]["yes"] = "Yes" ;
    cgi["templ"]["-"] = "None" ;

  DIR* dir = opendir(config.template_dir()) ;
  struct dirent* f ;
  while (f = readdir(dir), f) {
    String fname(f->d_name) ;
    if (fname.matches(RXidentifier))
	cgi["templ"][fname] = fname ;
  }
    cout << cgi.start("reply.cgi")
	<< cgi.submit("Reply")
	<< cgi.render("id")
	<< " Use Template: "
	<< cgi.render("templ")
    ; cout
	<< "Quote Original? "
	<< cgi.render("qq")
	<< cgi.end() ;
  cgi["what"]["1"] = "Message Header" ;
  cgi["what"]["2"] = "Message Body" ;
  if ( m.is_mime() )
    cgi["what"]["3"] = "Attachments" ;
  cgi["what"]["4"] = "Complete Message" ;
  cgi["do"]["export"] = "Desktop File" ;
  cgi["do"]["forward"] = "Email Address" ;
    cout << "<p>"
	<< cgi.start("forward.cgi")
	<< cgi.submit("Forward") << " the "
	<< cgi.render("what")
	<< cgi.render("id")
;cout	<< " to "
	<< cgi.render("do")
	<< cgi.render("To")
	//<< cgi.render("to")
	<< cgi.end() ;
	
//    cout << "</td></tr></table>" ;
    cout << "<hr noshade>" ;
    cout << "<table cellspacing=5><tr><td valign=top><h2>Message</h2>" ;
    switch ( m.is_mime() ) {
    case 2:
      cout << "This is a MIME message and may have attachments.<ul>"
	"<li><a href=\"rfc822.cgi?id=" << id << "\">Full Display</a>" ;
      break ;
    case 1:
      cout << "This is a simple message, but seems to be mangled by the "
		"sender's mail software. Use \"Full Display\" to see "
		"it as the sender intended.<ul>"
	"<li><a href=\"rfc822.cgi?id=" << id << "\">Full Display</a>" ;
      break ;
    case 0:
      cout <<
	"This is a simple message. Its contents is shown in full above.<ul>" ;
      break ;
    }
    String esc_id(id) ;
    Regex escaped("%[0-9A-Fa-f][0-9A-Fa-f]") ;
    esc_id.gsub(escaped,"") ;
    //www_unescape(esc_id) ;
    cout << "<li><a href=\"raw.cgi?id=" << id << "\">Raw Display</a>"
	"<li><a href=\"download.cgi/"
	<< esc_id
	<< "?id=" << id << "\">Download Message</a>"
    "</td><td>&#160;</td><td valign=top><h2>Sender</h2>" ;
	a.show_sender() ;
    cout << "</td></tr></table>" ;
  }
}
void _message_cgi() { MAIN_message("message.cgi") ; }
Register message_cgi("message.cgi", _message_cgi) ;
void _rfc822_cgi() { MAIN_message("rfc822.cgi") ; }
Register rfc822_cgi("rfc822.cgi", _rfc822_cgi) ;
void _raw_cgi() { MAIN_message("raw.cgi") ; }
Register raw_cgi("raw.cgi", _raw_cgi) ;
void _download_cgi() { MAIN_message("download.cgi") ; }
Register download_cgi("download.cgi", _download_cgi) ;

