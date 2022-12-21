#include <memory>
#include <iostream>

































int main() 
{
 auto str = std::make_unique<std::string>("Testando Smart Pointers");
 
 auto num1 = std::make_unique<int>(10);
 auto num2 = std::make_unique<double>(1.11);
 
 std::cout <<"\n\t"<< *str.get() << " size: " << (*str).size() << "\n\tNum int: "<<*num1.get()<<"\n\tNum Double: "<<*num2.get()<<"\n\n"; 
}