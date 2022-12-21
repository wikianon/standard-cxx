#include <iostream>

struct Date
{
 unsigned int dia;
 unsigned int mes;
 unsigned int ano;
 std::string tmp = "";
 
 Date(unsigned int, unsigned int, unsigned int);
 
 Date();
 ~Date();
 
 Date &setDia(unsigned int, std::string);
 Date &setMes(unsigned int, std::string);
 Date &setAno(unsigned int, std::string);
 
 Date &setDia(unsigned int);
 Date &setMes(unsigned int);
 Date &setAno(unsigned int);
 
 Date &setDia();
 Date &setMes();
 Date &setAno();
 
 std::string getDia();
 std::string getMes();
 std::string getAno();
 
 friend std::ostream& operator<<(std::ostream&, const Date&);
 
 private: 
  time_t mt = time(0);
  tm* tms = localtime(&mt);
};

std::ostream& operator<<(std::ostream& os, const Date& dt)
{
  std::string diatmp = "";
  diatmp = std::to_string(dt.dia);
  if (diatmp.length() == 1)
  diatmp.insert(0, "0");
  
  std::string mestmp = "";
  mestmp = std::to_string(dt.mes);
  if(mestmp.length() == 1)
  mestmp.insert(0, "0");
  
  std::string anotmp = "";
  anotmp = std::to_string(dt.ano);
  if(anotmp.length() == 1)
  anotmp.insert(0, "0");
  
  os << diatmp.c_str() << "/" << mestmp.c_str() << "/" << anotmp.c_str() << "\n";
  return os;
}

Date::Date(unsigned int d, unsigned int m, unsigned int a): dia(d), mes(m), ano(a){}

Date::Date()
{
 dia = tms->tm_mday;
 mes = tms->tm_mon+1;
 ano = tms->tm_year+1900;
}

Date::~Date()
{
 dia=0;
 mes=0;
 ano=0;
 tmp.clear();
}

Date &Date::setDia(unsigned int d, std::string sep)
{
  tmp = std::to_string(d);
  if(tmp.length() == 1)
  tmp.insert(0, "0");
  tmp+=sep;
  
  std::cout << tmp.c_str();
  return *this;
}

Date &Date::setMes(unsigned int m, std::string sep)
{
  tmp = std::to_string(m);
  if(tmp.length() == 1)
  tmp.insert(0, "0");
  tmp+=sep;
  
  std::cout << tmp.c_str();
  return *this;
}

Date &Date::setAno(unsigned int a, std::string sep)
{
  tmp = std::to_string(a);
  if(tmp.length() == 1)
  tmp.insert(0, "0");
  tmp+=sep;
 
  std::cout << tmp.c_str();
  return *this;
}

Date &Date::setDia(unsigned int d)
{
  tmp = std::to_string(d);
  if(tmp.length() == 1)
  tmp.insert(0, "0");
  
  std::cout << tmp.c_str()<<".";
  return *this;
}

Date &Date::setMes(unsigned int m)
{
  tmp = std::to_string(m);
  if(tmp.length() == 1)
  tmp.insert(0, "0");
  
  std::cout << tmp.c_str()<<".";
  return *this;
}

Date &Date::setAno(unsigned int a)
{
  tmp = std::to_string(a);
  if(tmp.length() == 1)
  tmp.insert(0, "0");
  
  std::cout << tmp.c_str()<<".";
  return *this;
}

Date &Date::setDia()
{
  tmp = std::to_string(dia);
  if(tmp.length() == 1)
  tmp.insert(0, "0");
  tmp+=".";
  std::cout << tmp.c_str();
  return *this;
}

Date &Date::setMes()
{
  tmp = std::to_string(mes);
  if(tmp.length() == 1)
  tmp.insert(0, "0");
  tmp+=".";
  std::cout << tmp.c_str();
  return *this;
}

Date &Date::setAno()
{
  tmp = std::to_string(ano);
  if(tmp.length() == 1)
  tmp.insert(0, "0");
  tmp+=".";
  std::cout << tmp.c_str();
  return *this;
}

std::string Date::getDia()
{
  tmp = std::to_string(dia);
  if(tmp.length() == 1)
  tmp.insert(0, "0");
  
  return tmp.c_str();
}

std::string Date::getMes()
{
  tmp = std::to_string(mes);
  if(tmp.length() == 1)
  tmp.insert(0, "0");
  
  return tmp.c_str();
}

std::string Date::getAno()
{
  tmp = std::to_string(ano);
  if(tmp.length() == 1)
  tmp.insert(0, "0");
  
  return tmp.c_str();
}

int main()
{
  Date data;
  
  std::cout << "\n\t";
  data.setDia(01,"-").setMes(07,"-").setAno(00,"");
  
  std::cout << "\n\n\t";
  data.setDia(01).setMes(07).setAno(00);
  
  std::cout << "\n\n\t";
  data.setMes(07).setAno(00).setDia(01);
  
  std::cout << "\n\n\t";
  data.setAno(00).setDia(01).setMes(07);
  
  Date sd;
  std::cout << "\n\n\t"<<sd;
  
  std::cout << "\n\n\t";
  sd.setDia().setMes().setAno();
  
  std::cout << "\n\n\t";
  sd.setMes().setAno().setDia();
  
  std::cout << "\n\n\t";
  sd.setAno().setDia().setMes();
  
  std::cout << "\n\n\t";
  sd.setDia(sd.dia,"-").setMes(sd.mes,"-").setAno(sd.ano,"");
  
  std::cout << "\n\n\t";
  sd.setAno(sd.ano,"-").setMes(sd.mes,"-").setDia(sd.dia,"");
  
  std::cout << "\n\n";
  return 0;
}