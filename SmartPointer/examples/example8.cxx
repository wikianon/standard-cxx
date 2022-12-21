#include <memory>
#include <iostream>

struct Foo
{ // object to manage
 Foo() { std::cout << "Foo ctor\n"; }
    
 Foo(const Foo&) { std::cout << "Foo copy ctor\n"; }
    
 Foo(Foo&&) { std::cout << "Foo move ctor\n"; }
    
 ~Foo() { std::cout << "~Foo dtor\n"; }
};
 
struct Bar
{ // deleter
 Bar() {};
    
 Bar(const Bar&) { std::cout << "Bar copy ctor\n"; }
    
 Bar(Bar&) { std::cout << "Bar non-const copy ctor\n";}
    
 Bar(Bar&&) { std::cout << "Bar move ctor \n"; }
    
 void operator()(Foo* p) const
  {
   std::cout << "Bar is deleting a Foo\n";
   delete p;
  };
};
 
int main()
{
    std::cout << "Example constructor(1)...\n";
    std::unique_ptr<Foo> up1;  // up1 is empty
    std::unique_ptr<Foo> up1b(nullptr);  // up1b is empty
 
    std::cout << "Example constructor(2)...\n";
    {
        std::unique_ptr<Foo> up2(new Foo); //up2 now owns a Foo
    } // Foo deleted
 
    std::cout << "Example constructor(3)...\n";
    Bar d;
    {  // deleter type is not a reference
       std::unique_ptr<Foo, Bar> up3(new Foo, d); // deleter copied
    }
    {  // deleter type is a reference 
       std::unique_ptr<Foo, Bar&> up3b(new Foo, d); // up3b holds a reference to d
    }
 
    std::cout << "Example constructor(4)...\n";
    {  // deleter is not a reference 
       std::unique_ptr<Foo, Bar> up4(new Foo, Bar()); // deleter moved
    }
 
    std::cout << "Example constructor(5)...\n";
    {
       std::unique_ptr<Foo> up5a(new Foo);
       std::unique_ptr<Foo> up5b(std::move(up5a)); // ownership transfer
    }
 
    std::cout << "Example constructor(6)...\n";
    {
        std::unique_ptr<Foo, Bar> up6a(new Foo, d); // Bar is copied
        std::unique_ptr<Foo, Bar> up6b(std::move(up6a)); // Bar is moved
 
        std::unique_ptr<Foo, Bar&> up6c(new Foo, d); // Bar is a reference
        std::unique_ptr<Foo, Bar> up6d(std::move(up6c)); // Bar is copied
    }
 
    std::cout << "Example constructor(7)...\n";
    {
        std::unique_ptr<Foo> up7a(new Foo);
        std::unique_ptr<Foo> up7b(std::move(up7a)); // ownership transfer
    }
}
/*
Output:

Example constructor(1)...
Example constructor(2)...
Foo ctor
~Foo dtor
Example constructor(3)...
Foo ctor
Bar copy ctor
Bar is deleting a Foo
~Foo dtor
Foo ctor
Bar is deleting a Foo
~Foo dtor
Example constructor(4)...
Foo ctor
Bar move ctor 
Bar is deleting a Foo
~Foo dtor
Example constructor(5)...
Foo ctor
~Foo dtor
Example constructor(6)...
Foo ctor
Bar copy ctor
Bar move ctor 
Foo ctor
Bar non-const copy ctor
Bar is deleting a Foo
~Foo dtor
Bar is deleting a Foo
~Foo dtor
Example constructor(7)...
Foo ctor
~Foo dtor
*/