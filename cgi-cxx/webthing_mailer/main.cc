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

#include "Register.h"
#include <String.h>
#include <CGI++/CGI.h>
#include "Config.h"
#include <string.h>

CGI cgi ;
Config config ;

Register* Register::first = 0 ;

int Register::dispatch(const char* arg) {
    if ( this )
      if ( !strcmp(_name, arg) )
	return _f(), 0 ;
      else
	return _next->dispatch(arg) ;		// +1 for profiling
    else
      return -1 ;
}
Register::Register(const char* name, void (*f)()) : _name(name), _f(f) {
  _next = first ;
  first = this ;
#ifdef DEBUG
  cerr << "Registering " << name << endl ;
#endif
}
int main(int, char** argv) {
    char* prog = strrchr(argv[0], '/') ;	// in gdb we get the fullpath
    if (prog)
	++prog ;
    else
      prog = argv[0] ;
 
  if ( Register::first->dispatch(prog) == -1 )
	cerr << "No handler for " << prog << endl ;

  return 0 ;
}
