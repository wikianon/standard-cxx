#ifndef _TIME_CXX
#define _TIME_CXX

#include "time.hxx"
   
Time::Time()
{ 
 time( &currentTime );
 timeInfo = localtime( &currentTime ); 
    
 hours   = std::to_string(timeInfo->tm_hour);
 minutes = std::to_string(timeInfo->tm_min);
 seconds = std::to_string(timeInfo->tm_sec);
}

Time::~Time()
{
 if(hours.length() != 0)
 hours.clear();
    
 if(minutes.length() != 0)
 minutes.clear();
    
 if(seconds.length() != 0)
 seconds.clear(); 
}

std::string Time::toString()
{
 if(hours.length() == 1)
 hours.insert(0, "0");
    
 if(minutes.length() == 1)
 minutes.insert(0, "0");
    
 if(seconds.length() == 1)
 seconds.insert(0, "0");
    
 return std::string(hours+":"+minutes+":"+seconds);
}

std::ostream& operator<<(std::ostream& os, const Time& mt)
{
 os << mt.hours << ":" << mt.minutes << ":" << mt.seconds;
 return os;
}

#endif