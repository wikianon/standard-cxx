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

#ifndef REGISTER_H
#define REGISTER_H

/* For routines to register themselves to be called as a sub-main().

  Register("foo", foo)
  causes foo() to be called straight from main() when argv[0] is "foo"

  Use static linking for modules containing functions which register
*/

class Register {
  friend int main(int, char**) ;
  static Register* first ;

private:
  Register* _next ;
  const char* _name ;
  void (*_f)() ;
  int dispatch(const char* as) ;
public:
  Register(const char* name, void (*f)()) ;
} ;
#endif
