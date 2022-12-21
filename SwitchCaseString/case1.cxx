#include <iostream>

constexpr unsigned int strToInt(const char* str, int h = 0)
{
    // not my code but can't remember where I got it from
    return !str[h] ? 5381 : (strToInt(str, h+1)*33) ^ str[h];
}

int main(int argc, char** argv) 
{
 
 std::string str;
 
 do{
    std::cout<<"Digite uma string: ";
    getline(std::cin,str);
    
    switch(strToInt(str.c_str()))
     {
      case strToInt("Say Hello"):
       std::cout <<"\n\tHello!\n\n";
      break;
      
      case strToInt("Say Goodbye"):
       std::cout << "\n\tGoodbye!\n\n";
      return 0; 
      break;
      
      default:
       std::cout << "\n\tInvalid!\n\n";
     }
   }while(1);
   
  return 0;
  
} 
