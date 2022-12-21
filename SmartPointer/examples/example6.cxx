#include <iostream>

struct SmartPointer
{  
  SmartPointer(std::string *s, int *age):p_Str(s), p_Int(age){}
    
  ~SmartPointer() 
   {
    delete p_Str;
     
    delete p_Int;
   }
    
  std::string *getString()
  {
   return p_Str;
  }
   
  int *getInt()
  {
   return p_Int;  
  }
  
 private:
  std::string *p_Str;
  int *p_Int;
};


int main()
{
  std::string str1, str2, strnome;
  int idade;
  
  std::cout<<"Enter First e Last Name: ";
  std::cin>>str1>>str2;
  
  std::cout<<"Enter Age: ";
  std::cin>>idade;
  
  strnome+=str1+" "+str2;
  
  SmartPointer sp(new std::string(strnome), new int(idade));
    
  std::cout << "\n\tUser: "<<*sp.getString()<<"\n\tUser Age: "<<*sp.getInt()<< " years\n\n";
  
  return 0;
}