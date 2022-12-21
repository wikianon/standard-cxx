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

#include <CGI++/Escape.h>
#include "Address.h"
#include "BookAddress.h"

extern String NULL_String ;

Address::Address(String& in) { set(in) ; }
void Address::set(String& in) {
  in.gsub('"',"") ;
  int leftbracket = in.index('<') +1  ;
  int at = in.index('@', leftbracket) ;
  int rightbr = in.index('>', at) ;
  if ( (0 < leftbracket) && (leftbracket < at) && (at < rightbr) ) {
    _email = in.at(leftbracket, rightbr-leftbracket) ;
    _name = in.at(0,leftbracket-2) ;
  } else {
    at = in.index('@') ;
    leftbracket = in.index('(',at) + 1;
    rightbr = in.index(')',leftbracket) ;
    if ( (0 < at) && (at < leftbracket) && (leftbracket < rightbr)) {
      _email = in.at(0,leftbracket-2) ;
      _name = in.at(leftbracket,rightbr-leftbracket) ;
    } else if (at > 0)
      _name = _email = in ;
    else {
      _name = in ;
      _email = NULL_String ;
    }
  }
  if ( ! _name.contains(RXalphanum) )
    _name = in ;
}
void Address::show_sender() const {
  BookAddress addr(*this) ;
  if ( addr.known() ) {
    cout << "Addressbook entry found for " << name() << ":<dl>" ;
    addr.dt_dd() ;
    cout << "</dl>" ;
  } else {
    cout << "Sender " << name() << " not known.   You may " ;
    cout
	<< "<a href=\"addressbook.cgi?do=new&name="
	<< www_escape(name())
;cout	<< "&email="
	<< www_escape(email())
	<< "\">add " << name() << " to your AddressBook.</a>" ;
  }
}
