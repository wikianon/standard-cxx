#include <deque>
#include <vector>
#include <string>
     
#ifndef N
#define N 3000000
#endif
     
#if DEQUE
 typedef std::deque< std::string > Container;
#else
 typedef std::vector< std::string > Container;
#endif
     
int main()
{
 Container container;
  for(int i = 0; i < N; ++i)
  container.push_back(std::string(100 + (i % 1000), 'a'));
 return 0;
}