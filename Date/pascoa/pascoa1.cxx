#include <iostream>
/*
.--------.-------------------------------.----------------------------------.
|Formato |Britânico:                     |Americano:                        |
|        |Dia - Mês - Ano                |Mês - Dia - Ano                   |
|--------+-------------------------------+----------------------------------|
|    A   | the Twenty-fourth of February,| February, the Twenty-fourth,2009 |
|        |             2009              |                                  |
|--------+-------------------------------+----------------------------------|
|    B   |     24th February 2009        |     February 24th, 2009          |
|    C   |      24 February 2009         |      February 24, 2009           |
|    D   |          24/2/2009            |          2/24/2009               |
|    E   |           24/2/09             |           2/24/09                |
|    F   |          24/02/09             |          02/24/09                |
'--------'-------------------------------'----------------------------------'
*/

int main()
{
  time_t now = time(0);
  tm *ltm = localtime(&now);
  
  int current_day(ltm->tm_mday);
  int current_month(ltm->tm_mon+1);
  int current_year(ltm->tm_year+1900);
  
  const char *months[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
  
  printf("\n\tThe current year is %4d\n",current_year);
  
  int year;
  
  printf("\n\tEnter the desired year to\n\tcalculate the day of Easter: ");
  scanf("%d", &year);
  
  int a=int(year%19);
  int b=int(year/100);
  int c=int(year%100);
  int d=int(b/4);
  int e=int(b%4);
  int f=int((b+8)/25);
  int g=int((b-f+1)/3);
  int h=((19*a+b-d-g+15)%30);
  int i=int(c/4);
  int j=int(c%4);
  int k=((32+2*e+2*i-h-j)%7);
  int l=int((a+11*h+22*k)/451);
  int easter_month=int((h+k-7*l+114)/31);
  int easter_day=int((h+k-7*l+114)%31)+1;
 
 if(year > current_year)
  {
   printf("\n\tEaster will fall on %s %dth %4d.\n\n", months[easter_month-1],easter_day,year);
  }
 else
 if(year < current_year)
  {
   printf("\n\tEaster fell on %dth %s %4d.\n\n",easter_day, months[easter_month-1],year);
  }
 else
 if(year == current_year && current_month == easter_month && current_day == easter_day)
  {
   printf("\n\tToday %dth %s %04d is Easter Day",easter_day, months[easter_month-1], year);
  }
 else
 if(year == current_year && current_month > easter_month)
  {
   printf("\n\tEaster fell on %dth %s %4d.\n\n",easter_day, months[easter_month-1],year);
  }
 else
 if(year == current_year && current_month < easter_month)
  {
   printf("\n\tEaster will fall on %s %dth %4d.\n\n", months[easter_month-1],easter_day,year);
  }
 else
 if(year == current_year && current_month == easter_month && current_day < easter_day)
  {
   printf("\n\tEaster will fall on %s %dth %4d.\n\n", months[easter_month-1],easter_day,year);
  }
 else
 if(year == current_year && current_month == easter_month && current_day > easter_day)
  {
   printf("\n\tEaster fell on %dth %s %4d.\n\n", months[easter_month-1],easter_day,year);
  }    
  return 0;
}