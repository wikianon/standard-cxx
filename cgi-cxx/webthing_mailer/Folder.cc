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

#include "Folder.h"
#include "FolderEntry.h"
#include "Config.h"
#include <CGI++/FileStream.h>
#include <CGI++/Escape.h>
#include <CGI++/CGI.h>

const int FOLDERSIZE = 128 ;
extern Config config ;
extern CGI cgi ;

void Folder::display() {
  if ( cgi.request_method() == "POST")
    display_update() ;
  else
    display_noupdate() ;
}

void Folder::display_update() {
  Astream destn(config.summary(cgi["folders"].as_text())) ;
  Bag& selected = cgi["id"].multi() ;
  String fname(config.summary(_foldername)) ;
  String tmpname = fname + ".tmp" ;
 { // enter new scope to be able to lose summary and tmp while we
   // still have lock on destn
  Istream summary(fname) ;
  Ostream tmp(tmpname) ;

  while ( !summary.eof()) {
    FolderEntry entries[FOLDERSIZE] ;
    int n = 0 ;
    while (summary >> entries[n]) {
      if ( selected.contains(entries[n].id()) )
	if ( cgi["action"].as_text() == "move" )
	  destn << entries[n] ;
	else
	  unlink (config.message(www_unescape(entries[n].id()))) ;
      else
	if ( ++n == FOLDERSIZE )
	  break ;
    }
    if ( n == 0 ) break ;
    qsort(entries, n, sizeof(FolderEntry), sort_fn) ;

    cout << "<table border=1>"
	<< "<tr><th>&#160;</th><th>From</th><th>Subject</th>"
	<< "<th>Date</th></tr>"
 ;
    for (int i =0; i<n; i++) {
      tmp << entries[i] ;
      cout << entries[i] ;
    }
    cout << "</table>" ;

    if (!summary.eof())
      cout << "<p>NOTE: Folder is too big to sort and has been split up. "
	<< "You should consider sorting your mail into different folders, "
	<< "which will dramatically speed it up and avoid the risk of "
	<< "your browser running out of memory.</p>" ;
  }
 }
 if (rename(tmpname,fname) == -1)
    cout << "<p><strong>Error:</strong> An internal error occurred while "
	<< "updating the folder - please retry.   If you were moving "
	<< "messages to another folder when this happened and find they "
	<< "are now in <em>both</em> folders, please repeat the move - "
	<< "do not delete them!   If in doubt, please ask us!</p>" ;
}
void Folder::display_noupdate() {
  Istream summary(config.summary(_foldername)) ;
  FolderEntry entry ;
  cout	<< "<table border=1>"
	<< "<tr><th>&#160;</th><th>From</th><th>Subject</th>"
	<< "<th>Date</th></tr>"
 ;
  while (summary >> entry)
    cout << entry ;
  cout << "</table>" ;
}
/*
String Folder::foldername() {
  return _foldername ;
}
*/
Folder::Folder(const String& name, const String& sort) : _foldername(name) {
  if ( sort == "time" )
    sort_fn = tcomp ;
  else if ( sort == "subj" )
    sort_fn = scomp ;
  else if ( sort == "from" )
    sort_fn = fcomp ;
  else
    sort_fn = 0 ;
}
