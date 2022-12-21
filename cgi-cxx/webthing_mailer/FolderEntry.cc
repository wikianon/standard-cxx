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

#include <time.h>
#include "FolderEntry.h"
#include <CGI++/CGI.h>
#include <CGI++/Escape.h>

int tcomp(const void* X, const void* Y) {
  return ((FolderEntry*)X)->_time - ((FolderEntry*)Y)->_time ; 
}
int scomp(const void* X, const void* Y) {
  const Regex Re("re:[ \t\xA0]*") ;
  String xsubj(downcase(((FolderEntry*)X)->_subj)) ;
  String ysubj(downcase(((FolderEntry*)Y)->_subj)) ;
  int order = 0 ;
  while (! xsubj.index(Re)) { ++order ; xsubj = xsubj.after(Re) ; }
  while (! ysubj.index(Re)) { --order ; ysubj = ysubj.after(Re) ; }
  int cmp = compare(xsubj, ysubj) ;
  return cmp ? cmp : order ;
}
int fcomp(const void* X, const void* Y) {
  return compare(((FolderEntry*)X)->_from, ((FolderEntry*)Y)->_from) ; 
}
ostream& operator<<(ostream& o, FolderEntry& f) {
  o	<< "<tr><td>" ; f.select_button(o) ;
  o	<< "</td><td>" ; f.reply_button(o) ; // _from
  o	<< "</td><td>" ; f.read_button(o) ; // _subj
  o	<< "</td><td>" << asctime(gmtime(&f._time))
	<< "</td></tr>"
  ;
  return o ;
}
void FolderEntry::select_button(ostream& o) {
  o <<
  "<input type=checkbox name=id value=\"" << _id << "\">" ;
}
void FolderEntry::reply_button(ostream& o) {
  anchor_button(_from, "message", o) ;
}
void FolderEntry::read_button(ostream& o) {
  anchor_button(_subj, "rfc822", o) ;
}
void FolderEntry::anchor_button(String code,
	char const*const name, ostream& o) {
  const unsigned char nbsp(160) ;
  code.gsub(nbsp,' ') ;
  o
	<< "<a href=\"" << name << ".cgi?id=" << _id
	<< "\">" << code << "</a>" ;
}
Istream& operator>>(Istream& in, FolderEntry& f) {
  return in >> f._id >> f._time >> f._from >> f._subj , in ;
}
