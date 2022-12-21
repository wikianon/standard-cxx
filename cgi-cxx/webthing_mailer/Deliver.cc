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

#include <sys/types.h>
#include <sys/wait.h>
#include <iostream.h>
#include <fstream.h>
#include <strstream.h>
#include "Message.h"
#include "Config.h"
#include "Recipient.h"
#include "Register.h"
#include <CGI++/CGI.h>
extern Config config;
extern CGI cgi ;

void MAIN_deliver() {
  Message message(cin) ;
  String delivered_to ;

// Need a lock on delivered_to throughout this
  Ostream o (config.recipients_lock(message.id())) ;

  Istream (config.recipients(message.id())) >> delivered_to ;

  message.save() ;	// Continue (to inc refcount) if already have message-id
			// Exit with error status on error
			// else return normally
  Bag& recipients = message.recipients() ;

  for (Pix x = recipients.first(); x; recipients.next(x)) {

// First check not already delivered to this address
    String& user = recipients(x) ;
    String User = '<' + user + '>' ;
    if (delivered_to.contains(User))
	continue ;
    else
	delivered_to += User ;

	Recipient to(user) ;
	  to.deliver(message) ;
    Astream (config.recipients(message.id())) << User ;
  }
}
Register deliver("deliver", MAIN_deliver) ;
/* linux seems to call it procmail even when it's this prog */
Register procmail("procmail", MAIN_deliver) ;
