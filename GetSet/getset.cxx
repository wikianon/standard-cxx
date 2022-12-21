#include <iostream>
//https://murilo.wordpress.com/2009/05/21/como-implementar-facilmente-getters-e-setters-em-c/
class GetSet
{
 protected:

 #define declare(type, name) \
 type _##name; \
 public: \
 void set##name(const type& var) \
 {\
   _##name = var;\
 }\
 \
 type get##name() \
 {\
  return _##name; \
 }
};

struct Teste: public GetSet
{
 declare(int, w); //declarando um atributo w do tipo int e seu método get e set
 declare(int, x); //declarando um atributo x do tipo int e seu método get e set
 declare(int, y); //declarando um atributo y do tipo int e seu método get e set
 declare(std::string, z); //declarando um atributo z do tipo string e seu método get e set
};

int main()
{
  Teste t;
  t.setw(29);
  t.setx(01);
  t.sety(2018);
  t.setz("GetSet Teste class");
  
  std::cout << "\n\tDate: "
            << t.getw()
            << "/"
            << t.getx()
            << "/"
            << t.gety()
            << "\n\t"
            << t.getz()
            << "\n\n";
  return 0;
}