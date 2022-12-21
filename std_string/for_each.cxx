#include <vector>
#include <algorithm>
#include <iostream>
 
struct Sum
{
    Sum(): sum{0} { }
    void operator()(int n) { sum += n; }
    int sum;
};
 
int main()
{
    std::vector<int> nums{3, 4, 2, 8, 15, 267};
 
    auto print = [](const int& n) { std::cout << " " << n; };
 
    std::cout << "before:";
    std::for_each(nums.begin(), nums.end(), print);
    std::cout << '\n';
 
    std::for_each(nums.begin(), nums.end(), [](int &n){ n++; });
 
    // calls Sum::operator() for each number
    Sum s = std::for_each(nums.begin(), nums.end(), Sum());
 
    std::cout << "after: ";
    std::for_each(nums.begin(), nums.end(), print);
    std::cout << '\n';
    std::cout << "sum: " << s.sum << '\n';
  return 0;  
}
/*
Output:

before: 3 4 2 8 15 267
after:  4 5 3 9 16 268
sum: 305
*/