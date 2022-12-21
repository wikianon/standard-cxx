#include <vector>
#include <iostream>
//Alterar um atributo de uma classe base pela classe derivada C++

class Base
{
 std::vector<std::string> Names;
  public:
   void Add(std::string);
   std::string Get(int);
};

void Base::Add(std::string name)
{
 this->Names.push_back(name);
}

std::string Base::Get(int name_position)
{
 return this->Names[name_position];
}

// classe derivada
class Derived : public Base
{
 public:
  void New(std::string);
};

void Derived::New(std::string name)
{
 Base::Add(name);
}

int main()
{
 Derived derived;
 
 derived.New("Bolsonaro");
 
 std::cout<<"\n\t"<<derived.Get(0)<<"\n\n";

 return 0;
}