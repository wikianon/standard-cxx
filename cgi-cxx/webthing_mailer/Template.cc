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
#include <dirent.h>
#include "Config.h"
#include "Layout.h"
#include "Register.h"

extern CGI cgi ;

void display(FORM& select) {
  cout <<
"<h1>Reply Templates</h1>"
"<p>If you get a lot of messages calling for similar replies, but "
"wish to avoid the impersonality of an autoresponder, you can use "
"these templates.</p>"
"<p>To use this facility, you enter a generic reply in this form. "
"When you are replying to a message, you then have the option to "
"pre-load the template in the message composition window, where "
"you can then personalise and tailor it to the message you are "
"answering.</p>"
"<p>Note that the <a href=\"server.cgi\">MailServer</a> permits "
"you to set up fully automated replies, and the InterFAQ facility of "
"the <a href=\"selforg/\">Web&#222;ing conferencing and collaborative "
"authoring environment</a> offers an altogether more advanced approach to "
"answering Frequently Asked Questions.</p><hr>"

<< select.start("template.cgi", "GET")
<< "Select a template to edit, or create a new one: "
<< select.render("name")
<< select.submit("Select")
<< select.end()
;cout
<< "<p><hr><p>To delete an existing template, just edit it and delete "
"all the text</p>"

<< cgi.start("template.cgi")
<< "Name: " << cgi.render("name")
<< "<p>Text: " << cgi.render("text")
<< "<p>"
<< cgi.submit("OK")
<< cgi.reset()
<< cgi.end()
;
}
void read_templates(FORM& select, const String& name) {
  select.set_template("name:select:" + name) ;
  select["name"]["-"] = "New Template" ;
  DIR* dir = opendir(config.template_dir()) ;
  struct dirent* f ;
  while (f = readdir(dir), f) {
    String fname(f->d_name) ;
    if (fname.matches(RXidentifier))
	select["name"][fname] = fname ;
  }
  closedir(dir) ;
}

inline void error() {
cout << "<h2>Error</h2>"
"<p>Invalid name for reply template.   Only alphanumerics and underscore "
"are allowed, and the first character must be a letter.</p><hr>" ;
}
inline void confirm(const String& name) {
  cgi.saveto(config.template_file(name)) ;
  cgi["text"].set(www_unescape(cgi["text"].as_text())) ;
  cout << "<p><big>Template reply <code>"
	<< name
	<< "</code> created.</big></p><hr>" ;
}
inline void del(const String& name) {
  if ( unlink(config.template_file(name)) == -1 )
    cout << "<p>Error deleting " << name << " - is there such a template?</p>";
  else
    cout << "<p><big>Template " << name << " is no more.</big></p>" ;
}
void MAIN_template() {
  cgi.define(2,
	"name:input" ,
	"text:textarea::12:72"
  ) ;
  cgi.form() ;

//  cout << "Content-Type: text/html\n\n" ;
  Layout l("Reply Templates") ;
  l.print(1) ;
  String name(cgi["name"].as_text()) ;
  if ( ! name.matches(RXidentifier))
    cgi["name"].set((name=NULL_String,name)) ;

  if ( cgi.request_method() == "POST" )
    if ( name == NULL_String )
      error() ;
    else if ( cgi["text"].as_text() == NULL_String )
      del(name) ;
    else
      confirm(name) ;
  else if (name != NULL_String)
    cgi.readfrom(config.template_file(name)) ;
// cout << "dunnit\n" ;
  FORM select ;
  read_templates(select, name) ;
  display(select) ;
}
Register template_cgi("template.cgi", MAIN_template) ;
