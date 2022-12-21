#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>

void my_exit(int status)
{
 status &= 0xff;
 
 abort();
}

int mini_printf(const char *fmt, ...)
{
 va_list arg;
 
 va_start(arg, fmt);
 
 int done = vfprintf(stdout, fmt, arg);
 
 va_end(arg);
 
 return done;
}

int mini_fprintf(FILE *stream, const char *fmt, ...)
{
 va_list arg;
  
 va_start(arg, fmt);
  
 int done = vfprintf(stream, fmt, arg);
  
 va_end(arg);
  
 return done;
}

int mini_sprintf(char *buf, const char *fmt, ...)
{
 va_list args;
 
 va_start(args, fmt);
 
 int done = vsprintf(buf,fmt,args);
 
 va_end(args);
 
 return done;
}

int mini_snprintf(char *str, size_t len, const char *fmt, ...)
{
 va_list ap;
 
 va_start(ap, fmt);
 
 int err = vsnprintf(str, len, fmt, ap);
 
 va_end(ap);
 
 return err;
}

int mini_dprintf(int d, const char *format, ...)
{
 va_list arg;
 
 va_start(arg, format);
 
 int done = vdprintf(d, format, arg);
 
 va_end(arg);
 
 return done;
}

/* Display an IP address in readable format. */
//1921681001
char *in_ntoa(unsigned long in)
{
 static char buff[18];
 register char *p;
  
 p = (char *) &in;
 
 mini_sprintf(buff, "%d.%d.%d.%d", (p[0] & 255), (p[1] & 255), (p[2] & 255), (p[3] & 255));
  
 return(buff);
}


int mini_fscanf(FILE *fp, char const *fmt, ...)
{
 va_list ap;
 
 va_start(ap, fmt);
 
 int ret = vfscanf(fp, fmt, ap);
 
 va_end(ap);
 
 return ret;
}

int main()
{
 char * ip = in_ntoa(1921681001); // input: 1921681001 dyaplay format: 105.134.138.114
 mini_printf("\n\t%s\n\n",ip);
 return 0;
}