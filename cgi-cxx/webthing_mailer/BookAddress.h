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

#ifndef BOOKADDRESS_H
#define BOOKADDRESS_H

#include "Address.h"
#include <CGI++/CGI.h>
#include <CGI++/FileStream.h>

/* BookAddress deals with AddressBook entries, providing facilities
   to enter, display, update and clone them.  It will also look
   up an Address from the header of a message (or anywhere else
   we happen to have one).  BookAddress can also parse lists.

   Depends on class Address to parse raw addresses.
*/

/* BookAddressMode: is this a new address or an existing entry?

	NEW - new entry (refuse to overwrite existing entry)
	UPDATE - display existing entry in Form() and do overwrite it
	IMPORT - clone name/address from a message we're reading;
		 refuse to overwrite an existing entry
*/
typedef enum { NEW, UPDATE, IMPORT } BookAddressMode ;

class BookAddress {
  friend istream& operator>>(istream&, BookAddress&) ;
  friend ostream& operator<<(ostream&, const BookAddress&) ;
  friend class Address ;

  String _alias ;
  String _desc ;
  String _access ;
  class Address _who ;
  char _known ;

  void ErrorDoc(const CDATA&) const ;
  const int save() const ;
  void write_this(ostream&,const String&) const ;

public:
  BookAddress(const char _ =0) : _known(_) { }
  BookAddress(const String&) ;			// alias lookup
  BookAddress(const Address&) ;			// perform address lookup
  const char known() const { return _known ; }	// did lookup find it?


/* Form() and confirm() are the main methods of the AddressBook program.

	Form() displays a form, pre-filled with whatever information is
	available (ALL for an existing entry, name and email when
	cloning the sender of a message we're reading.

	confirm() makes the entry, and displays it (or an error message
	on failure)
*/
  void Form(const BookAddressMode) const ;
  void confirm() const ;

/* Methods to display an entry, including HTML links for mailto and update */
  void dt_dd() const ;
  void display() const { cout << "<dl>" ; dt_dd() ; cout << "</dl>" ; }
} ;
#endif
