#include <iostream>

#define irange(i,a,b) int i = (a); i < (b); ++i // if you want to use ints all the time
#define range(i,a,b) i = (a); i < (b); ++i      // if you ever want to use something other than an int
#define zrange(i,b) i = 0; i < (b); ++i       // if you want to start at zero
#define izrange(i,b) int i = 0; i < (b); ++i  // if you want to start at zero and use ints








int main()
{
 for(int num = 0; num < 10; ++num)std::cout << num << ' ';
 
 std::cout << '\n';
 
 for(irange(num, 0, 10))std::cout << num << ' ';
 
 std::cout << '\n';
 
 for(int range(num, 0, 10))std::cout << num << ' ';

 std::cout << '\n';
 
 for(int zrange(num, 10))std::cout << num << ' ';
 
 std::cout << '\n';

 for(izrange(num, 10))std::cout << num << ' ';
 
 std::cout << '\n';
}
/*
 Output:
 0 1 2 3 4 5 6 7 8 9 
 0 1 2 3 4 5 6 7 8 9 
 0 1 2 3 4 5 6 7 8 9 
 0 1 2 3 4 5 6 7 8 9 
 0 1 2 3 4 5 6 7 8 9
*/