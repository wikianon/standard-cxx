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

#ifndef MESSAGE_H
#define MESSAGE_H

/* Message is a kitchen-sink class for mail messages, handling headers,
   body and MIME attachments for:
	incoming messages (from sendmail),
	static messages (reading from mailfolder; exporting)
	Composition (form)
	Posting a composed message
	Forwarding or redistributing an incoming message

  It's brutally (and inelegantly) chopped, since this distribution 
  doesn't do anything interesting with incoming messages.

  Relies on class MessageBody as a buffer for body data.  The original
  design used the String class until I discovered its 32K limit.
*/

#include "MessageBody.h"
#include <CGI++/FEP.h>
#include <CGI++/FileStream.h>
typedef StringStringAVLMap Map ;
typedef int Error ;
extern String NULL_String ;

class Message {
	String _head ;
	String _id ;
	Map _headers ;
	MessageBody _body ;
	String _errors ;
	String _boundary ;
public:
	friend ostream& operator<< (ostream&, Message&) ;
	Message(const String& s =NULL_String)
		: _id(s) , _headers(NULL_String) , _body(s)
		{ }
	Message(istream& in) : _headers(NULL_String)
		{ read(in) ; }
	String& id() { return header("Message-Id") ; }

/* misc header functions: header() returns a header, optionally truncated
   to n chars; id() returns Message-Id.
   parse_headers() is used when posting, to parse free-format user input.
*/
	String& header(String& x) { return _headers[x] ; }
	String header(const char* x, unsigned int n) {
		String s(header(x)) ;
		if (s.length() <= n )
			return s ;
		else
			return s.at(0,n) ;
	}
	String& header(const char* c) { String x(c); return _headers[x] ; }
	Map& parse_headers(String& headers) ;

/* Reads are from stdin (new message passed by sendmail)
   or from a message file
*/
	Error read() ;			// from own name
	Error read(const String& id) { _id = id ; return read() ; }
					// from file
	Error read(istream&) ;

/* write() and save() are identical, and write to a message file.
   saveto() exports to desktop
*/
	Error write() ;
	Error save() ;
	Error saveto(const String& file, const int part) ;

	Error del() ;			// delete

/* For message delivery: we only deliver once, even if the same
   message arrives more than once from sendmail.  Keep track of
   who it's already delivered to with index()/recipients()/delivered_to()

   summary() composes a line for the folder index
   master() is the "master copy"; we link this to individual recipients
   to avoid keeping unnecessary duplicates.  masters are cleaned by cron.
   deliver() delivers to a user's folder
*/
	void summary(const String&) ;
	const String master() const ;
	Error index(String& to) ;	// add recipient as having it
	Bag& recipients() ;		// from headers
	Bag& delivered_to() ;		// from database
	Error deliver(String& to, char* folder ="INTRAY") ;

/* reply_address returns (in order of preference)
   Reply-To, From, Return-Path
*/
	String& reply_address() ;

/* Handles on the MessageBody */
	const char* body() { return _body.as_text() ; }
	const char* body(const char* body)
		{ _body << body ; return _body.as_text() ; }

/* MIME handling:
Writing:
	start_mime() writes the MIME prelude
	add_mime() adds the contents of a buffer as a MIME attachment;
	  the buffer may come straight from CGI (FEP&) or be an
	  arbitrary const char*.  Accepts "inline" or "attachment"
	  as additional args with FEP.
	end_mime() adds the end boundary

Reading:
	is_mime() returns 0 for non-MIME, 1 for one-part non-text
		(incoming only; we don't inflict that horror on the Net)
		  or 2 for multipart MIME
	part() returns the n'th part
*/
	const String& boundary() const { return _boundary ; }
	String& boundary() { return _boundary ; }
	const Regex& rxbdy() ;

	char* part(int n)
		{ return _body.part(n, rxbdy() ) ; }
	void start_mime() ;				// start
	void end_mime() ;				// start
	void add_mime(const char*) ;			// add
	void add_mime(FEP&, const String&) ;		// add
	void add_mime(FEP& x)
		{ String s("attachment") ; add_mime(x,s) ; }
	const int is_mime() ;

/* quote() is for replying, and quotes a message body (or the
     first part of a multipart message)
*/
	String& quote() ;

/* Internal errors */
	void add_error(const String& err) { _errors += err ; }
	int ok() const { return ( _errors.length() == 0 ) ; }
	const String& errormsg() const { return _errors ; }

/* send() sends a new message; relay() forwards an existing one (and
   is not the same as SMTP relaying
*/
	void send() ;
	void relay() ;

/* New message-id based on that of this message */
	void update_id() ;

} ;
#endif
