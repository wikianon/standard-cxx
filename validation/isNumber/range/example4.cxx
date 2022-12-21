 // Function template example.
#include <iostream>

template <class X> void swapargs(X &a, X &b)// This is a function template.
{
 X temp;
 temp = a;
 a = b;
 b = temp;
}

int main()
{
 int i=10;
 int j=20;
 char a='x';
 char b='z';
 double x=10.1;
 double y=23.3;
 
 std::cout << "Original i, j: " << i << ' ' << j << '\n'
           << "Original x, y: " << x << ' ' << y << '\n'
           << "Original a, b: " << a << ' ' << b << '\n';
 
 swapargs(i, j); // swap integers
 swapargs(x, y); // swap floats
 swapargs(a, b); // swap chars
 
 std::cout << "Swapped i, j: " << i << ' ' << j << '\n'
           << "Swapped x, y: " << x << ' ' << y << '\n'
           << "Swapped a, b: " << a << ' ' << b << '\n';
 return 0;
} 
/*
 Output:
 Original i, j: 10 20
 Original x, y: 10.1 23.3
 Original a, b: x z
 Swapped i, j: 20 10
 Swapped x, y: 23.3 10.1
 Swapped a, b: z x
*/