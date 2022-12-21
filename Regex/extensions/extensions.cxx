#include <regex>
#include <iostream>

struct extensions
{ 
 extensions(std::string file): _file(file){}

 ~extensions()
  {
   if(!_file.empty())_file.clear();
  }
  
 bool isFile()
 {
  //validas arquivos com as extensoes da regex
  const std::regex pattern("^.*[.](aspx|p(hp|y)|java|rb|d|mp3|h?(h|pp|xx|\\++|tm|tml)|H?(H|PP|XX|\\++|TM|TML)|c?(c|pp|xx|\\++|ss|shtml)|C?(C|PP|XX|\\++|SS|SHTML))$");
  
  return std::regex_match(_file, pattern);
 }

 void print()
 {
  std::cout << "\n\tFile: "+_file+(isFile()?" is Valid\n":" is Invalid\n");
 }

 private:
  std::string _file;
};

int main()
{
 const char* files[33] = {
                          "file1.c",
                          "file2.cc",
                          "file3.cpp",
                          "file4.cxx",
                          "file5.c++",
                          "file6.C",
                          "file7.CC",
                          "file8.CPP",
                          "file9.CXX",
                          "file10.C++",
                          "file11.h",
                          "file12.hh",
                          "file13.hpp",
                          "file14.hxx",
                          "file15.h++",
                          "file16.H",
                          "file17.HH",
                          "file18.HPP",
                          "file19.HXX",
                          "file20.H++",
                          "file21.htm",
                          "file22.HTM",
                          "file23.HTML",
                          "file24.HTML",
                          "file25.aspx",
                          "file26.php",
                          "file27.py",
                          "file28.java",
                          "file29.rb",
                          "file30.d",
                          "file31.mp3",
                          "file32.css",
                          "file33.CCPP"
                         };
    
  for(int i=0; i<33; i++)
   extensions(files[i]).print();
    std::cout << "\n";
    
  return 0;   
}