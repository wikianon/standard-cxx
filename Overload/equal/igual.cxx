#include <iostream>




class Stock
{
 int resource;
 
 public:
  int get_resource() { return resource; }
  
  void set_resource(int r) { resource = r; }
  
  bool operator==(const Stock& stk) { return resource == stk.resource; }
};













int main ()
{
 Stock stk1, stk2;

 stk1.set_resource(10);
 stk2.set_resource(12);

 std::cout << "\n\tNumbers: "
           << stk1.get_resource()
           << " and "
           << stk2.get_resource()
           <<(stk1 == stk2 ? " they are the same.\n\n":" are different.\n\n");

   return 0;
}