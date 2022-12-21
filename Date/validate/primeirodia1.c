#include <time.h>
#include <stdio.h>
#include <stdlib.h>






int main(void)
{
 const char *weekdays[7] = {
                            "Sunday",
                            "Monday",
                            "Tuesday",
                            "Wednesday", 
                            "Thursday",
                            "Friday",
                            "Saturday"
                           };
 
 const char *monthName[12] = {
                              "January", "February",
                              "March", "April",
                              "May", "June",
                              "July", "August",
                              "September", "October",
                              "November", "December"
                             };
      
 printf("Date (dd/mm/yyyy): ");
 
 int m, y, d;
 
 if(scanf("%d/%d/%d", &d, &m, &y) != 3)
  {
   fprintf(stderr, "Invalid input, brah\n");
   return EXIT_FAILURE;
  }
   
  /* Initialize to a sane default */
  time_t datime = time(NULL);
  struct tm *dt = localtime(&datime);
  
  /* Apply adjustments based on input */
  dt->tm_mday = d;
  dt->tm_mon = m-1;
  dt->tm_year = y-1900;
  
  /* Normalize the adjustments */
  datime = mktime(dt);
  dt = localtime(&datime);
    
  /* Rock on! */
  printf("\n\t%d de %s do ano %d é %s\n\n", d, monthName[dt->tm_mon], y, weekdays[dt->tm_wday]);
 
  return EXIT_SUCCESS;
}