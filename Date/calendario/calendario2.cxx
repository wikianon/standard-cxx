#include <iostream>

int primeiro_dia(int, int);
int getMonthDays(int, int);
const char* getMonthNames(int);
void calendario(int);
void calendario(int, int);

int main()
{
 int mes, ano;
 
 std::cout << "\nDigite o mes e ano: ";
 std::cin >>ano;
 
 calendario(ano);
 return 0;
}

// pega primeiro dia do mes
int primeiro_dia(int mes, int ano)
{
 int a = ((14 - mes)/12);
 int y = (ano-a);
 int m = ((mes+(12*a))-2);
 int dia = 1;
 
 return dia = (((dia+y)+(y/4)-(y/100)+(y/400)+((31*m)/12))%7);
}

int getMonthDays(int mes, int ano)
{
 int month_days[12] = {31, 28, 31, 30, 31, 30, 31 ,31 ,30, 31, 30, 31};
 
 month_days[1] = (((ano % 4 == 0) && (ano % 100 != 0)) || (ano % 400 == 0)) ? 29 : 28;
 
 return month_days[mes-1];
}

const char* getMonthNames(int mes)
{
 const char *months[12]={ "January","February","March","April","May","June","July","August","September","October","November","December" };
 
 return months[mes-1];
}

//imprime calendario de todos os meses do ano
void calendario(int ano)
{
 std::cout << "\n";
 
 for(int mes = 1; mes <=12; mes++)
 {
  std::cout << getMonthNames(mes) << "-" << ano << "\nSun Mon Tue Wed Thu Fri Sat\n";
  
  int primeiro = primeiro_dia(mes, ano); //pega primeiro dia do mes
  
  for(int i=0; i < primeiro; i++)std::cout << "    ";
  
  int dia_mes = getMonthDays(mes, ano);
  
  for(int dia = 1; dia <= dia_mes; dia++)
  {
   printf("%3d ",dia);
   primeiro++;
   if(primeiro%7 == 0)std::cout << "\n";
  }
  std::cout << "\n\n";
 }
}

//imprime calendario de mes especifico
void calendario(int mes, int ano)
{
  std::cout << "\n" << getMonthNames(mes) << "-" << ano << "\nSun Mon Tue Wed Thu Fri Sat\n";
  
  int primeiro = primeiro_dia(mes, ano); //pega primeiro dia do mes
  
  for(int i=0; i < primeiro; i++)std::cout << "    ";
  
  int dia_mes = getMonthDays(mes, ano);
  
  for(int dia = 1; dia <= dia_mes; dia++)
  {
   printf("%3d ",dia);
   primeiro++;
   if(primeiro%7 == 0)std::cout << "\n";
  }
  std::cout << "\n\n";
}