#include <iostream>

void getIntArray(int array[], const int len)
{
    for (int i = 0; i < len; ++i)
        array[i] = i + 1;
}

int main(void)
{
 int my_array[10] = { 0 }; // initialize to all 0s
 
 getIntArray(my_array, 10);
  
 for(int i = 0; i < 10; ++i)
  {
   std::cout << "\n\tArray index " << i << " = " << my_array[i] << "\n\n";
  }
 return 0;
}