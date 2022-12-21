#include <iostream>
//Julian Date
//https://calendar.zoznam.sk/julian_calendar-sp.php?ly=1987#March
int GetDates(int day, int month, int year) // Function that calculates the julian date.
{
 long intRes1((2 - year / 100) + (year / 400)); // Calculation formula.
 long dias_totais = (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)) ? 366 : 365.25; 
 long intRes2((int)(dias_totais * year));
 long intRes3((int)(30.6001 * (month + 1)));

 int jdn(intRes1 + intRes2 + intRes3 + day + 1720994.5);

 return jdn;
}

int main()
{
 int day;
 int month; //Declare variables
 int year;
 int dayOfWeek=0;
 
 do{
    std::cout << "\n\tPlease enter the day (dd): "; // Gets day input from user.
    std::cin >> day;
    
    if(day < 01 || day > 31)
    std::cout << "\n\tInvalid day! Please enter a day "
              << "less from 1-31: "; // Checks for invalid input.
      
   }while(day < 01 || day > 31);
   
 do{
    std::cout << "\n\tPlease enter the month (mm): "; // Gets month input from user.
    std::cin >> month;
    
    if(month < 01 || month > 12)
    std::cout << "\n\tInvalid month! Please enter a month "
              << "from 01-12: "; // Checks for invalid input.
         
   }while(month < 01 || month > 12);
   
  std::cout << "\n\tPlease enter the year (yyyy): "; // Gets year input from user.
  std::cin >> year;
   
  dayOfWeek=((GetDates(day,month,year) + 1)%7); // calculates day of the week
  
  const char * days = "Day Invalid.";
  
  enum {Monday=0, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday};
  
  switch(dayOfWeek)
   {
    case Monday:
    days = "Monday";
    break;
    
    case Tuesday:
    days = "Tuesday";    
    break;
    
    case Wednesday:
    days = "Wednesday";
    break;
    
    case Thursday:
    days = "Thursday";
    break;
    
    case Friday:
    days = "Friday";
    break;
    
    case Saturday:
    days = "Saturday";
    break;
    
    case Sunday:
    days = "Sunday";
    break;
   }

   std::cout <<"\n\t" << days << " is " << dayOfWeek+1 << " day on date " << day << "/" // Outputs day of week results.
             << month << "/" << year << "\n\n";
  
   return 0;
}