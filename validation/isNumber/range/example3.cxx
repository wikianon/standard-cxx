#include <vector>
#include <iostream>

template <int _begin, int _end>
struct range
{
 struct iterator
  {
   int value;
   
   iterator(int v): value(v){}
   operator int() const
    {
     return value;
    }
    
   operator int&()
    {
     return value;
    }
    
   int operator *() const
    {
     return value;
    }
  };
  
 iterator begin()
  {
   return _begin;
  }
  
 iterator end()
  {
   return _end;
  }
};

int main()
{
 for(auto i : range<0, 5>())
  std::cout << i << ' ';
 std::cout << "\n"; 
 return 0;
}