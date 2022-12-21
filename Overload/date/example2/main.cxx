#include "date.hxx"

int main()
{
 Date date;
 
 std::cout << "\n\tThe date is Int "
           << "\n\tThe date Int Format..: " << date
           << "\n\tThe day is...........: " << date.day
           << "\n\tThe month is.........: " << date.month
           << "\n\tThe year is..........: " << date.year
           << "\n\n\tThe date is Str "
           << "\n\tThe date String Format..: " << date.toString()
           << "\n\tThe day is..............: " << date.day
           << "\n\tThe month is............: " << date.month
           << "\n\tThe year is.............: " << date.year
           << "\n\n";
  return 0;
}