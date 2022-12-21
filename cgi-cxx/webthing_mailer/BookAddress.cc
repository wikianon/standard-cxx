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
#include "BookAddress.h"
#include <CGI++/Escape.h>
#include <CGI++/CGI.h>

extern CGI cgi ;
const String br_escape(const String& text) {
  static const Regex nl("[\r\n]+",1) ;
  String ret(text) ;
  ret.gsub(nl,"<br>") ;
  return ret ;
}

BookAddress::BookAddress(const String& key) {
  _known = 0 ;
  BookAddress addr ;
  Istream in(config.addressbook()) ;
  while ( in >> addr )
    if ( key == addr._alias ) {
      *this = addr ;
      _known = 1 ;
      break ;
    }
}
BookAddress::BookAddress(const Address& key) {
  _known = 0 ;
  BookAddress addr ;
  Istream in(config.addressbook()) ;
  while ( in >> addr )
    if ( key.email() == addr._who.email() ) {
      *this = addr ;
      _known = 1 ;
      break ;
    }
}
istream& operator>> (istream& in, BookAddress& addr) {
  String buf ;
  String parts[5] ;
  in >> buf ;
  split(buf,parts,5,'&') ;
  addr._alias = www_unescape(parts[0]) ;
  String email(www_unescape(parts[1])) ;
  addr._access = parts[2] ;
  String name (www_unescape(parts[3])) ;
  addr._desc = www_unescape(parts[4]) ;
  addr._who.set(name, email) ;
  addr._known = 1 ;
  return in ;
}
ostream& operator<< (ostream& out, const BookAddress& addr) {
  out
	<< www_escape(addr._alias) << '&'
;out	<< www_escape(addr._who.email()) << '&'
	<< addr._access << '&'
;out	<< www_escape(addr._who.name()) << '&'
;out	<< www_escape(addr._desc) << endl ;
  return out ;
}

void BookAddress::write_this(ostream& tmp, const String& myalias) const {
  static int count =0 ;
  if ( ++count > 1 ) return ;
  if ( cgi["action"].as_text() == "delete" )
	return ;
  tmp	<< myalias << '&' ;
  String s = cgi["email"].as_text() ;
  tmp	<< www_escape(s) << '&'
	<< cgi["access"].as_text() << '&' ;
  s = cgi["name"].as_text() ;
  tmp	<< www_escape(s) << '&' ;
  s = cgi["desc"].as_text() ;
  tmp	<< www_escape(s) << endl ;
}

