#include <iostream>

uint64_t constexpr mix(char m, uint64_t s)
 {
  return ((s<<7) + ~(s>>3)) + ~m;
 }
 
uint64_t constexpr hash(const char * m)
 {
  return (*m) ? mix(*m,hash(m+1)) : 0;
 }
 
int main()
 {
  
  std::string str;
  while(1)
  {
   std::cout<<"\n\tDigite uma string: ";
   std::cin>>str;
  
   switch(hash(str.c_str())) // run-time
   {
    case hash("teste1"): // compile-time
    std::cout << "\n\tHello Teste 1 ..!\n";
    break;
    
    case hash("teste2"): // compile-time
    std::cout << "\n\tHello Teste 2 ..!\n";
    break;
     
    case hash("sair"):
    std::cout<<"\n\tGood Bye ..!!\n\n";
    return 0;
    break;
    
    default:
    std::cout << "\n\tWhat?\n";
    break;
   }
  }
  
  return 0;
 }