#include <iostream>

class date 
{
   unsigned int _dia;
   unsigned int _mes;
   unsigned int _ano;
   std::string _sep;
   std::string diatmp = "";
   std::string mestmp = "";
   std::string anotmp = "";
   
   time_t _tm = time(NULL);
   tm* curtime = localtime(&_tm);
   
   public:
   
   ~date(){}
   
   date()
   {
    _dia = curtime->tm_mday;
    _mes = curtime->tm_mon+1;
    _ano = curtime->tm_year+1900;    
   }
   
   date( unsigned int d, unsigned int m, unsigned int a, std::string sep=(".") ):
   _dia(d), _mes(m), _ano(a), _sep(sep)
   { 
     _dia = curtime->tm_mday;
     _mes = curtime->tm_mon+1;
     _ano = curtime->tm_year+1900;
   }
   
   std::string day()
   {
    diatmp = std::to_string(_dia);
    if(diatmp.length() == 1)
    diatmp.insert(0, "0");
    return diatmp;
   }
   
   std::string month()
   {
    mestmp = std::to_string(_mes);
    if(mestmp.length() == 1)
    mestmp.insert(0, "0"); 
    return mestmp;
   }
   
   std::string year()
   {
    anotmp = std::to_string(_ano);
    return anotmp;
   }
   
   std::string datetime()
   {
    return asctime(curtime);
   }
   
   friend std::ostream& operator<<( std::ostream& os , const date& dt ) 
   {
    std::string diatmp = "";
    diatmp = std::to_string(dt._dia);
    if(diatmp.length() == 1)
    diatmp.insert(0, "0");

    std::string mestmp = "";
    mestmp = std::to_string(dt._mes);
    if(mestmp.length() == 1)
    mestmp.insert(0, "0");
     
    std::string anotmp = "";
    anotmp = std::to_string(dt._ano);
    
    os << diatmp.c_str() << dt._sep << mestmp.c_str() << dt._sep << anotmp.c_str() << "\n";
           
    return os;
   }
};

int main(void) 
{ 
  unsigned int dia;
  unsigned int mes;
  unsigned int ano;
  
  date sepdate(dia, mes, ano , "-");
  std::cout << "\n\tThe date sep "
            << sepdate 
            << std::flush;
   
  date data(dia, mes, ano);
  std::cout << "\n\tThe date default "
            << data 
            << std::flush;
  
  date dt;
  std::cout << "\n\tDay......: " << dt.day()
            << "\n\tMonth....: " << dt.month()     
            << "\n\tYear.....: " << dt.year()
            << "\n\tDateTime.: " << dt.datetime()
            << "\n\n";
  return 0;
}