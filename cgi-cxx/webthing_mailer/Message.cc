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

#include <stdio.h>
#include <string.h>
#include "Message.h"
#include "Address.h"
#include "Config.h"
#include <CGI++/FileStream.h>
#include <CGI++/Escape.h>
#include <CGI++/CGI.h>

#include <time.h>

//extern Astream errlog ;

// Astream messagelog("/tmp/recipients") ;
extern Config config ;
extern CGI cgi ;


void Message::summary(const String& file) {
  Astream o(file) ;
  const unsigned char nbsp = 160 ;
  String h("Message-Id") ;
  String s = _headers[h] ;
  s.gsub(' ', nbsp) ;
  o << www_escape(s) << ' ' << time(0) << ' ' ;
  h = "From" ;
  s = Address(_headers[h]).name() ;
  s.gsub(' ', nbsp) ;
  o << s << ' ' ;
  h="Subject" ;
  s = _headers[h] ;
  s.gsub(' ', nbsp) ;
  o << s << endl ;
}

Error Message::read() {
  const String& filename = config.message_file(_id) ;	// lookup
  Istream in(filename) ;
  return read(in) ;
}
Bag& Message::recipients() {
  Regex TO("\\(\\(Original-\\)?\\(Resent-\\)?\\(To\\|Cc\\|Bcc\\)\\|\\(X-Envelope\\|Apparently\\(-Resent\\)?\\)-To\\)") ;
  Regex comma (",[ \t\r\n]*") ;
  Regex braces("\\(.*<\\)\\|\\(>.*\\)") ;
  String all_recipients ;
  int n = -1 ;
  for (Pix x = _headers.first(); x; _headers.next(x)) {
    String tmp = _headers.key(x) ;
    if ( tmp.matches(TO) ) {
      tmp = _headers.contents(x) ;
      if (++n) all_recipients += ',' ;
      all_recipients += tmp ;
    }
  }
  all_recipients.downcase() ;
  n = all_recipients.freq(',') + 1 ;
  String* r = new String[n] ;
  split(all_recipients,r,n,comma) ;
  static Bag ret ;
  for (int i=0; i<n; ++i) {
    String addr = Address(r[i]).email() ;
    String pair[2] ;
    int k = split(addr, pair, 2, '@') ;
    pair[1].gsub(braces, NULL_String) ;
    if (k == 1 || pair[1].contains(config.domain()) )
	ret.add(pair[0]) ;
  }
  return ret ;
}
Error Message::read(istream& in) {
  String line ;
  String name("RFC822-Error") ;
  in.unsetf(ios::skipws) ;
  Regex nonwhite("[^ \t]",1) ;
  Regex colon(":[ \t]*", 1) ;
  readline (in, _head) ;			// Envelope From line
  _head += "\n" ;

  while ( readline(in, line) ) {
    if (line.index("From:") ==0) line.gsub("\"","'") ;
    _head += line + "\n" ;
    if ( line.index(nonwhite) > 0 )
	_headers[name] += " " + line ;
    else {
      name = line.before(colon) ;
      name.capitalize() ;
      _headers[name] = line.after(colon) ;
    }
  }

  while ( readline(in, line), !in.eof() )
    _body << line + "\n" ;

  String mid("Message-Id") ;
  _id = _headers[mid] ;
  if ( header("Subject") == NULL_String )
    header("Subject") = "(no subject)" ;

  return 0 ;
}
Error Message::del() {
  return unlink(config.message_file(_id)) ;
}
const String Message::master() const {
  return config.message_file(_id) ;
}
Error Message::write() {
  Ostream out(config.message_file(_id)) ;
  out << _head ;
  out << "\n" ;
  out << _body.as_text() ;
  return 0 ;
}
Error Message::save() {
  return write() ;
}
String& Message::quote() {
  static int called =0 ;
  static String ret(header("From") + " wrote:\n") ;
  if (!called++) {
    ret += part(1) ;
    ret.gsub("\n", "\n> ") ;
  }
  return ret ;
}
String& Message::reply_address() {
  static String ret ;
  ret = header("Reply-To") ;
  if (ret == NULL_String)
    ret = header("From") ;
  if ( ! ret.contains('@') )
    ret = header("Return-Path") ;
  return ret ;
}
/* parse headers: assumes there's nowt but headers in what it's fed */
Map& Message::parse_headers(String& head) {
  const Regex endl("[\r\n]+") ;
  static const Regex fold("\r\n[ \t\r\n]+") ;
  static const Regex colon(":[ \t]*",1) ;
  static const Regex to_colon("[^ \t:]+:") ;

  head.gsub(fold,' ') ;
  int nlines = head.freq("\n") + 1 ;
  String* h = new String[nlines] ;
  nlines = split(head,h,nlines,endl) ;
  String thishdr[2] ;

  for (int line=0; line<nlines; ++line) {
    if (! h[line].contains(to_colon,0)) {
      _errors = _errors + "Header Error at or near " + line + '\n' ;
    } else {
      if (split(h[line],thishdr,2,colon) == 2) {
      _headers[thishdr[0]] = thishdr[1] ;
}
    }
  }
  delete [] h ;
  return _headers ;
}
void Message::update_id() {
  _id.at("<") = "<R." ;
}
void Message::start_mime() {
  _boundary = config.unique_id("--") ;
  String h("MIME-Version") ;
  _headers[h] = "1.0" ;
  String ctype("multipart/mixed; boundary=\"") ;
  ctype += _boundary + '"' ;
  h = "Content-Type" ;
  _headers[h] = ctype ;
  _body << "This is a multipart message in MIME format.\n\n\
If you are reading this, your mailer is not MIME-compatible.\n\
Send mail to mime@docserver.cac.washington.edu for more information.\n" ;

}
void Message::end_mime() {
  _body << "\n--" + _boundary + "--\n" ;
}

