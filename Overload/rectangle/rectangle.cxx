#include <iostream>


struct Rectangle
{
 //Function overloading
 void printArea(int x, int y)
 {
  std::cout << x * y << std::endl;
 }
 
 void printArea(int x)
 {
  std::cout << x * x << std::endl;
 }
 
 void printArea(int x, double y)
 {
  std::cout << x * y << std::endl;
 }
 
 void printArea(double x)
 {
 std::cout << x * x << std::endl;
 }
 
};

int main()
{
 Rectangle rt;
 
 rt.printArea(2,4);
 
 rt.printArea(2,5.1);
 
 rt.printArea(10);
 
 rt.printArea(2.3);
 
 return 0;
} 
