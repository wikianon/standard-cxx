#ifndef _TIME_HXX
#define _TIME_HXX

#include <iostream>











struct Time
{
 std::string hours="";
 std::string minutes="";
 std::string seconds="";
 
 Time();
 
 ~Time();
   
 std::string toString();
  
 private:
    
 friend std::ostream& operator<<(std::ostream&, const Time&);
  
 time_t currentTime=0;
 tm *timeInfo;   
};









#endif