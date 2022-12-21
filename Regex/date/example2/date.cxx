#include <regex>
#include <iostream>
//https://www.debuggex.com/r/xVhik3zVmXVNkEdc
struct date
{
 date(std::string date): _date(date){}
 
 ~date()
  {
   if(!_date.empty()) _date.clear();
  }
 
 bool isDate()
 {
  std::smatch date_smatch;
  //Regex valida anos bisextos e nao bisextos no formato yyyy/mm/dd ou yyyy/str/dd ou yyyy/Str/dd
  const std::regex pattern(
                           R"(^(?:\d{4}([-/.])(?:(?:(?:0?[13578]|1[02]|[Jj](?:an|ul)|[Mm]a[ry]|[Aa]ug|[Oo]ct|[Dd]ec)\1(?:0?[1-9]|[1-2][0-9]|3[01]))|(?:(?:0?[469]|11|[Aa]pr|[Jj]un|[Ss]ep|[Nn]ov)\1(?:0?[1-9]|[1-2][0-9]|30))|(?:(0?2|[Ff]eb)\1(?:0?[1-9]|1[0-9]|2[0-8])))|(?:(?:\d{2}(?:0[48]|[2468][048]|[13579][26]))|(?:(?:[02468][048])|[13579][26])00)([-/.])(0?2|[Ff]eb)\3(29))$)"
                          );
  
  /*//Regex valida anos bisextos e nao bisextos no formato dd/mm/yyyy ou dd/str/yyyy ou dd/Str/yyyy
  const std::regex pattern(
                           R"(^(?:(?:(0?[1-9]|1\d|2[0-8])([-/.])(0?[1-9]|1[0-2]|[Jj](?:an|u[nl])|[Mm]a[ry]|[Aa](?:pr|ug)|[Ss]ep|[Oo]ct|[Nn]ov|[Dd]ec|[Ff]eb)|(29|30)([-/.])(0?[13-9]|1[0-2]|[Jj](?:an|u[nl])|[Mm]a[ry]|[Aa](?:pr|ug)|[Ss]ep|[Oo]ct|[Nn]ov|[Dd]ec)|(31)([-/.])(0?[13578]|1[02]|[Jj]an|[Mm]a[ry]|[Jj]ul|[Aa]ug|[Oo]ct|[Dd]ec))(?:\2|\5|\8)(0{2,3}[1-9]|0{1,2}[1-9]\d|0?[1-9]\d{2}|[1-9]\d{3})|(29)([-/.])(0?2|[Ff]eb)\12(\d{1,2}(?:0[48]|[2468][048]|[13579][26])|(?:0[48]|[13579][26]|[2468][048])00))$)"
                          );
  */
  return std::regex_match(_date, date_smatch, pattern);
 }
 
 void print()
 {
  std::cout << "\n\tDate: "+_date+(isDate()?" is Valid\n":" is Invalid\n");
 }
 
 private:
  std::string _date;
};

int main()
{    
 std::string str;
    
 do{
    
    std::cout << "\n\tEnter date: ";
    getline(std::cin, str);
    
    date(str).print();
    std::cout << "\n";
    
   }while(!date(str).isDate());
  
  return 0;
}