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

#ifndef CONFIG_H
#define CONFIG_H

/* Filesystem info, etc.  All the stuff that really needs configuring is 
   in a separate file "Config.local", so you should be able to ignore
   Config.h
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <String.h>
#include <stdlib.h>

#include <strstream.h>
#include <time.h>


class Config {
  const String basepath ;
  String _path ;

public:
#include "Config.local"

  const String intray() const
	{ return folder("INTRAY") ; }

  const String tmpstore(char const* id) const
	{ return basepath + "tmp/" + id ; }

  const String preferences() const
	{ return _path + "/preferences"  ; }

  const String folder(char const * name) const
	{ return _path + "/folders/" + name  ; }

  const String message(char const * id) const
	{ return _path + "/messages/" + id  ; }

  const String recipients(const String& id) const
	{ return _path + "/deliveries/" + id  ; }

  const String recipients_lock(const String& id) const
	{ return _path + "/deliveries/lock." + id  ; }

  const String folder_dir() const
	{ return _path + "/folders/" ; }

  const String message_dir() const
	{ return _path + "/messages/" ; }

  const String message(const String& u, const String& id) const
	{ return _path + u + "/messages/" + id ; }

  const String template_dir() const
	{ return _path + "/templates/" ; }

  const String template_file(const String& name) const
	{ return _path + "/templates/" + name ; }

  const String server_h(const String& name) const
	{ return _path + "/server/" + name + ".h" ; }

  const String server_b(const String& name) const
	{ return _path + "/server/" + name + ".b" ; }

  const String server_dir() const
	{ return _path + "/server/" ; }

  const String server_defs() const
	{ return _path + "/server.defs" ; }

  const String server_defs(const String& u) const
	{ return _path + u + "/server.defs" ; }

  const String reply(const String& u, const String& id) const
	{ return _path + u + "/server/" + id ; }

  const String server_b(const String& u, const String& name) const
	{ return _path + u + "/server/" + name + ".b" ; }

  const String server_h(const String& u, const String& name) const
	{ return _path + u + "/server/" + name + ".h" ; }

  const String message_file(const String& id) const
	{ return _path + "/messages/" + id ; }

  const String summary(const String& id) const
	{ return _path + "/folders/" + id ; }

  const String summary(const String& user, const String& folder)
	{ return _path + user + "/folders/" + folder ; }

  const String folder(const String& user, const String& folder)
	{ return _path + user + "/folders/" + folder ; }

  const String addressbook() const
	{ return _path + "/addressbook" ; }

  const String user(const String& u) const
	{ return _path + u ; }

  const String addressbook(const String& user) const
	{ return basepath + user + "/addressbook" ; }
/*
  const int valid_user(const String& user) const {
    struct stat buf ;
    String path(basepath+user) ;
    if ( stat(path, &buf) == -1)
      return 0 ;
    else
      return 1 ;
  }
  */
  const char* unique_id(const String& sep) const {
    ostrstream o ;
    o << time(0) << sep << getpid() << ends ;
    return o.str() ;
  }
} ;

extern Config config ;

#endif


