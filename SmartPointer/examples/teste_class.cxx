#include <stdio.h>

class c
{
 public:
  short f;
};

class c2
{
 public:
  char * f2;
};

static class ttt: public c, public c2
{
 public:
  short m;
}ttt;

static class sss
{
 public:
 char * m;
 virtual int f (int i) {return i;}
}sss;

#define _offsetof(st,f) ((char *)&((st *) 16)->f - (char *) 16)



int main(void)
{
  printf("+++Class starting with pointer and containing virtual function:\n");
  printf("size=%d,align=%d\n", sizeof(sss), __alignof__(sss));
  printf("offset-m=%d,align-m=%d\n",_offsetof(class sss, m), __alignof__(sss.m));
  
  printf ("++Class with short inhereting classes with short & pointer:\n");
  printf ("size=%d,align=%d\n", sizeof (ttt), __alignof__ (ttt));
  printf ("offset-f=%d,offset-f2=%d,offset-m=%d,\nalign-f=%d,align-f2=%d,align-m=%d\n",
          _offsetof (class ttt, f), _offsetof (class ttt, f2), _offsetof (class ttt, m),
          __alignof__ (ttt.f), __alignof__ (ttt.f2), __alignof__ (ttt.m));
  return 0;
}