void Message::add_mime(const char* part) {
  _body << "\n--" + _boundary
	+ "\nContent-type: text/plain\n\n" + part ;
}

void Message::add_mime(FEP& part, const String& as) {
  _body << "\n--" + _boundary + "\n" ;
  String filename = part.filename() ;
  String local ;
  if ( filename != NULL_String ) {
    local = "Content-Disposition: " + as + "; filename=\"" ;
    local += part.filename() + "\"\n" ;
  }
  Map& headers ( part.headers()) ;
  for (Pix x = headers.first(); x; headers.next(x))
    local += headers.key(x) + ": " + headers.contents(x) + "\n" ;

  int is_bin = cgi["BASE64"].as_text().length() ;
  if (local.contains("Content-Type") == 0)
    if ( is_bin )
      local += "Content-Type: application/octet-stream\n" ;
    else
      local += "Content-Type: text/plain\n" ;	// default if not set

  _body << local ;

  int tmptext = downcase(local.after("Content-Type:")).index("text") ;
  if ( tmptext < 0 || tmptext > 3 || is_bin) {
    _body << "Content-Transfer-Encoding: BASE64\n\n" ;
    _body << part.as_base64() ;
  } else {
    _body << "\n" ;
    _body << part.as_longtext() ;
  }
}
void Message::send(void) {
  cout.flush() ; fflush(stdin) ; fflush(stdout) ;
  FILE* mailer = popen("/usr/sbin/sendmail -t","w") ;
  if ( !mailer ) { // || mailer->_fileno <= 0 ) {
	cout << "<h1>Error</h1>Mailer unavailable - please try later" ;
	return ;
  }
  cout << "<h1>Posting Message</h1>" ;
  for (Pix x = _headers.first(); x; _headers.next(x)) {
    fprintf(mailer,"%s: %s\n", (const char*)_headers.key(x), (const char*)_headers.contents(x)) ;
    cout << html_escape(_headers.key(x))
	<< ": "
	<< html_escape(_headers.contents(x)) << "<br>" ;
  }
  fprintf(mailer, "\n%s", _body.as_text()) ;
  cout << "<p>(followed by " << _body.length() << " bytes of message data)." ;
  if ( pclose(mailer) != 0 )
    cout << "<h2>Error!</h2>" << "Please mail this page to "
	"webthing+errors@webthing.com" ;
  Astream(config.sendlog()) << time(0) << ": SEND " << _body.length()
	<< " f=" << header("From")
	<< " t=" << header("To")
	<< " s=" << header("Subject") << endl ;
}
void Message::relay(void) {
  cout.flush() ; fflush(stdin) ; fflush(stdout) ;
  FILE* mailer = popen("/usr/sbin/sendmail -t","w") ;
  if ( !mailer ) {
	return ;
  }
  for (Pix x = _headers.first(); x; _headers.next(x)) {
    if (_headers.key(x).contains(' ') )
      continue ;
    fprintf(mailer,"%s: %s\n", (const char*)_headers.key(x), (const char*)_headers.contents(x)) ;
  }
  fprintf(mailer, "\n%s", _body.as_text()) ;

  if ( pclose(mailer) != 0 )
	;
  Astream(config.sendlog()) << time(0) << ": RELAY " << _body.length()
	<< " f=" << header("From")
	<< " t=" << header("To")
	<< " s=" << header("Subject") << endl ;
}
const Regex& Message::rxbdy() {
  String& ctype = header("Content-Type") ;
  if ( _boundary == NULL_String ) {
    int pos = downcase(ctype).index("boundary")+10 ;
    if ( pos >= 10 ) {
      _boundary = ctype.from(pos) ;
      _boundary = _boundary.before('"') ;
      _boundary.prepend("--") ;
    }
  }
  static Regex rx(_boundary) ;
  return rx ;
}

const int Message::is_mime() {
  String ct = header("Content-Type") ;
  if ( _boundary == NULL_String )
    if ( (ct.length() == 0 )
	|| downcase(ct).contains("text/plain") )
	return 0 ;
    else
	return 1 ;
  else
	return 2 ;
}
ostream& operator<< (ostream& o, Message& m) {
  Map& head(m._headers) ;
  for (Pix x = head.first(); x; head.next(x)) {
    String& name = head.key(x) ;
    String& value = head.contents(x) ;
    name.capitalize() ;
    if ( value != NULL_String)
      o << name << ": " << value << "\n" ;
  }
    o << "\n" << m._body ;
  return o ;
}
