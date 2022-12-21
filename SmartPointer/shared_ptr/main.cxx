#include "shared_ptr.hxx"

struct A 
{
 int i;
 
 explicit A(int _i) : i(_i) {}

 ~A() 
  {
   std::cout << "deleting A\n\n";
  }
};

CuShPtr<A> make_A() 
{
 CuShPtr<A> a(new A(5));
 return a;
}

int main() 
{
 CuShPtr<A> a = make_A();
 CuShPtr<A> b = a;
 CuShPtr<A> c = a;

 std::cout << a.use_count() << " " << b.use_count() << " " << c.use_count() << "\n";
 c.reset();
 std::cout << a.use_count() 
           << " " << b.use_count() << " " << c.use_count()
           <<"\n"<< a->i << "\n\n";
}