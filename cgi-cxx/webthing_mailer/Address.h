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

#ifndef ADDRESS_H
#define ADDRESS_H
#include <String.h>

/* Class to parse email addresses.

   Not 100% RFC822-compliant, but I don't _think_ this will actually
   break anything unless your sendmail is already broken.  I also
   suspect it's unlikely to break on anything that doesn't break a
   high proportion of other mailers around the 'Net.

   Parses the following formats:
	(1) webthing@webthing.com
	(2) webthing@webthing.com (WebThing Ltd)
	(3) WebThing Ltd <webthing@webthing.com>
   In the first case (bare address), it has to serve for both the
   human- and machine-friendly representations.
*/

class Address {

  String _name, _email ;
  void set(String&) ;

public:
/* The String& constructor parses its argument */
  Address (String&) ;

/* human and machine-friendly representations */
  const String& name() const { return _name ; }
  const String& email() const { return _email ; }

/*
The following violate encapsulation by overlapping "BookAddress"
which deals with WebThing AddressBook representations.
*/
  friend class BookAddress ;
  friend istream& operator>>(istream&, BookAddress&) ;

/* display addressbook entry (if found) or a link to enter sender */
  void show_sender() const ;

private:
  Address () {}
  void set(String& name, String& email) { _name = name ; _email = email ; }
} ;
#endif
