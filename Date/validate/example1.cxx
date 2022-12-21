#include <limits> //numeric_limits
#include <iostream>
//https://codereview.stackexchange.com/questions/67636/is-it-friday-yet
//dayOfWeek.cpp
enum { January=1, February, March, April, May, June, July, August, September, October, November, December};

/**
 * Makes sure data isn't malicious, and signals user to re-enter proper data if invalid
 * getSanitizedNum
 */
int getInt()
{
    int input = 0;
    //while(!(std::cin>>input))
    while((std::cin>>input).fail())  
    {
        // clear the error flag that was set so that future I/O operations will work correctly
        std::cin.clear();
        // skips to the next newline
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\n\tInvalid input.\n\tPlease enter a positive number: ";
    }
    return input;
}
 
/**
 * Safetly grabs and returns a lowercase version of the character (if the lowercase exists)
 * getSanitizedChar
 */
char32_t getChar()
{
    // absorb newline character (if existant) from previous input
    if('\n' == std::cin.peek()) std::cin.ignore();
    return std::tolower(std::cin.get());
}
 
int getCenturyValue(int year)
{
    return (2 * (3 - div(year / 100, 4).rem));
}
 
int getYearValue(int year)
{
    int mod = year % 100;
    return (mod + div(mod, 4).quot);
}
 
int getMonthValue(int month, int year)
{
  const static int LEAP_VALS[] = { 0xDEAD, 6, 2, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5 };
  
  const static int NON_LEAP_VALS[] = { 0xDEAD, 0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5 };
  
  return (__isleap(year) ? LEAP_VALS[month] : NON_LEAP_VALS[month]);
}
/*
int getMonthValue(int month, int year)
{
    switch (month)
    {
        case January:
            if (__isleap(year)) return 6;
        case October:
            return 0;
        case May:
            return 1;
        case August:
            return 2;
        case February:
            if (__isleap(year)) return 2;
        case March:
        case November:
            return 3;
        case June:
            return 4;
        case September:
        case December:
            return 5;
        case April:
        case July:
            return 6;
        default:
            return -1;
    }
}

int dayOfWeek(int day, int month, int year)
{
 return div(day + getMonthValue(month, year) + getYearValue(year) + getCenturyValue(year), 7).rem;
}
*/

int dayOfWeek(int day, int month, int year)
{
    static int months[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    year -= month < 3;
    return ( year + year/4 - year/100 + year/400 + months[month-1] + day) % 7;
}

void getInput(int &day, int &month, int &year)
{
 tm *atual; 
 time_t curTime = 0; // Amount of seconds since 1 jan 1970
 
 int meses[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
 
 const char *strmes[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
 
 do{
    do{
       std::cout << "\n\tEnter the day (1-31): ";
       day = getInt();
      }while(day < 1 || day > 31);
     
    do{  
       std::cout << "\n\tEnter the month (1-12): ";
       month = getInt();
      }while(month < 1 || month > 12);
     
    do{  
       std::cout << "\n\tEnter the year: ";
       year = getInt();
      }while(year < 1100);
     
      if(__isleap(year))meses[1] = 29; // atualiza dia+1 caso  ano seja bisexto
      
     
    //Pega a hora atual e converte em uma estrutura tm
    time(&curTime);
    atual = localtime(&curTime);
    
    //Atualiza os valores dia, mes e ano da estrutura tm
    atual->tm_mday = day;
    atual->tm_mon  = month-1;   /* Mes vai de 0 a 11 */
    atual->tm_year = year-1900; /* Ano inicial é 1900 */
    
    //Atualizar o dia da semana (tm_wday) e dia do ano (tm_yday), de acordo com os atualizados anteriormente
    mktime(atual);
    
    if(day > meses[month-1])std::cout<<"\n\t"<<strmes[atual->tm_mon-1]<<" of year "<<year<<" does not have "<<day<<" days!!!\n\n";
    
   }while(day > meses[month-1]);
}

void getDetails()
{
 int day   = 0;
 int month = 0;
 int year  = 0;
 
 const char* weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
 
 do{
    getInput(day, month, year);
    
    std::cout << "\n\tThe day of the week is " << weekDays[dayOfWeek(day, month, year)] << std::endl;
   
    std::cout << "\n\tRun the program again (y/N): ";  // signify n as default with capital letter
   
   }while('y' == getChar()); 
}

int main()
{
 getDetails();
 return 0;
}