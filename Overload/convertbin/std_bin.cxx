#include <bitset> //bitset
#include <iostream>

namespace std
{
 struct BinStream
 {
  template<typename T>
  typename std::enable_if<std::is_integral<T>::value, std::string>::type
  //aceita apenas numeros inteiros
  binary(const T& n)
  {
   return std::bitset<sizeof(T)*3>(n).to_string();
  }
  
  BinStream(std::ostream& os):os(os){}
  
  template<class T>
  BinStream& operator<<(T& value)
  {
   os << value;
   return *this;
  }
  
  BinStream& operator<<(int value)
  {
   os << binary(value);
   return *this;
  }
  
  std::ostream &operator<<(decltype(std::dec)& _Pfn)
  {
   return os << _Pfn;
  }
/*  
#if defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
  
  std::ostream& operator<<(std::ios_base& (__cdecl *_Pfn)(std::ios_base&))
  {
   return os << _Pfn;
  }
  
#endif
*/
  private:
   std::ostream& os;
 };
 
 struct Bin
 {
  friend BinStream operator<<(std::ostream& os, const Bin&);
 }bin;
 
 BinStream operator<<(std::ostream& os, const Bin&)
 {
  return BinStream(os);
 }
 
 std::ostream& line(std::ostream& outs)
 {
  outs << "\n\n";
  return outs;
 }
 
} //fim namespace


int main()
{
  std::cout << "\n\t127 em binario.....: " << std::bin << 127
            << "\n\t127 em octal.......: " << std::oct << 127
            << "\n\t127 em binario.....: " << std::bin << 127
            << "\n\t127 em hexadecimal.: " << std::hex << 127
            << "\n\t127 em binario.....: " << std::bin << 127
            << "\n\t127 em decimal.....: " << std::dec << 127
            << "\n\t127 em binario.....: " << std::bin << 127 
            << std::line;
  return 0;
}
/*
  Output:
        127 em binario.....: 0001111111
        127 em octal.......: 177
        127 em binario.....: 0001111111
        127 em hexadecimal.: 7f
        127 em binario.....: 0001111111
        127 em decimal.....: 127
        127 em binario.....: 0001111111
*/