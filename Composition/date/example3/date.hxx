#ifndef _DATE_HXX
#define _DATE_HXX

#include <iostream>

struct date
{
 int _day;
 int _month;
 int _year; 
 
 date(int d, int m, int y): _day(d), _month(m), _year(y){}
 
 int day()  { return _day;   }
 int month(){ return _month; }
 int year() { return _year;  }
 
 date()
 {
  _day   = now->tm_mday;
  _month = now->tm_mon+1;
  _year  = now->tm_year+1900;
 }
 
 ~date()
 {
  _day   = 0;
  _month = 0;
  _year  = 0;
 }
 
 friend std::ostream& operator<<(std::ostream& os, const date &d)
 {
  return os << d._day << "/" << d._month << "/" << d._year;
 }
 
 private:
  
  time_t tms = time(NULL);
  tm *now = localtime(&tms);
};


#endif