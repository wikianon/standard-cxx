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

#ifndef FolderEntry_H
#define FolderEntry_H

/* FolderEntry is a class for managing and displaying message summary
   information in a mail folder display.  The methods are to read and
   write internal folder files, and to display as HTML.
*/

#include <String.h>
#include <time.h>
#include <CGI++/FileStream.h>
class FolderEntry {
/* sort functions for passing to qsort() by folder manager */
  friend int tcomp(const void*, const void*) ;
  friend int scomp(const void*, const void*) ;
  friend int fcomp(const void*, const void*) ;

	String _id ;
	String _from ;
	String _subj ;
	time_t _time ;
  void anchor_button(String, const char*, ostream&) ;
  void select_button(ostream&) ;
  void read_button(ostream&) ;
  void reply_button(ostream&) ;
public:
  String& id() { return _id ; }
  FolderEntry() {}

/* For printing as HTML */
  friend ostream& operator<<(ostream& o, FolderEntry& f) ;

/* I/O from folder summary file */
  friend Istream& operator>>(Istream& in, FolderEntry& f) ;
  friend Astream& operator<<(Astream& o, FolderEntry& f)
	{ return o << f._id << ' ' << f._time
	<< ' ' << f._from << ' ' << f._subj << endl , o ; }
  friend Ostream& operator<<(Ostream& o, FolderEntry& f)
	{ return o << f._id << ' ' << f._time
	<< ' ' << f._from << ' ' << f._subj << endl , o ; }

} ;

#endif
