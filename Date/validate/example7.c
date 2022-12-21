#include <time.h>
#include <stdio.h>




int valid_date(int dd, int mm, int yy)
{
 if (mm < 1 || mm > 12)return 0;
   
 if (dd < 1)return 0;
 
 int days = 31;
 
 if(mm == 2)
  {
   days = 28;
   
   if (__isleap(yy))days = 29;
   
  }
  else
  if(mm == 4 || mm == 6 || mm == 9 || mm == 11)days = 30;

  if(dd > days)return 0;
  return 1;
} 




int main()
{
 int dd,mm,yy;
 
 printf("write a date(day/month/year):");
 scanf("%d/%d/%d",&dd,&mm,&yy);
    
 printf("\n\tDate: %02d/%02d/%04d",dd, mm, yy);
    
 printf("%s\n", valid_date(dd, mm, yy) ? " is Valid\n" : " is Invalid\n");
 
 return 0;
}