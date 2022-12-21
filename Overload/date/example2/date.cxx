#ifndef _DATE_CXX
#define _DATE_CXX

#include "date.hxx"

   std::string Date::currentDateTime()
   {
    return asctime(timeInfo);
   }
   
   Date::Date()
   {
    day   = std::to_string(timeInfo->tm_mday);
    month = std::to_string(timeInfo->tm_mon+1);
    year  = std::to_string(timeInfo->tm_year+1900);
   }
   
   Date::~Date()
   {
    day.clear();
    month.clear();
    year.clear();
   }
   
   std::string Date::toString()
   {
    if(day.length() == 1)
    day.insert(0,"0");
    
    if(month.length() == 1)
    month.insert(0,"0");
    
    return std::string(day+"/"+month+"/"+year);
   }
   
   std::ostream& operator<<(std::ostream& os, const Date& d)
   {
    os << d.day << "/" << d.month << "/" << d.year;
    return os;
   }
   
#endif