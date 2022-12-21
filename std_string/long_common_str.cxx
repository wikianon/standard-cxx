#include <set>
#include <vector>
#include <iostream>
#include <algorithm>

//https://github.com/ryukinix/cpp-journey/blob/master/Inheritance/LongestCommonSubstring.cpp
void findSubstrings(const std::string& word , std::set<std::string>& substrings)
{
 int l = word.length();
 
 for(int start = 0; start < l; start++)
  for(int length = 1; length < l - start + 1; length++)
   substrings.insert(word.substr(start, length));
}

std::string lcs(const std::string& first, const std::string& second)
{
   std::set<std::string> firstSubstrings;
   std::set<std::string> secondSubstrings;
   
   findSubstrings(first, firstSubstrings);
   findSubstrings(second, secondSubstrings);
   
   std::set<std::string> common;
   
   std::set_intersection(
                         firstSubstrings.begin(),
                         firstSubstrings.end(),
                         secondSubstrings.begin(),
                         secondSubstrings.end(),        
                         std::inserter(common, common.begin())
                        );
   
   std::vector<std::string>
              commonSubs(
                         common.begin(),
                         common.end()
                        );
   
 std::sort(
           commonSubs.begin(),
           commonSubs.end(),
           [](const std::string &s1, const std::string &s2)
            {
             return s1.length() > s2.length();
            }
          );
 
 return *commonSubs.begin();
}

int main( )
{
 //s1 substring a ser buscada
 std::string s1("@");
   
 //s2 mensagen para encontrar a string
 std::string s2("usuario@gmail.com") ;
 std::cout << "The longest common substring of " << s1 << " and " << s2 << " is:\n";
 std::cout << lcs(s1, s2) << " !\n\n";
 
 return 0 ;
}