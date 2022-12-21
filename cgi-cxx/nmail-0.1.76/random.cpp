/*
	random.cpp
*/

#include <stdio.h>
#include <stdlib.h>

char* get_rand_string(int);


///////////////////////////////////////////////////////////////////
//This function will return a random ascii string read from /dev/urandom
//      consisting entirely of alpha_numeric characters
//   string is allocated with malloc
//   returns NULL on error

char* get_rand_string(int length)
{
   char* buff=NULL;
   FILE* file=NULL;
   int x=0;
   char c=0;

   if(length <= 0 || length > 1000)
      return(NULL);

   file=fopen("/dev/urandom","r");

   if(file == NULL)
      return(NULL);

   buff=(char*)malloc(length+1);

   if(buff == NULL)
   {
      fclose(file);
      return(NULL);
   }

   for(x=0;x<length;x++)
   {
      do
      {
         c=getc(file);
      }
      while( ! ( (c >= '0' && c <= '9') ||
		 (c >= 'A' && c <= 'Z') ||
                 (c >= 'a' && c <= 'z') ) );

      buff[x]=c;
   }

   buff[length]='\0';
   fclose(file);

   return(buff);
}

///////////////////////////////////////////////////////////////////



