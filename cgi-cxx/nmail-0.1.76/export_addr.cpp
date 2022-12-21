/*
	export_addr.cpp
*/

#include <stdio.h>

#include "addr.h"
#include "preferences.h"

//////////////////////////////////////////////////////////

int export_addr_action(void)
{
   int x=0;

   printf("Content-Type: application/octet-stream\n");
   printf("Content-Disposition: attachment; filename=\"addresses.csv\"\n\n");

   printf("Name,E-mail Address\n");

   if(user_pref.addr == NULL)
      return(-1);

   for(x=0; user_pref.addr[x].name || user_pref.addr[x].email; x++)
   {
      if(user_pref.addr[x].name != NULL)
         printf("%s",user_pref.addr[x].name);

      printf(",");

      if(user_pref.addr[x].email != NULL)
         printf("%s",user_pref.addr[x].email);

      printf("\n");
   }

   return(0);
}

//////////////////////////////////////////////////////////


