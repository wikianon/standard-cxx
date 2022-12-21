#include <stdio.h>
#include <stdlib.h>


//contador progressivo
int main()
{
 int hr = 0;
 
 int min = 0;
 
 int sec = 0;
 
 int mm = 0;
 
 //for(int i = 0; i < 5; --i)
 while(1)
  {
   printf("\n\t%02dh:%02dm:%02ds",hr,min,sec);
   
   #if defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
   
   system("cls");
   
   #elif defined(__linux__) || defined(__gnu_linux__) || defined(__unix__)
   
   system("clear");
   
   #endif
   
   mm++;
   if(mm == 285) // faz com que os segundos sejan contados no mesmo tempo que o relogio do computador
   {
    mm=0;
    sec++;
   }
   
   if(sec == 60)
    {
     sec=0;
     min++;
    }
    
   if(min == 60)
    {
     min=0;
     hr++;
    }
    
   if(hr == 12)
    {
     hr=0;
    }
  }
 return 0;
}