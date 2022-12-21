#include <iostream>


struct Animals
{
   //Function Overriding Not virtual
   void sound()
   {
    std::cout << "\n\tThis is parent class\n\n";
   }
};

struct Dogs : Animals
{
 void sound()
 {
  std::cout << "\n\tDogs bark\n\n";
 }
};

int main()
{
 Dogs d;
 d.sound();
 return 0;
} 
/*
 Output: Dogs bark
*/