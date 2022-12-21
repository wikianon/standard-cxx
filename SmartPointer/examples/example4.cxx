#include <iostream>

class IntSmartPointer
{
  int *pValue;
  
  public:
    IntSmartPointer(int *p):pValue(p){}
        
    ~IntSmartPointer()
     {
      delete pValue;
     }
        
     int *getValue()
     {
      return pValue;
     }       
};












int main()
{
    IntSmartPointer ptr(new int(5));

    std::cout << "IntSmartPointer Valor: " << *ptr.getValue() << std::endl;
  
    //Ponteiro normal:
    int* intstr = new int(6);
    std::cout << "IntNormalPointer Valor: "  << *intstr<< std::endl;
    delete intstr;
    
    return 0;
}