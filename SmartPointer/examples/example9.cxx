#include <memory>
#include <vector>
#include <cstdio>
#include <cassert>
#include <fstream>
#include <iostream>
#include <functional>
 
struct Foo
{
  virtual void bar() { std::cout << "Foo::bar\n"; }
  virtual ~Foo() = default;
};

struct Bar : Foo
{
    Bar() { std::cout << "Bar::Bar\n";  }
    ~Bar() { std::cout << "Bar::~Bar\n";  }
    void bar() override { std::cout << "Bar::bar\n";  }
};
 
// a function consuming a unique_ptr can take it by value or by rvalue reference
std::unique_ptr<Bar> pass_through(std::unique_ptr<Bar> p)
{
    p->bar();
    return p;
}
 
int main()
{
  std::cout << "unique ownership semantics demo\n";
  {
      auto p = std::make_unique<Bar>(); // p is a unique_ptr that owns a Bar
      auto q = pass_through(std::move(p)); 
      assert(!p); // now p owns nothing and holds a null pointer
      q->bar();   // and q owns the Bar object
  } // ~Bar called here
 
  std::cout << "Runtime polymorphism demo\n";
  {
    std::unique_ptr<Foo> p = std::make_unique<Bar>(); // p is a unique_ptr that owns a Bar
                                                  // as a pointer to base
    p->bar(); // virtual dispatch
 
    std::vector<std::unique_ptr<Foo>> v;  // unique_ptr can be stored in a container
    v.push_back(std::make_unique<Bar>());
    v.push_back(std::move(p));
    v.emplace_back(new Bar);
    for(auto& p: v) p->bar(); // virtual dispatch
  } // ~Bar called 3 times
 
  std::cout << "Custom deleter demo\n";
  std::ofstream("demo.txt") << 'x'; // prepare the file to read
  {
      std::unique_ptr<std::FILE, decltype(&std::fclose)> fp(std::fopen("demo.txt", "r"),
                                                            &std::fclose);
      if(fp) // fopen could have failed; in which case fp holds a null pointer
        std::cout << (char)std::fgetc(fp.get()) << '\n';
  } // fclose() called here, but only if FILE* is not a null pointer
    // (that is, if fopen succeeded)
 
  std::cout << "Custom lambda-expression deleter demo\n";
  {
    std::unique_ptr<Bar, std::function<void(Bar*)>> p(new Bar, [](Bar* ptr)
        {
            std::cout << "destroying from a custom deleter...\n";
            delete ptr;
        });  // p owns Bar
    p->bar();
  } // the lambda above is called and Bar is destroyed
 
  std::cout << "Array form of unique_ptr demo\n";
  {
      std::unique_ptr<Bar[]> p{new Bar[3]};
  } // calls ~Bar 3 times
}
/*
Output:

unique ownership semantics demo
Bar::Bar
Bar::bar
Bar::bar
Bar::~Bar
Runtime polymorphism demo
Bar::Bar
Bar::bar
Bar::Bar
Bar::Bar
Bar::bar
Bar::bar
Bar::bar
Bar::~Bar
Bar::~Bar
Bar::~Bar
Custom deleter demo
x
Custom lambda-expression deleter demo
Bar::Bar
Bar::bar
destroying from a custom deleter...
Bar::~Bar
Array form of unique_ptr demo
Bar::Bar
Bar::Bar
Bar::Bar
Bar::~Bar
Bar::~Bar
Bar::~Bar
*/