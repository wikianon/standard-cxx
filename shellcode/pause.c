#include <stdio.h>

//const char pause_shel[]="\xb8\x1d\x00\x00\x00\xcd\x80";

const char pause_shel[] = "\x31\xc0\xb0\x1d\xcd\x80";

int main()
{
 int (*shell)();
  
 shell=pause_shel;
 shell();
}
