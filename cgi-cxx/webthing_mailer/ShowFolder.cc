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
#include "Folder.h"
#include "FolderEntry.h"
#include "Config.h"
#include "Layout.h"
#include "Register.h"
#include <CGI++/CGI.h>

extern CGI cgi ;

void MAIN_folder() {

    cgi.define(1,
	"sort:select:time"
    ) ;
  Layout l("Folder Manager") ;
  l.print(1) ;
  String name(cgi.path_info().after('/')) ;
  String start("folder.cgi") ;
  cout << cgi.start(start+cgi.path_info()) ;
  cout << "<h1>MailFolder " << name << "</h1>" ;

  cgi["id"] = new form_select_multiple(NULL_String) ;

  String tmp("SavedMail") ;
  cgi["folders"] = new form_select(tmp) ;
  struct DIR* folders = opendir(config.folder_dir()) ;
  struct dirent* f ;
  while ( f = readdir(folders) , f) {
    String fname(f->d_name) ;
    if ( name == fname)
	continue ;
    if ( fname.matches(RXalphanum) )
	cgi["folders"][fname] = fname ;
  }
  closedir(folders) ;
  
  tmp = "delete" ;
  cgi["action"] = new form_radio(tmp, NULL_String) ;
  cgi["action"]["delete"] = "Delete selected messages, or " ;
  cgi["action"]["move"] = "move them to " ;

  tmp = "time" ;
  cgi["sort"] = new form_select(tmp) ;
  cgi["sort"]["time"] = "Time/Date" ;
  cgi["sort"]["from"] = "Sender" ;
  cgi["sort"]["subj"] = "Subject" ;
/*
  cout 
	<< "<p>Special Message Reading Modes: "
	<< "<b>R</b> - Raw (full headers) "
	<< "<b>D</b> - Download message</p>"
  ;
*/
  cgi.form() ;

  Folder folder(name, cgi["sort"].as_text()) ;
  folder.display() ;
  cout
	<< "<h4>Folder Control</h4>"
	<< "<p>To move or delete messages, select the button to the left of "
	<< "each message to be processed.   Then select \"Delete\" or "
	<< "a folder to move them to.</p>"
	<< "<table><tr><td colspan=2>" << cgi.render("action")
	<< cgi.render("folders")
;cout	<< "</td></tr><tr><td>Sort folder " << name << " by "
	<< cgi.render("sort")
	<< "</td><td>" << cgi.submit("OK") << "</td></tr></table>"
  ;

  cout << cgi.end() ;
}
Register folder_cgi("folder.cgi", MAIN_folder) ;
