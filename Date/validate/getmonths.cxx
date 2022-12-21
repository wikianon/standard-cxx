#include <map>
#include <iterator>
#include <iostream>

std::map<std::string, int> months;
std::map<std::string, int>::iterator iter;

//take value of the month through the name
int GetValue(std::string vals)
{
 return months[vals];
}

//take the name of the month through your number
std::string GetName(int value)
{
 for(iter = months.begin(); iter != months.end(); iter++ )
  if( value == iter->second )
   return (std::string)iter->first;
    return "Illegal value";   //Error Illegal input
}

void initMonths()
{
 months["January"]  =  1;
 months["February"] =  2;
 months["March"]    =  3;
 months["April"]    =  4;
 months["May"]      =  5;
 months["June"]     =  6;
 months["July"]     =  7;
 months["August"]   =  8;
 months["Setember"] =  9;
 months["October"]  = 10;
 months["November"] = 11;
 months["December"] = 12;
}
void getMont(std::string name, int value)
{
 initMonths();

 std::cout<<"\n\tMonth value by name: "<<GetValue(name)
          <<"\n\n\tName of the month\n\twith your number: "<<GetName(value)
          <<"\n\n";
}

int main()
{
 int num; 
 std::string str;
 
 std::cout<<"Enter the number of the month: ";
 std::cin>>num;
 
 std::cout<<"Enter the name of the month in English: ";
 std::cin>>str;
 
 getMont(str, num);
}