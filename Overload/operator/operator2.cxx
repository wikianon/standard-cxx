#include <iostream>






// sized class-specific deallocation functions
struct Operator
{
    static void operator delete(void* ptr, std::size_t sz)
    {
     std::cout << "custom delete for size " << sz << '\n';
     ::operator delete(ptr);
    }
    
    static void operator delete[](void* ptr, std::size_t sz)
    {
     std::cout << "custom delete for size " << sz << '\n';
     ::operator delete(ptr);
    }
};









int main()
{
     Operator* p1 = new Operator;
     delete p1;
     Operator* p2 = new Operator[10];
     delete[] p2;
}
/*
Possible output:

custom delete for size 1
custom delete for size 18 
*/