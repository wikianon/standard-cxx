#include <cstring>
#include <iostream>

class Person
{
 int *pAge;
 std::string* sName; 

  public:
   Person(): sName(0),pAge(0){}
   
   Person(std::string* sName, int *age): sName(sName), pAge(age){}
   
   ~Person(){}

   void display()
   {
    std::cout<<"\n\tName: "<<*sName<<"  Age: "<<*pAge<<"\n\n";
   }
};

template < typename T >
class GenericSmartPointer
{
 T* pData; // Generic pointer to be stored
         
 public:
    
  GenericSmartPointer(T* pValue) : pData(pValue){}

  ~GenericSmartPointer()
   {
    delete pData;
   }

   T& operator* ()
   {
    return *pData;
   }

   T* operator-> ()
   {
    return pData;
   }
};

int main()
{
  int idade;
  std::string str;

  std::cout<<"Digite um nome e idade: ";
  std::cin>>str>>idade;
  
  GenericSmartPointer<Person> gsp(new Person(new std::string(str), new int(idade)));
  gsp->display(); // Não precisa apagar o ponteiro da pessoa.
}