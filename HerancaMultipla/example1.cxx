#include <iostream>

class Test4
{
 public:
 Test4(){}
 
 ~Test4(){ std::cout<<"\n\tI am class Test4.\n"; }
};

class Test3
{
 public:
 Test3(){}
 
 ~Test3(){ std::cout<<"\n\tI am class Test3.\n"; }
};

class Test2
{
 public:
 Test2(){}
 
 ~Test2(){ std::cout<<"\n\tI am class Test2.\n"; }
};

class Test1: public Test4, public Test3, public Test2
{
 public:    
 Test1(){}
 ~Test1(){ std::cout<<"\n\tI am class Test1.\n";}
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