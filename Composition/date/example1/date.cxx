#include <iostream>

struct Date
{
 std::string day="";
 std::string month="";
 std::string year="";
 
 std::string sday="";
 std::string smonth="";
 
   Date()
   {
    sday = std::to_string(now->tm_mday);
    if(sday.length() == 1)
    sday.insert(0,"0");
    
    smonth = std::to_string(now->tm_mon+1);
    if(smonth.length() == 1)
    smonth.insert(0,"0");
    
    day   = std::to_string(now->tm_mday);
    month = std::to_string(now->tm_mon+1);
    year  = std::to_string(now->tm_year+1900);
   }
   
   ~Date()
    {      
     sday.clear();
     smonth.clear();
     
     day.clear();
     month.clear();
     year.clear();
    }
   
   std::string toInt()
   {
    return day+"/"+month+"/"+year;
   }
   
   std::string toString()
   {
    return sday+"/"+smonth+"/"+year;
   }
   
   std::string toInt(std::string sep)
   {
    return day+sep+month+sep+year;
   }
   
   std::string toString(std::string sep)
   {
    return sday+sep+smonth+sep+year;
   }
   
   const std::string currentDateTime()
   {
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", now);

    return buf;
   }
   
   std::string dateTime()
   {
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", now);
    return buf;
   }
   
  private:
   char buf[50];
   time_t tms = time(NULL);
   tm *now = localtime(&tms);
   
}date;

int main()
{
 std::cout << "\n\tThe date is Int "
           << "\n\tThe day is..............: " << date.day
           << "\n\tThe month is............: " << date.month
           << "\n\tThe year is.............: " << date.year
           << "\n\tThe date Int Format.....: " << date.toInt()
           << "\n\tThe date Int Format Sep.: " << date.toInt("-")
           << "\n\n\tThe date is Str "
           << "\n\tdate time..................: " << date.dateTime()
           << "\n\tThe day is.................: " << date.sday
           << "\n\tThe month is...............: " << date.smonth
           << "\n\tThe date String Format.....: " << date.toString() 
           << "\n\tThe date String Format Sep.: " << date.toString("-")
           << "\n\n";
 return 0;
}