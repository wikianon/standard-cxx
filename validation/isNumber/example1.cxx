#include <regex>
#include <sstream>
#include <iostream>

bool isOk(std::string x)
{
 std::string s;
 
 while(s.size() > 1 && x[0] == ' ')x.erase(x.begin());
 
 int ini = x[0] == '-';

 for(int i=ini; i<x.size(); i++)
  if('0' <= x[i] && x[i] <= '9')
   continue;
   else
   return false;
   return true;
}

int toNumber(std::string x)
{
 std::string s;
 while(s.size() > 1 && x[0] == ' ')x.erase(x.begin());
    
  int ini = x[0] == '-', res = 0;
    
  for(int i=ini; i<x.size(); i++)
   {
    res *= 10;
    res += x[i] - '0';
   } 
 return ini?-res:res;
}

int isNumber(std::string str)
{
  while(getline( std::cin, str )) 
  {
   const char *idx = str.c_str( );
   
   // Mientras que no lleguemos al final de la cadena,
   // y el caracter sea un dígito.
   while( *idx && *idx >= '0' && *idx <= '9' )++idx;
    
   // Si llegamos al final de la cadena, la validación es correcta.
   if(!( *idx ) )break;
   
   std::cout << "\n\tEntrada inválida.\n"; 
  }
} 

bool isIntReg(std::string numero)
{
 std::smatch number_smatch;
 
 std::regex pattern("^[0-9]+$");
 
 return std::regex_match(numero, number_smatch, pattern);
}

void getIntReg()
{
 std::string num;
  
 do{
    std::cout<<"\n\tDigite um numero: ";
    std::cin>>num;
    
    std::cout<<"\n\tNumber: "+num+(isIntReg(num)?" is Valid\n\n":" is Invalid\n");
     
   }while(!isIntReg(num));
}

void getNum1()
{
 std::string s;
 do{
    std::cout<<"\n\tDigite um numero: ";
    std::cin >> s;
    
    if(isOk(s))
     std::cout << "\n\tIs Number " << toNumber(s) << "\n\n";
    else
     std::cout << "\n\tIs Invalid.\n\n";
    
   }while(isOk(s) == 0);
}

void getNum2()
{
 std::string s;
 do{
    std::cout<<"\n\tDigite um numero: ";
    std::cin >> s;
    
    if(isOk(s))
     std::cout << "\n\tIs Number " << s << "\n\n";
    else
     std::cout << "\n\tIs Invalid.\n\n";
    
   }while(isOk(s) == 0);
}

bool checkNumber(std::string &s) 
{
 std::istringstream ssIn(s);
 int n;
 
 if(ssIn >> n)
  {
   //std::cout << n << std::endl;
   // we got a number, feed it back
   std::stringstream ssOut;
   ssOut << n;
   s = ssOut.str();	  
   return true;
  }
  else
  s.clear();
 return false;
}

void getBool()
{
 std::string num;
 
 do{ 
    std::cout<<"\n\tDigite um numero: ";
    std::cin>>num;
  
    if(checkNumber(num) == 0)
     std::cout<<"\n\tNao é um numero\n\n";
    else
     std::cout<<"\n\tÉ um numero.\n\n";
    
   }while(checkNumber(num) == 0);  
}

bool isNumber(const char* input)
{
  for(; *input; ++input)
  {// En cuanto un caracter no sea numérico
   if('0' > *input || '9' < *input)return false;
  }
  
  return true;
}

int isInt()
{
 int dado = -1;
 std::string entrada = ""; //melhor ler uma string e tentar converter depois
 
 while(true)
 {
  std::cout << "Digite um numero inteiro: ";
  getline(std::cin, entrada); //pede um stream de uma linha toda
  std::cout << std::endl;
  std::stringstream myStream(entrada);
  
  if(!(myStream >> dado))
   { //se não conseguir fazer a conversão
    std::cout << "\n\tValor inválido, tente novamente\n\n";
    continue;
   }
  if(dado < 0)
   { //se digitar o número que determina encerrar
    break;
   }
 }
}

int main()
{
 getNum2();
  //getBool();
 //getIntReg();
 return 0;
}