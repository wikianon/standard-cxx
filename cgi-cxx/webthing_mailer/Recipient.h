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

#ifndef RECIPIENT_H
#define RECIPIENT_H

#include "Message.h"

/* Recipient may be a folder, a WebThing processing function or an
   external program.  Details are specified in the User's server.defs
   for generic processing (e.g. level of spam filtering) and in an
   individual server definition for each server that isn't a plain
   folder.  This is the principal module of WebThing's mailserver,
   but has been reduced to simple delivery for the mail-client-only
   version.
*/

class Recipient {
  String _folder ;
  String _user ;

  void DELIVER(Message&) ;			// to folder
  
  void _init (const String&) ;
  void (Recipient::*_action )(Message&) ;

public:
/* Constructor parses the address by splitting on +:
	One part (user@domain) is a user's INTRAY folder
	Two parts (user+folder@domain) is a user-defined server
		or named folder
	Three parts (user+folder+control@domain is a control message
		(currently limited to mailing list subscribe/unsubscribe)
*/
  Recipient(const char* c) { const String s(c) ; _init(s) ; }
  Recipient(const String& s) { _init(s) ; }

/* Deliver processes the message using the action determined by constructor */
  void deliver(Message& m) { _action(m) ; }
} ;
#endif
