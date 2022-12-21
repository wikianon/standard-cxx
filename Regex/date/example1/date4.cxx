#include <regex>
#include <iostream>




















int main()
{
 constexpr char text[]{"29-feb/2020"};
 
 const std::regex re(
                     R"((?:(?:(0?[1-9]|1\d|2[0-8])([-/.])(0?[1-9]|1[0-2]|j(?:an|u[nl])|ma[ry]|a(?:pr|ug)|sep|oct|nov|dec|feb)|(29|30)([-/.])(0?[13-9]|1[0-2]|j(?:an|u[nl])|ma[ry]|a(?:pr|ug)|sep|oct|nov|dec)|(31)([-/.])(0?[13578]|1[02]|jan|ma[ry]|jul|aug|oct|dec))(?:\2|\5|\8)(0{2,3}[1-9]|0{1,2}[1-9]\d|0?[1-9]\d{2}|[1-9]\d{3})|(29)([-/.])(0?2|feb)\12(\d{1,2}(?:0[48]|[2468][048]|[13579][26])|(?:0?[48]|[13579][26]|[2468][048])00)))",
                     std::regex_constants::icase
                    );
 
 std::smatch match_date;
 
 if(!std::regex_match(text, match_date, re))std::cout << "\n\tData inválida!!\n\n";
 else
  {
   std::cout << "\n\tDate: " << match_date[0]
             << "\n\tDia: " << match_date[1]  << match_date[4]  << match_date[7]  << match_date[11]
             << "\n\tMês: " << match_date[3]  << match_date[6]  << match_date[9]  << match_date[13]
             << "\n\tAno: " << match_date[10] << match_date[14] 
             << "\n\n";
  }
 return 0;
}