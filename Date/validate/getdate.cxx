#include <iostream>

struct GetDate
{
  struct Weeks
   {
    const char* longDays[7]={ "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
   }week;
   
  struct Months
   {
    const char* longMonths[12]={ "January", "February", "March", "April", "May","June", "July", "August", "September", "October", "November", "December" };
   }month;
   
  int Month()
   {
    if(&timeInfo == NULL)return -1;
    else
    return timeInfo.tm_mon+1;
   }
  
  int Year()
   {
    if(&timeInfo == NULL)return -1;
    else
    return timeInfo.tm_year+1900;
   }
   
  int Day()
   {
    if(&timeInfo == NULL)return -1;
    else
    return timeInfo.tm_mday;
   }
   
  int dayWeek(int day, int month, int year)
   {
    timeInfo.tm_year = year - 1900;
    timeInfo.tm_mon  = month - 1;
    timeInfo.tm_mday = day;
    timeInfo.tm_hour = 12;
    mktime(&timeInfo);
    return timeInfo.tm_wday;
   }
   
  std::string theMonth()
   {
    //Testa o resultado e retorna
    switch(Month())
     {
      case Jan: return shortMonths[Month()-1]; break;
      case Feb: return shortMonths[Month()-1]; break;
      case Mar: return shortMonths[Month()-1]; break;
      case Apr: return shortMonths[Month()-1]; break;
      case May: return shortMonths[Month()-1]; break;
      case Jun: return shortMonths[Month()-1]; break;
      case Jul: return shortMonths[Month()-1]; break;
      case Aug: return shortMonths[Month()-1]; break;
      case Sep: return shortMonths[Month()-1]; break;
      case Oct: return shortMonths[Month()-1]; break;
      case Nov: return shortMonths[Month()-1]; break;
      case Dec: return shortMonths[Month()-1]; break;
     }
     
   }
   
  std::string dayWeek()
   {
    //Algoritmo para pegar o dia da semana
    int a = ((12 - Month()) / 10);
    int b = (Year() - a);
    int c = (Month() + (12 * a));
    int d = (b/100);
    int e = (d/4);
    int f = ((2-d) + e);
    int g = int(365.25 * b);
    int h = int(30.6001 * (c + 1));
    int i = int((f + g) + (h + Day()) + 5);
    int j = int(i % 7); //Resto de I por 7, onde 0=Saturday, 1=Sunday, 2=Monday, 3=Tuesday, 4=Wednesday, 5=Thursday, 6=Friday
   
    //Testa o resultado e retorna
    switch(j)
     {
      case Zero:  return shortDays[j-1]; break;
      case One:   return shortDays[j-1]; break;
      case Two:   return shortDays[j-1]; break;
      case Three: return shortDays[j-1]; break;
      case Four:  return shortDays[j-1]; break;
      case Five:  return shortDays[j-1]; break;
      case Six:   return shortDays[j-1]; break;
     }
   }
   
  private:
  
  time_t timeval = time(NULL);
  
  tm timeInfo = *localtime(&timeval);
  
  enum { Zero, One, Two, Three, Four, Five, Six };
  
  const char* shortDays[7]={ "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
  
  enum { Jan=1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec };
  
  const char* shortMonths[12]={ "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec" };
}get;


int main()
{
 std::cout<<"\n\tshort Weekday: "<<get.dayWeek()<<" "<<get.Day()<<"/"<<get.theMonth()<<"/"<<get.Year()<<"\n";
 
 std::cout<<"\n\tlong Weekday: "<<get.week.longDays[get.dayWeek(get.Day(),get.Month(),get.Year())]
          <<" "<<get.Day()<<"/"<<get.month.longMonths[get.Month()-1]<<"/"<<get.Year()<<"\n\n";
 
 return 0;
}