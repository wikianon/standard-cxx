#include <regex>
#include <iostream>
//Real number

bool isReal(std::string input)
{
 std::smatch number_smatch;
 
 const std::regex pattern("((\\+|-)?[[:digit:]]+)[.](([[:digit:]]+)?)?((e|E)((\\+|-)?)[[:digit:]]+)?");
 //As long as the input is correct ask for another number
  
 return std::regex_match(input, number_smatch, pattern);
}

void getNumber()
{
 std::string input;
 
 do{
    std::cout<<"\n\tDigite um numero real(float or double): ";
    std::cin>>input;
   
    std::cout<<"\n\tNumber: "+input+(isReal(input)?" is Valid\n\n":" is Invalid\n");
    
   }while(!isReal(input));
}













int main()
{
 getNumber();
 return 0;
}