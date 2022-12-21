#include "time.hxx"

int main()
{
 Time times;
 
 std::cout << "\n\n\n\tThe time is Int "
           << "\n\tThe hour is.............: " << times.hours
           << "\n\tThe minute is...........: " << times.minutes
           << "\n\tThe seconds is..........: " << times.seconds 
           << "\n\tThe time is Int Format..: " << times
           << "\n\n\tThe time is Str "
           << "\n\tThe time String Format..: " << times.toString() 
           << "\n\tThe hour is.............: " << times.hours
           << "\n\tThe minute is...........: " << times.minutes
           << "\n\tThe seconds is..........: " << times.seconds
           << "\n\n";  
  return 0; 
}
