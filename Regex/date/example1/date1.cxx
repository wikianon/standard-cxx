#include <regex>
#include <iostream>






















int main()
{
 std::tm _tm;
 std::smatch result;
 
 std::string data("On 12.01/2015, the shop will be closed, but on 2016-01-03, it will be open. Invalid date: 2016-13-32. ID: 1022015-12-233333.");
 const std::regex pattern("(\\b\\d{2}[-|.|/]\\d{2}[-|.|/]\\d{4}\\b)|(\\b\\d{4}[-|.|/]\\d{2}[-|.|/]\\d{2}\\b)");
 
 while(std::regex_search(data, result, pattern)) 
  {
   if(strptime(result[0].str().c_str(), "%d.%m.%Y", &_tm) != NULL)
    std::cout << result[0] << std::endl;
   else
   if(strptime(result[0].str().c_str(), "%Y-%m-%d", &_tm) != NULL)
    std::cout << result[0] << std::endl;
            
   data = result.suffix().str();
  }
 return 0;      
}