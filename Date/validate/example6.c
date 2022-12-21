#include <time.h>
#include <stdio.h>





























int main ()
{
  const char *str = "15-08-2012";
  struct tm tm; 
  if (strptime (str, "%d-%m-%Y", &tm) == NULL)
  {
    /* Bad format !! */
  }
  char buffer [80];
  strftime (buffer, 80, "Day is %a", &tm);
  puts (buffer);    
  return 0;
}