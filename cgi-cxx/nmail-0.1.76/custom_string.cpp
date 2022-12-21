/*
	custom_string.cpp
*/

#include <stdlib.h>
#include <stdio.h>

///////////////////////////////////////////////////////////
// Searches forward
// Returns pointer to first char in string that is not member of skipset
//    returns NULL if all characters members of skipset

char* strpcbrk(const char* string, const char* skipset)
{
   char* c;
   char* skip;

   for(c=(char*)string; *c != '\0'; c++)
   {
      for(skip=(char*)skipset; *skip != '\0'; skip++)
         if(*c == *skip)
            break;

      if(*skip == '\0')
         return(c);
   }

   return(NULL);
}

///////////////////////////////////////////////////////////
// Searches backward
// Returns pointer to char in string that is member of stopset
//    returns NULL if no char found in stopset

char* strrpbrk(const char* string, const char* stopset)
{
   char* c;
   char* stop;

   for(c=(char*)string; *c != '\0'; c++);

   c--;

   for(;c >= string; c--)
      for(stop=(char*)stopset; *stop != '\0'; stop++)
         if(*c == *stop)
            return(c);

   return(NULL);
}

///////////////////////////////////////////////////////////
// Returns pointer to char in string that is NOT member of skipset
//    returns NULL if all characters members of skipset
// Searches backward

char* strrpcbrk(const char* string, const char* skipset)
{
   char* c;
   char* skip;

   for(c=(char*)string; *c != '\0'; c++);

   c--;

   for(; c>=string; c--)
   {
      for(skip=(char*)skipset; *skip != '\0'; skip++)
         if(*c == *skip)
            break;

      if(*skip == '\0')
         return(c);
   }

   return(NULL);
}

///////////////////////////////////////////////////////////

