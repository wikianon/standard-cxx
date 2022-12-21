#include <stdio.h>

const char *wd1(int day, int month, int year)
{
  /* using C99 compound literals in a single line: notice the splicing */
  return ((const char *[])                                                \
          {"Monday", "Tuesday", "Wednesday",                              \
           "Thursday", "Friday", "Saturday", "Sunday"})[                  \
          (                                                               \
           day + ((153 * (month + 12 * ((14 - month) / 12) - 3) + 2) / 5) \
           + (365 * (year + 4800 - ((14 - month) / 12)))                  \
           + ((year + 4800 - ((14 - month) / 12)) / 4)                    \
           - ((year + 4800 - ((14 - month) / 12)) / 100)                  \
           + ((year + 4800 - ((14 - month) / 12)) / 400) - 32045          \
          ) % 7];
}

const char *wd2(int day, int month, int year)
{
 static const char *weekdays[] =
  {
   "Monday",
   "Tuesday",
   "Wednesday", 
   "Thursday",
   "Friday",
   "Saturday",
   "Sunday"
  };

  size_t JND = day + ((153 * (month + 12 * ((14 - month) / 12) - 3) + 2) / 5) \
               + (365 * (year + 4800 - ((14 - month) / 12)))                  \
               + ((year + 4800 - ((14 - month) / 12)) / 4)                    \
               - ((year + 4800 - ((14 - month) / 12)) / 100)                  \
               + ((year + 4800 - ((14 - month) / 12)) / 400) - 32045;

  return weekdays[JND % 7];
}

int main(void)
{
 printf("\n\t%02d/%02d/%4d foi %s", 10, 03, 2018, wd1(10, 03, 2018));
 printf("\n\t%02d/%02d/%4d foi %s\n\n", 10, 03, 2018, wd2(10, 03, 2018));
 return 0;
} 
