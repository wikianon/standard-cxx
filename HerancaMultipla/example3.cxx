#include <iostream>

class Test1
{
 protected:
  std::string _msg1;
 
 public:
  Test1( std::string msg1 ): _msg1( msg1 ){}
};

class Test2
{
 protected:
  std::string _msg2;
  
 public:
  Test2( std::string msg2 ): _msg2( msg2 ){}
};

class Test3
{
 protected:
  std::string _msg3;
 
 public:
  Test3( std::string msg3 ): _msg3( msg3 ){}
};

class Test4: public Test1, public Test2, public Test3
{
 std::string _msg4;
  public:
     
 Test4( std::string msg1, std::string msg2 , std::string msg3, std::string msg4 ):
 Test1( msg1 ), Test2( msg2 ), Test3( msg3 ), _msg4( msg4 ){}
 
 void show();
};

void Test4::show()
{
 std::cout <<"\n\t"<< _msg1 <<" "<< _msg2 <<" "<< _msg3 <<" "<< _msg4 << "\n";
}
     
int main()
{
 Test4 teste("Teste1", "Teste2","Teste3","Teste4\n");
 teste.show();
 return 0;
}