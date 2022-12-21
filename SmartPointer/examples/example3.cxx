#include <iostream>

class StringSmartPointer
{
 std::string *pStr;
    
   public:
   
    StringSmartPointer(std::string *s):pStr(s){}
    
   ~StringSmartPointer() 
    {
      delete pStr;
    }
    
   std::string *getString()
   {
    return pStr;
   }
};










int main()
{
    StringSmartPointer str(new std::string("Testando Smart Pointers"));
    
    std::cout << "\n\tStringSmartPointer: "<<*str.getString();
    
    //Ponteiro normal:    
    std::string *nmptr = new std::string("Testando Default Pointers");
    
    std::cout << "\n\n\tStringDefaultPointer: "<<*nmptr<< "\n\n";
    delete nmptr;
    
    return 0;
}