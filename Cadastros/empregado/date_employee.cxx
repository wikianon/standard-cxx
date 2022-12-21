#include <iostream>

enum { manager, laborer, secretary };

struct Date
{
 void Set()
 {
  std::cout << "\n\tEnter In The Year: ";
  std::cin >> year;
  std::cout << "\n\tEnter In The Month: ";
  std::cin >> month;
  std::cout << "\n\tEnter In The Day: ";
  std::cin >> day;
   
  if(!!Validate())
   {
    std::cout << "\n\n";
    Set();
   }
 }

 void Set(int yearx, int monthx, int dayx)
 {
  year = yearx;
  month = monthx;
  day = dayx;
  
  if(!!Validate())
   {
    std::cout << "\n\n";
    Set();
   }
 }

 void Display()
  {
   std::cout << year << ":" << month << ":" << day << "\n\n";
  }

 bool Validate()
  {
   int days_in_month[] = {31, 28 + (year % 4 == 0), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
   
   if(year >= 1900)
    {
     if(month >= 1 && month <= 12)
      {
       if(day >= 1 && day <= days_in_month[month - 1])
        {
         return false;
        }
        else
        {
         std::cout << "\n\t[ERROR] Day range ( 1 to " << days_in_month[month - 1] << " )\n"; 
         return true;
        }
      }
      else
      {
       std::cout << "\n\t[ERROR] Month range ( 1 to 12 )\n"; 
       return true;
      }
    }
    else
    {
     std::cout << "\n\t[ERROR] Year is to small (1900)\n";
     return true;
    }
  }
 
 private:
  int year, month, day; 
};


struct Employee
{
 void Set()
 {
  std::cout << "\n\tEnter In The Employee's Number: ";
  std::cin >> e_number;
  std::cout << "\n\tEnter In The Employee's Salary R$: ";
  std::cin >> e_salary;
  std::cout << "\n\t[0] Manager,  [1] Laborer, [2] Secretary\n\tEnter In The Employee's Job Category: ";
  std::cin >> e_job;
  
  if(!!Validate())
   {
    std::cout << "\n\n"; 
    Set();
   }
   dl.Set();
 }

 void Set(int e_numberx, double e_salaryx, int e_jobx, Date dlx)
 {
  e_number = e_numberx;
  e_salary = e_salaryx;
  e_job = e_jobx;
  dl = dlx;
  
  if(!!Validate())
   {
    std::cout << "\n\n"; 
    Set();
   }
 }

 void Display()
 {
  std::cout << "\n\n\tEmployee's Number: " << e_number
            << "\n\tEmployee's Salary R$: " << e_salary
            << "\n\tEmployee's Job Category: ";
  
  switch(e_job)
   {
    case manager:
     std::cout << "Manager\n";
    break;
    
    case laborer:
     std::cout << "Laborer\n";
    break;
    
    case secretary:
     std::cout << "Secretary\n";
    break;
   }
  std::cout << "\n\tDate of employed: ";
  dl.Display();
 }
 
 bool Validate()
 {
  if(e_number > 0 && e_number < 100000000)
   {
    if(e_salary > 0 && e_salary < 1000000)
     {
      if(e_job > -1 && e_job < 3){ return false; }
      else
      {
       std::cout << "\n\tEmployee's Category 0 to 2)\n";
       return true;
      }
     }
     else
     {
      std::cout << "\n\tEmployee's Salary 1 to 999999)\n";
      return true;
     }
   }
   else
   {
    std::cout << "\n\tEmployee's Number 1 to 999999999)\n";
    return true;
   }
 }

 private:
  int e_number, e_job, e_dateofhire; 
  double e_salary;
  Date dl;
};//End of Employee

int main()
{
 Employee dl, dl2;
 Date dx;

 dx.Set(2004, 2, 29);
 dl.Set(1023, 8555, 0, dx);
 dl.Display();

 dl2.Set();
 dl2.Display();
 
 return 0;
}