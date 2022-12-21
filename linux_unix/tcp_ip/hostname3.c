#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

char *addrtype(int addrtype)
{
 switch(addrtype)
  {
   case AF_INET:
    return "AF_INET";
   case AF_INET6:
    return "AF_INET6";
  }
  
 return "Unknown";
}

int main(int argc, char **argv)
{
 int i = 0; 
 struct hostent *tmp = 0;
 
  if(argc < 2)
   {
    printf("Usage: %s <hostname>\n", argv[0]);
    exit(EXIT_FAILURE);
   }
  
  tmp = gethostbyname(argv[1]);
  
  if(!tmp)
   {
    printf("Lookup Failed: %s\n", hstrerror(h_errno));
    return 0;
   }

  printf("Lookup: %s\n", argv[0]);
  printf("h_name: %s\n", tmp->h_name);
  
  i = 0;
  while(tmp->h_aliases[i] != NULL)
   {
    printf("h_aliases[i]: %s\n", tmp->h_aliases[i]);
    i++;
   }
  
  printf("h_addrtype: %d - %s\n", tmp->h_addrtype, addrtype(tmp->h_addrtype));
  printf("h_length: %d\n", tmp->h_length);
  
  i = 0;
  while(tmp->h_addr_list[i] != NULL)
   {
    printf("h_addr_list[i]: %s\n", inet_ntoa( (struct in_addr) *((struct in_addr *) tmp->h_addr_list[i])));
    i++;
   }
        
 return 0;
}
/*
 Lookup: ./host
 h_name: localhost
 h_addrtype: 2 - AF_INET
 h_length: 4
 h_addr_list[i]: 127.0.0.1
*/