const int BookAddress::save() const {
  String book(config.addressbook()) ;
  String tmpbook(book+".tmp") ;
  BookAddress addr ;
  int ret = 0 ;
  String myalias(downcase(cgi["alias"].as_text())) ;
					// all lc => this is case-insensitive

  Istream in(book) ;
  Ostream tmp(tmpbook) ;

  while ( in >> addr )
    if ( addr._alias < myalias )	// check each entry isn't same
	tmp << addr ;
    else if ( myalias < addr._alias ) {// past it - needn't check rest
	write_this(tmp,myalias) ;
	tmp << addr ;
	break ;
    } else				// identical entry
	if ( cgi["mode"].as_text() == "NEW" )
	  ret = -1 ;			// refuse to overwrite accidentally
	else {
	  write_this(tmp,myalias) ;
	  break ;
	}
    if ( ret == 0 ) {
      write_this(tmp,myalias) ;		// insert here
      while ( in >> addr )		// finish copying (abandon if error)
	tmp << addr ;
      rename(tmpbook,book) ;
    }
/* tmpbook is still open (and locked) .. chop this if it gives trouble
   but unlinking when we don't have a lock could hurt.
   if ( ret >= 0 ) then we already renamed tmpbook
*/
  if ( ret < 0 )
    unlink(tmpbook) ;

  return ret ;
}
void BookAddress::ErrorDoc(const CDATA& name) const {
  cout	<< "<h2>Error</h2><p>Can't save addressbook entry - "
	"probably there is already an entry \""
	<< name <<
	"\" in your addressbook.   You should either go back "
	"and select a different alias, or go to the "
	"<a href=\"addressbook.cgi\">main addressbook page</a> "
	"to update (or delete) an existing entry.</p></form>" ;
  exit(0) ;
}
void BookAddress::confirm() const {
  String& alias(cgi["alias"].as_text()) ;
  if ( save() < 0 )
    ErrorDoc(cgi["alias"].as_text()) ;
  if ( cgi["action"].as_text() == "delete" ) {
    cout << "<h3>Entry for " << alias
  	<< "</h3><p>Deleted." ;
  } else {
    cout << "<h3>Entry for " << alias << "</h3><h4>"
	<< html_escape(cgi["name"].as_text())
	<< " &lt;" << html_escape(cgi["email"].as_text()) << "&gt;" << "</h4><p>"
	<< br_escape(html_escape(cgi["desc"].as_text()))
	<< "</p><p>" ;
    String& access = cgi["access"].as_text() ;
    if ( access == "1" )
      cout << alias << " is not authorised to access your "
	"restricted workgroup data.</p>" ;
    else if ( access == "2" )
      cout << alias << " has privilege to read your "
	"restricted workgroup data.</p>" ;
    else if ( access == "3" )
      cout << alias << " has privilege to read and write to your "
	"restricted workgroup data.</p>" ;
    cout	<< "</dl><p>If you wish to change "
	<< "this entry, <a href=\"addressbook.cgi?do=update&key="
	<< alias
	<< "\">here is the Form</a>.</p>" ;
  }
}
void BookAddress::Form(BookAddressMode mode) const {
  cout << cgi.start("addressbook.cgi") ;

  switch (mode) {
    case UPDATE:
	cgi["alias"].set(_alias) ;
	cgi["name"].set(_who.name()) ;
	cgi["email"].set(_who.email()) ;
	cgi["desc"].set(_desc) ;
	cgi["mode"].set("UPDATE") ;
	cout << "<h2>Update Entry for " << _alias << "</h2>" ;
	cout << cgi.render("mode") << "Alias: " << cgi.render("alias") ;
	cout << cgi.render("action") ;
	break ;
    case IMPORT:
	cgi["alias"].set(_alias) ;
	cout	<< "<h2>Entry for " << _alias << "</h2><p>This entry has "
		<< "been imported as shown - please check it.   If it is "
		<< "correct, you should just ignore this Form.</p>" ;
	if ( save() < 0 )
	  ErrorDoc(cgi["alias"].as_text()) ;
	cout << cgi.render("mode") << "Alias: " << cgi.render("alias") ;
	cout << cgi.render("action") ;
	break ;
    case NEW:
    cout << "<h2>New Entry</h2><p>Enter alias (or nickname) here.   If this "
	<< "correspondent is a Web&#222;ing user, you should use his/her "
	<< "Web&#222;ing Username as alias:</p>Alias: " ;
	cout << cgi.render("alias") ;
	cout << cgi.render("mode") ;
	break ;
  }

  cout	<< "<p>Description for this Entry.   This field is optional, "
	"and may be used to record general information such as "
	"postal address, phone, fax, timezone, etc.</p>" ;
  cout << cgi.render("desc") ;

  cout	<< "<p>An AddressBook entry may be an individual or a list. "
	"You can create a short list by entering multiple addresses "
	"(which should be separated by a comma) in the address "
	"box below.   Alternatively, you can use the "
	"<a href=server.cgi>List Manager</a> to define longer lists "
	"and enter your list's address in the Addressbook.<br>Name: "
	<< cgi.render("name")
;cout	<< "<br>Email: "
	<< cgi.render("email") << "</p>"

	<< cgi.submit("Commit Entry")
	<< cgi.reset("Revert to Saved Entry")
	<< cgi.end()
  ;
}
void BookAddress::dt_dd() const {
  if ( _known)
    cout << "<dt><p>[<a href=\"compose.cgi?To=" << www_escape(_who.email())
	<< "\">mailto</a> | <a href=\"addressbook.cgi?do=update&key="
	<< _alias << "\">update</a>]<b> "
	<< _alias << " (" << _access << ")</b></dt>"
	<< "<dd>" << _who.name() << " &lt;"
	<< _who.email() << "&gt;<br>"
	<< br_escape(html_escape(_desc))
	<< "</dd>" ;
  else
    cout << "<dt>Addressbook Error: no such entry at "
	<< _alias << "</dt>" ;
}
