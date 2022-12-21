#ifndef _DATE_HXX
#define _DATE_HXX

#include <iostream>

struct Date
{
 std::string day="";
 std::string month="";
 std::string year="";
 
 Date();
   
 ~Date();
    
 std::string toString();
   
 std::string currentDateTime();
   
 private:
     
 friend std::ostream& operator<<(std::ostream&, const Date&);
  
 time_t currentTime = time(NULL);
 tm * timeInfo = localtime(&currentTime);
};


#endif