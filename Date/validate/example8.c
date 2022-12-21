#include <time.h>
#include <stdio.h>

int isValid(int dd, int mm, int yy)
{
 if (dd <= 0) return 0;
    
 switch (mm)
  {
   case 1:
   case 3:
   case 5:
   case 7:
   case 8:
   case 10:
   case 12:
    return dd <= 31;
   break;
   
   case 4:
   case 6:
   case 9:
   case 11:
    return dd <= 30;
   break;
   
   case 2:
    return dd <= 28 + (yy % 4 == 0);
   break;
   
   default:
    return 0;  /* invalid month */
  }
}

int main()
{
 int dd,mm,yy;
 
 if ( 3 == scanf("%d/%d/%d",&dd,&mm,&yy) && isValid(dd, mm, yy) )
 {
  puts("Valid date");
 }
 else
 {
  puts("Invalid date");
 }
 
 return 0;
}