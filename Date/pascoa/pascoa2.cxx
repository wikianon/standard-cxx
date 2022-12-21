#include <iostream>

int main()
{
 time_t now = time(0);
 tm *ltm = localtime(&now);
 
 int current_day(ltm->tm_mday);
 int current_month(ltm->tm_mon+1);
 int current_year(ltm->tm_year+1900);

 printf("\n\tThe current year is %4d\n",current_year);
  
 int year;
  
 printf("\n\tEnter the desired year to\n\tcalculate the day of Easter: ");
 scanf("%d", &year);
 
 int a=(year%19);
 int b=int(year/100);
 int c=(year%100);
 int d=int(b/4);
 int e=(b%4);
 int f=int((b+8)/25);
 int g=int((b-f+1)/3);
 int h=((19*a+b-d-g+15)%30);
 int i=int(c/4);
 int j=(c%4);
 int k=((32+2*e+2*i-h-j)%7);
 int l=int((a+11*h+22*k)/451);
 int easter_month=int((h+k-7*l+114)/31);
 int easter_day=int((h+k-7*l+114)%31)+1;
 
 enum { January=1, February, March, April, May, June, July, August, September, October, November, December };

 const char* months = "Month Invalid"; //aqui nova variável para o texto do easter_month

 switch(easter_month)
  {
   case January:   months = "January";   break;
   case February:  months = "February";  break;
   case March:     months = "March";     break;
   case April:     months = "April";     break;
   case May:       months = "May";       break;
   case June:      months = "June";      break;
   case July:      months = "July";      break;
   case August:    months = "August";    break;
   case September: months = "September"; break;
   case October:   months = "October";   break;
   case November:  months = "November";  break;
   case December:  months = "December";  break;
  }
 
 if(year > current_year)
  {
   printf("\n\tEaster will fall on %s %dth %4d.\n\n", months,easter_day,year);
  }
 else
 if(year < current_year)
  {
   printf("\n\tEaster fell on %dth %s %4d.\n\n",easter_day, months,year);
  }
 else
 if(year == current_year && current_month == easter_month && current_day == easter_day)
  {
   printf("\n\tToday %dth %s %04d is Easter Day\n\n",easter_day, months, year);
  }
 else
 if(year == current_year && current_month > easter_month)
  {
   printf("\n\tEaster fell on %dth %s %4d.\n\n",easter_day, months,year);
  }
 else
 if(year == current_year && current_month < easter_month)
  {
   printf("\n\tEaster will fall on %s %dth %4d.\n\n", months,easter_day,year);
  }
 else
 if(year == current_year && current_month == easter_month && current_day < easter_day)
  {
   printf("\n\tEaster will fall on %s %dth %4d.\n\n", months,easter_day,year);
  }
  
 return 0;
}