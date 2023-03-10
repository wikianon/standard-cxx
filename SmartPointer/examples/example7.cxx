#include <memory>
#include <iostream>

struct Foo
{
    Foo() { std::cout << "Foo::Foo\n"; }
    
    ~Foo() { std::cout << "Foo::~Foo\n"; }
    
    void bar() { std::cout << "Foo::bar\n"; }
};
 
void f(const Foo &foo)
{
    std::cout << "f(const Foo&)\n";
}

int main()
{
    std::unique_ptr<Foo> p1(new Foo);  // p1 owns Foo
    if (p1) p1->bar();
 
    {
        std::unique_ptr<Foo> p2(std::move(p1));  // now p2 owns Foo
        f(*p2);
 
        p1 = std::move(p2);  // ownership returns to p1
        std::cout << "destroying p2...\n";
    }
 
    if (p1) p1->bar();
 
    // Foo instance is destroyed when p1 goes out of scope
}

/*
  Output:
  
  Foo::Foo
  Foo::bar
  f(const Foo&)
  destroying p2...
  Foo::bar
  Foo::~Foo
 */