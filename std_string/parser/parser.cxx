#include <regex>
#include <string>
#include <vector>
#include <cassert>
#include <iostream>
#include <stdexcept>
//parser regex https://cecko.eu/public/pb173/cviceni_13
namespace dynamic
{     
enum class Type { Integer, Double, String, IntArray };
     
struct Dynval
{
 Dynval(): Dynval(Type::Integer){}
 
 explicit Dynval(Type t): _type(t)
  {
   switch(t)
    {
     case Type::Integer:
      _data.integer = 0;
     break;
     
     case Type::Double:
      _data.dbl = 0;
     break;
     
     case Type::String:
      _make(_data.str);
     break;
     
     case Type::IntArray:
      _make(_data.arr);
     break;
    }
   }
 
 Dynval(const Dynval &o): _type(o._type)
  {
   switch(_type)
    {
     case Type::Integer:
      _copy(_data.integer, o._data.integer);
     break;
     
     case Type::Double:
      _copy(_data.dbl, o._data.dbl);
     break;
     
     case Type::String:
      _copy(_data.str, o._data.str); break;
     case Type::IntArray:
      _copy(_data.arr, o._data.arr);
     break;
    }
  }
  
 Dynval(Dynval &&o): _type(o._type)
  {
   switch(_type)
    {
     case Type::Integer:
      _move(_data.integer, o._data.integer);
     break;
     
     case Type::Double:
      _move(_data.dbl, o._data.dbl);
     break;
     
     case Type::String:
      _move(_data.str, o._data.str);
     break;
     
     case Type::IntArray:
      _move(_data.arr, o._data.arr);
     break;
    }
  }
     
 Dynval(long l): _type(Type::Integer)
  {
   _move(_data.integer, l);
  }
  
 Dynval(double d): _type(Type::Double)
  {
   _move(_data.dbl, d);
  }
 
 Dynval(std::string str): _type(Type::String)
  {
   _move(_data.str, str);
  }
 
 Dynval(std::vector<long> arr): _type(Type::IntArray)
  {
   _move(_data.arr, arr);
  }
 
 ~Dynval()
  {
   switch(_type)
    {
     case Type::String:
      _del(_data.str);
     break;
     
     case Type::IntArray:
      _del(_data.arr);
     break;
     
     default: ; // nothing otherwise
    }
  }
  
 Type type()const
  {
   return _type;
  }
 
 long getInt()const
  {
   assert(_type == Type::Integer);
   return _data.integer;
  }
     
 double getDouble()const
  {
   assert(_type == Type::Double);
   return _data.dbl;
  }
  
 const std::string &getString()const
  {
   assert(_type == Type::String);
   return _data.str;
  }
  
 const std::vector<long> &getIntArray()const
  {
   assert(_type == Type::IntArray);
   return _data.arr;
  }
  
 friend std::ostream &operator<<(std::ostream &os, const Dynval &val)
  {
   switch(val._type)
    {
     case Type::Integer:
      return os << val._data.integer;
     break;
     
     case Type::Double:
      return os << val._data.dbl;
     break;
     
     case Type::String:
      return os << val._data.str;
     break;
     
     case Type::IntArray:
      const auto end = val._data.arr.end();
       os << "[";
        for(auto it = val._data.arr.begin(); it != end; ++it)
         {
          os << *it;
          if(it+1 != end)
          os << ", ";
         }
        os << "]";
     return os;
    }
  }
  
 static Dynval parse(std::string str)
  {
   if(std::regex_match(str, _regInt))
    return Dynval(std::stol(str));
   
   if(std::regex_match(str, _regDbl))
    return Dynval(std::stod(str));
   
   if(std::regex_match(str, _regStr))
    return Dynval(str.substr(1, str.length()-2));
   
   if(std::regex_match(str, _regArr))
    {
     std::sregex_iterator it(str.begin(), str.end(), _regInt);
     const std::sregex_iterator end;
     std::vector< long > values;
     
     for(; it != end; ++it)
      values.emplace_back( std::stol( it->str() ) );
       return Dynval( values );
    }
   throw std::invalid_argument( "Dynval::parse: invalid expression '" + str + "'" );
  }
     
 private:
  
  template<typename T> static void _del(T &t)
   {
    t.~T();
   }
  
  template<typename T> static void _make(T &t)
   {
    new(&t) T();
   }
  
  template<typename T>
  static void _move(T &where, T &what)
   {
    new (&where) T(std::move(what));
   }
  
  template<typename T>
  static void _copy(T &where, const T &what)
   {
    new (&where) T(what);
   }
  
  union Data
   {
    long integer;
    double dbl;
    std::string str;
    std::vector<long> arr;
    
    Data(){} // see Dynval
    ~Data(){} // see ~Dynval
   };
     
  Type _type;
  Data _data;
     
        static const std::regex _regInt;
        static const std::regex _regDbl;
        static const std::regex _regStr;
        static const std::regex _regArr;
};
     
 constexpr std::regex_constants::syntax_option_type regopts
  = std::regex_constants::egrep
  | std::regex_constants::nosubs
  | std::regex_constants::optimize;
     
 const std::regex Dynval::_regInt = std::regex("[0-9]+", regopts);
 const std::regex Dynval::_regDbl = std::regex("[0-9]+\\.[0-9]+", regopts);
 const std::regex Dynval::_regStr = std::regex("\"[^\"]*\"", regopts);
 
 const std::regex Dynval::_regArr = std::regex("[[][ ]*([0-9]+,[ ]*)*([0-9]+)?[ ]*[]]", regopts);
 
}
     
#define ASSERT_EQ( X, Y ) \
do{ \
   if( X != Y ) \
    { \
     std::cerr << #X "==" #Y " failed: " << "[" << X << "] != [" << Y << "]" \
               << ", at " << __FILE__ << ", line " << __LINE__ << std::endl; \
     std::abort(); \
    } \
  }while(false)
     
int main()
{
 using namespace dynamic;
 
 // some simple tests
 {
  ASSERT_EQ(Dynval::parse("0").getInt(), 0);
  
  ASSERT_EQ(Dynval::parse("42").getInt(), 42);
  
  ASSERT_EQ(Dynval::parse("1000000000000").getInt(), 1000000000000);
  
  ASSERT_EQ(Dynval::parse("3.14").getDouble(), 3.14);
  
  ASSERT_EQ(Dynval::parse("\"ahoj\"").getString(), "ahoj");
  
  auto arr = Dynval::parse("[]").getIntArray();
  
  ASSERT_EQ(arr.size(), 0);
  
  arr = Dynval::parse("[1]").getIntArray();
  
  ASSERT_EQ(arr.size(), 1);
  
  ASSERT_EQ(arr[0], 1);
     
  arr = Dynval::parse("[1,2,3]").getIntArray();
  
  ASSERT_EQ(arr.size(), 3);
  
  ASSERT_EQ(arr[0], 1);
  
  ASSERT_EQ(arr[1], 2);
  
  ASSERT_EQ(arr[2], 3);
  
  arr = Dynval::parse("[   1,   2,3       ]").getIntArray();
  
  ASSERT_EQ(arr.size(), 3);
  ASSERT_EQ(arr[0], 1);
  ASSERT_EQ(arr[1], 2);
  ASSERT_EQ(arr[2], 3);
 }
 
 std::cout << "You can try parser now (you can terminate it by CTRL+D)\n";
 std::string line;
 
 while(std::getline(std::cin, line))
  {
   try{
       auto dval = Dynval::parse( line );
       std::cout << dval << std::endl;
      }
      catch(std::exception &ex)
      {
       std::cerr << "Exception: " << ex.what() << std::endl;
      }
  }
}