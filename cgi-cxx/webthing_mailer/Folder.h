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

#ifndef FOLDER_H
#define FOLDER_H

/* Mail Folder Class: display, sort and update mail folders

   Relies on FolderEntry to deal with each individual line,
   and CGI data internally.
*/

#include <String.h>
class Folder {
  const String _foldername ;
  int (*sort_fn)(const void*,const void*) ;
  void display_update() ;
  void display_noupdate() ;

public:
/* Constructor requires folder name and sort method.  Sorting a folder
   is permanent, and only happens when the folder is being updated
   (otherwise the sort arg is ignored).
   Values of sort are:
	time
	subj
	from
   Any other input will leave it as-is.
*/
  Folder(const String& name ="INTRAY", const String& sort =0) ;

/* display() displays an entire folder in HTML.

	If called with method POST, the folder will be updated,
	including sorting and moving/deleting of selected messages.
	Only the form in the Folder itself will do this (i.e. to
	edit a folder you must first be viewing it).

	If called with GET (or HEAD), it will simply be displayed as-is.
*/
  void display() ;
} ;

#endif
