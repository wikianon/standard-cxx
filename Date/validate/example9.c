#include <stdio.h>



























int main()
{
 enum { January=1, February, March, April, May, June, July, August, September, October, November, December}months;
 
 const char *monthName[12] = {
                              "January", "February", "March",
                              "April", "May", "June", "July",
                              "August", "September", "October",
                              "November", "December"
                             };
   
 for(months = January; months <= December; months++)
 printf("%2d%11s\n", months, monthName[months-1]);

 return 0;
}