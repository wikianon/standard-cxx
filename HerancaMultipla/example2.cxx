#include <iostream>

class Test4
{
 public:
 Test4(){  std::cout<<"\n\tI am class Test4.\n"; }
 
 ~Test4(){ std::cout<<"\n\tDestroied class Test4.\n"; }
};

class Test3
{
 public:
 Test3(){ std::cout<<"\n\tI am class Test3.\n"; }
 
 ~Test3(){ std::cout<<"\n\tDestroied class Test3.\n"; }
};

class Test2
{
 public:
 Test2(){ std::cout<<"\n\tI am class Test2.\n";}
 
 ~Test2(){ std::cout<<"\n\tDestroied class Test2.\n"; }
};

class Test1: public Test4, public Test3, public Test2
{
 public:
 Test1(){  std::cout<<"\n\tI am class Test1.\n"; }
 ~Test1(){ std::cout<<"\n\tDestroied class Test1.\n"; }
 void show();
};

void Test1::show()
{
 Test1();
}

int main()
{
 Test1 teste;
 teste.show();
 return 0;
}