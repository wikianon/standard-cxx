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

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Config.h"
#include "Layout.h"
#include "Register.h"
#include <CGI++/FileStream.h>
#include <CGI++/CGI.h>

extern CGI cgi ;
extern Config config ;

typedef enum { NONE, EMPTY, NONEMPTY } file_status ;
const file_status filestatus(const char* filename) {
  struct stat filestats ;
  if ( stat(filename, &filestats) == -1)
    return NONE ;
  else
    return filestats.st_size ? NONEMPTY : EMPTY ;
}
inline const int is_empty(const char* filename) {
  return ( filestatus(filename) == EMPTY) ? 1 : 0 ;
}
inline const int exists(const char* filename) {
  return ( filestatus(filename) == NONE) ? 0 : 1 ;
}
void error(const char* msg) {
  cout << "<h2>Error</h2><p>Can't complete your request, because "
	<< msg << "</p>" ;
}
  
void MAIN_foldermgr() {

    cgi.define(3,
	"Name:input::48" ,
	"Organization:input:WebThing:48" ,
	"sort:select:time"
    ) ;
  Layout l("Folder Manager") ;
  l.print(1) ;
  cout << "<h1 align=center>Mail Folders</h1><hr noshade>" ;

  cgi["option"] = new form_radio(NULL_String) ;
  cgi["option"]["new"] = "Create New Folder" ;
  cgi["option"]["move"] = "Rename Folder" ;
  cgi["option"]["delete"] = "Delete Folder" ;

  String tmp("INTRAY") ;
  cgi["old"] = new form_select(tmp) ;
  cgi["new"] = new form_input(NULL_String,"12") ;

  cgi.form() ;

  String& nname = cgi["new"].as_text() ;
  String& opt = cgi["option"].as_text() ;
  const String& newname(config.folder(nname)) ;
  const String& oldname(config.folder(cgi["old"].as_text())) ;
  if ( nname != NULL_String && ! nname.matches(RXalphanum) )
    error("Only alphanumeric characters are allowed in Folder Names") ;

  else if ( opt == "new")
    if ( exists( newname ) )
      error("I cannot overwrite an existing folder!") ;
    else
      { Istream I(newname) ; }
  else if ( opt == "move" )
    if ( exists( newname ) )
      error("I cannot overwrite an existing folder!") ;
    else
      rename(oldname, newname) ;
  else if ( opt == "delete")
    if (is_empty(oldname))
      unlink(oldname) ;
    else
      error("I cannot delete a non-empty or nonexistent folder!") ;

  cgi["new"].set(NULL_String) ;

  cout	<< "<table><tr><td valign=top >" ;
  cout << "<h2>Open a Folder</h2><p>Folders which have changed<br>since "
	"you last read them<br>are shown as unvisited links.<ul>" ;

  struct DIR* folders = opendir(config.folder_dir()) ;
  struct dirent* f ;
  while ( f = readdir(folders) , f) {
    String fname(f->d_name) ;
    if ( fname.matches(RXalphanum) ) {
      struct stat buf ;
      stat(config.folder(f->d_name), &buf) ;
      cout << "<li><a href=\"folder.cgi/" << fname
	<< '?' << buf.st_mtime << "\">" << fname << "</a>" ;
      cgi["old"][fname] = fname ;
    }
  }
  closedir(folders) ;
  cout << "</ul></td><td bgcolor=\"#600000\">&nbsp;</td><td>" ;

  cout	<< cgi.start("foldermgr.cgi")
	<< "<h2>Manipulate Folders</h2>"
	"<p>Use this form to create, rename or delete folders:</p>"
	<< cgi.render("option")
	<< "<br>Folder to delete or rename: " << cgi.render("old")
	<< "<br>Name for new or renamed folder: " << cgi.render("new")
	<< "<br>" << cgi.submit()
	<< cgi.end()
	<< "</td></tr></table>"
;
  
}
Register foldermgr_cgi("foldermgr.cgi", MAIN_foldermgr) ;
