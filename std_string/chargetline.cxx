// istream::getline example
#include <iostream>     // std::cin, std::cout

int main()
{
 char name[256], title[256];
 
 std::cout << "Please, enter your name: ";
 std::cin.getline(name,sizeof(name));
 
 std::cout << "Please, enter your favourite movie: ";
 std::cin.getline(title,sizeof(title));
 
 std::cout << "\n\t" << name << "\'s favourite movie is " << title << "\n\n";
 
 return 0;
} 
