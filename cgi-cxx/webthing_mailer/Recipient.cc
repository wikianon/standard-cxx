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
#include <strstream.h>
#include <sys/stat.h>
#include <sysexits.h>
#include "BookAddress.h"
#include "Post.h"
#include "Message.h"
#include "Config.h"
#include "Recipient.h"

extern CGI cgi ;

void Recipient::_init(const String& s) {
  _action = DELIVER ;
  String ss[2] ;
  switch ( split (s, ss, 2, '+') ) {
    case 1: _user = ss[0] ; _folder = "INTRAY" ; break ;
    case 2: _user = ss[0] ; _folder = ss[1] ; break ;
  }
  Regex tainted("[^A-Za-z0-9_]+") ;
  _user.gsub(tainted,"") ;
  _folder.gsub(tainted,"") ;
}
void Recipient::DELIVER(Message& m) {
  if ( ! _folder.matches(RXidentifier) )
    _folder = "ERRORS" ;

  if (link(m.master(), config.message(_user, m.id())) == 0) {
    m.summary(config.summary(_user, _folder)) ;
  } else if ( _folder != "ERRORS" ) {
    _folder = "ERRORS" ;
    DELIVER(m) ;
  } else if ( _user != "postmaster" ) {
    _user = "postmaster" ;
    DELIVER(m) ;
  } else
    exit(EX_SOFTWARE) ;
}
