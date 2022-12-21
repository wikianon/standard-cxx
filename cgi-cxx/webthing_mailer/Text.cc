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

#include "Text.h"

void localtext(String& s) {
/* per-paragraph text level processing
   convert _something_ to <em>something</em>, etc
*/
  int pos, xpos ;
  const Regex em("[ \t\n]_[^_ \t\n]") ;
  const Regex xem("[^_ \t\n]_[ \t\n]") ;
  for (pos = s.index(em) ; pos >= 0 ; pos = s.index(em, ++pos))
    if ( xpos = s.index(xem, pos) , xpos > pos ) {
      s.at(pos+1,1) = "<em>" ;
      xpos = s.index('_',xpos) ;
      s.at(xpos,1) = "</em>" ;
      pos = xpos ;
    }

  const Regex strong("[ \t\n]\\*[^* \t\n]") ;
  const Regex xstrong("[^* \t\n]\\*[ \t\n]") ;
  for (pos = s.index(strong) ; pos >= 0 ; pos = s.index(strong, ++pos))
    if ( xpos = s.index(xstrong, pos) , xpos > pos ) {
      s.at(pos+1,1) = "<strong>" ;
      xpos = s.index('*',xpos) ;
      s.at(xpos,1) = "</strong>" ;
      pos = xpos ;
    }
}
String& Text::smart() {
  String tmp ;
  gsub("\r","") ;
  const Regex url("<[Uu][Rr][Ll]:[^ \t\n<>\"]+>",1) ;
  int lastpos = -1 ;
  int pos ;
  for (pos = index(url); pos >= 0; pos = index(url,lastpos)) {
    if ( lastpos >= 0 )
      before(pos) = before(lastpos)
	+ html_escape((tmp=before(pos),tmp=tmp.from(lastpos),tmp)) ;
    else
      before(pos) = html_escape((tmp=before(pos),tmp)) ;
    tmp = after((pos=index(url,pos),pos+4)) ;
    tmp = tmp.before(">") ;
    String repl("&lt;URL:<a href=\"" + tmp + "\">" + tmp + "</a>&gt;") ;
    at(pos,tmp.length()+6) = repl ;
    lastpos = pos + repl.length() ;
  }
  if ( lastpos < (int)length() )
    after(lastpos) = html_escape((tmp=after(lastpos),tmp)) ;

  const Regex blankline("\n\n+",1) ;
  int npara = freq("\n\n")+1 ;
  String* para = new String[npara] ;
  npara = split(*this, para, npara, blankline) ;

  const Regex format("\n[^A-Za-z0-9<(]\\|\t", 1) ;

  tmp = "" ;
  for (int i=0; i<npara; ++i) {
    localtext(para[i]) ;
    if ( para[i].contains(format) )
	tmp += "<pre>" + para[i] + "</pre>" ;
    else {
      Regex br("[:?!,;.-]\n") ;
      for (pos = para[i].index(br) ; pos >= 0 ;
		pos = para[i].index(br,pos+5))
	para[i].at(pos+1,1) = "<br>" ;
	tmp += "<p>" + para[i] + "</p>" ;
    }
  }
  delete[] para ;
  *this = tmp ;
  return *this ;
}
