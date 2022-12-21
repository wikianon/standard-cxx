#include <iostream>

struct Animals
{
 //Function Overriding Not virtual 
 void sound()
 {
  std::cout << "This is parent class\n";
 }
};

struct Dogs : Animals
{
 void sound()
 {
  std::cout << "Dogs bark\n";
 }
};

struct Cats : Animals
{
 void sound()
 {
  std::cout << "Cats meow\n";
 }
};

struct Pigs : Animals
{
 void sound()
 {
  std::cout << "Pigs snort\n";
 }
};

int main()
{
 Dogs d;
 d.sound();
 
 Cats c;
 c.sound();
 
 Pigs p;
 p.sound();
 
 return 0;
} 
/*
 Output: 
 Dogs bark
 Cats meow
 Pigs snort
 */