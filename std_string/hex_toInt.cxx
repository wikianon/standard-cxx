#include <iostream>

int hexToInt(std::string s)
{
 int res = 0;

 for(size_t i = 0; i < s.size(); i++)
  {
   int multiplier = 1;
   int exp = (s.size() - 1 - i);
   
   while (exp-- > 0)
    multiplier *= 16;
   
   int ch = s[i];
   
   if(ch >= '0' && ch <= '9')
    res += multiplier * (ch - '0');
   else
   if(ch >= 'a' && ch <= 'z')
    res += multiplier * (ch - 'a'+10);
   else
   if(ch >= 'A' && ch <= 'Z')
    res += multiplier * (ch - 'A'+10);
  }
 return res;
}


int main ()
{
 std::string convert[33]={
                          "0","1","2","3","4","5","6","7","8","9",
                          "A","B","C","D","E","F","10","11","12",
                          "13","14","15","16","17","18","19","1A",
                          "1B","1C","1D","1E","1F","20"
                         }; //33
 for(int i=0; i<33;i++)
 std::cout<<"\n\n\thexToInt{"<<convert[i]<<"}: "<<hexToInt(convert[i]);
  
  /*
  std::cout<<"\n\n\thexToInt{0}: "<<hexToInt("0");
  std::cout<<"\n\n\thexToInt{1}: "<<hexToInt("1");
  std::cout<<"\n\n\thexToInt{2}: "<<hexToInt("2");
  std::cout<<"\n\n\thexToInt{3}: "<<hexToInt("3");
  std::cout<<"\n\n\thexToInt{4}: "<<hexToInt("4");
  std::cout<<"\n\n\thexToInt{5}: "<<hexToInt("5");
  std::cout<<"\n\n\thexToInt{6}: "<<hexToInt("6");
  std::cout<<"\n\n\thexToInt{7}: "<<hexToInt("7");
  std::cout<<"\n\n\thexToInt{8}: "<<hexToInt("8");
  std::cout<<"\n\n\thexToInt{9}: "<<hexToInt("9");
  std::cout<<"\n\n\thexToInt{A}: "<<hexToInt("A");
  std::cout<<"\n\n\thexToInt{B}: "<<hexToInt("B");
  std::cout<<"\n\n\thexToInt{C}: "<<hexToInt("C");
  std::cout<<"\n\n\thexToInt{D}: "<<hexToInt("D");
  std::cout<<"\n\n\thexToInt{E}: "<<hexToInt("E");
  std::cout<<"\n\n\thexToInt{F}: "<<hexToInt("F");
  std::cout<<"\n\n\thexToInt{10}: "<<hexToInt("10");
  std::cout<<"\n\n\thexToInt{11}: "<<hexToInt("11");
  std::cout<<"\n\n\thexToInt{12}: "<<hexToInt("12");
  std::cout<<"\n\n\thexToInt{13}: "<<hexToInt("13");
  std::cout<<"\n\n\thexToInt{14}: "<<hexToInt("14");
  std::cout<<"\n\n\thexToInt{15}: "<<hexToInt("15");
  std::cout<<"\n\n\thexToInt{16}: "<<hexToInt("16");
  std::cout<<"\n\n\thexToInt{17}: "<<hexToInt("17");
  std::cout<<"\n\n\thexToInt{18}: "<<hexToInt("18");
  std::cout<<"\n\n\thexToInt{19}: "<<hexToInt("19");
  std::cout<<"\n\n\thexToInt{1A}: "<<hexToInt("1A");
  std::cout<<"\n\n\thexToInt{1B}: "<<hexToInt("1B");
  std::cout<<"\n\n\thexToInt{1C}: "<<hexToInt("1C");
  std::cout<<"\n\n\thexToInt{1D}: "<<hexToInt("1D");
  std::cout<<"\n\n\thexToInt{1E}: "<<hexToInt("1E");
  std::cout<<"\n\n\thexToInt{1F}: "<<hexToInt("1F");
  std::cout<<"\n\n\thexToInt{20}: "<<hexToInt("20");
  */
 std::cout<<"\n\n";
 return 0;
}