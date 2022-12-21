#include <limits> //numeric_limits
#include <iostream>

int getInt();
void getInput();
char32_t getChar();

int main()
{
 getInput();
 return 0;
}

int getInt()
{
 int input = 0;
 
 //while(!(std::cin>>input))
 while((std::cin>>input).fail())
  {
   // clear the error flag that was set so that future I/O operations will work correctly
   std::cin.clear();
   
   // skips to the next newline
   std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
   std::cout << "\n\tInvalid input.\n\n\tPlease enter a positive number: ";
  }
 return input;
}

char32_t getChar()
{
 // absorb newline character (if existant) from previous input
 if('\n' == std::cin.peek()) std::cin.ignore();
 return std::tolower(std::cin.get());
}

void getInput()
{
 int num;
 do{
    std::cout<<"\n\tDigite um numero: ";
    num = getInt();
    std::cout << "\n\tIs Number "<<num<<"\n\n\tRun the program again (y/N): ";  // signify n as default with capital letter
   }while('y' == getChar());
}