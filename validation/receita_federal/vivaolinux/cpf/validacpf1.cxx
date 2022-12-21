#include <cstring>
#include <iostream>
/*
site que gera cpf's validos
http://geradordecpf.clevert.com.br/
Programinha em C++ que checa se um número de CPF é válido

Gerador de pessoas validas.
https://www.4devs.com.br/gerador_de_pessoas

validacpf1.cxx - feb/2018 - Jean Zonta

Modificação do Script Referência: 
 https://www.vivaolinux.com.br/script/Validar-CPF-em-C-esse-funciona
 https://www.vivaolinux.com.br/script/Formatar-strings-em-C-(RG-telefone-CEP-etc)/
*/

struct validation
{
 validation();
 ~validation();
 
 validation(char *);
 
 void getCpf();
 
 private:
 
 bool isCpf();
 
 void fmtCpf(char *);
 
 bool is_valid(const int *);
 
 char* fmtCpf(char *, const char *);
 
 void print();
 
 int cpf[11];
 
 char *_input;
 
}validation;

int main()
{
 validation.getCpf();
 std::cout<<"\n";
 return 0;
}

validation::validation(){}

inline validation::~validation()
{
  cpf[0]=0;
  _input=0;
}

inline validation::validation(char *input): _input(input)
 {
  for(int i = 0; i < 11; i++)
   {
    //Convertendo char para valor absoluto segundo tabela ASCII e passando para array de inteiros// 
    cpf[i] = static_cast<int>(_input[i] - 48);
    
    //Validando a entrada de dados
    if(cpf[i] < 0 || cpf[i] > 9)std::cout << "\n\tENTRADA INVÁLIDA\n";
   }
   
  for(int i = 0; i < 11; i++)
   {
    cpf[i];
    if(i == 2 || i == 5);
    if(i == 8);
   }
 }
 
inline void validation::getCpf()
 {
  char input[12]={0};
  do{
     std::cout << "\n\tInforme o CPF sem pontos, espaços ou traços\n\tDigite: ";
     std::cin.getline(input, sizeof(input), '\n');
     
     validation(input).print();
     
    }while(!validation(input).isCpf());
 }
 
inline bool validation::is_valid(const int * cpf)
 {
  for(int i = 1; i < sizeof(cpf); i++)
   if(cpf[i] != cpf[i-1])
    return true;
     else
      return false;
 }
 
inline bool validation::isCpf()
 {
  int digito1;
  int digito2;
  int temp = 0;
  const int *cpf;
  
    /*
    Obtendo o primeiro digito verificador:
    
    Os 9 primeiros algarismos são multiplicados
    pela sequência 10, 9, 8, 7, 6, 5, 4, 3, 2
    (o primeiro por 10, o segundo por 9, e assim por diante);
    Em seguida, calcula-se o resto “r1″ da divisão da,
    soma dos resultados das multiplicações por 11,
    e se o resto for zero ou 1, digito é zero,
    caso contrário digito = (11-r1)
    */  
    for(char i = 0; i < 9; i++)
     temp += (cpf[i] * (10 - i));
      temp %= 11;
    
    if(temp < 2)
     digito1 = 0;
      else
       digito1 = 11 - temp;
    
    /*
    Obtendo o segundo digito verificador:
    
    O dígito2 é calculado pela mesma regra, 
    porém inclui-se o primeiro digito verificador ao final
    da sequencia. Os 10 primeiros algarismos são 
    multiplicados pela sequencia 11, 10, 9, ... etc...
    (o primeiro por 11, o segundo por 10, e assim por diante);
    procedendo da mesma maneira do primeiro digito
    */
    temp = 0;
     for(char i = 0; i < 10; i++)
      temp += (cpf[i] * (11 - i));
       temp %= 11;
    
    if(temp < 2)
     digito2 = 0;
      else
       digito2 = 11 - temp;
          
    /* 
    Se os digitos verificadores obtidos
    forem iguais aos informados pelo usuário,
    então o CPF é válido.
    */
    if(digito1 == cpf[9] && digito2 == cpf[10])
     return true;
      else
       return false;
 }
 
inline void validation::fmtCpf(char *fmt)
 {
  for(int i = 0; i < 11; i++)
   {
    std::cout << fmt[i];
     if(i == 2 || i == 5)
      std::cout << ".";
     if(i == 8)
      std::cout << "-";
   }
 }
 
inline char* validation::fmtCpf(char *var, const char *fmt)
 {
  int i = 0;
  char aux[strlen(var)-1];
   
   while(*var)
    {
     if(fmt[i] != '#')
      {
       aux[i] = fmt[i];
       i++;
      }
      else
      {
       aux[i] = *var;
       var++;
       i++;
      }
    }
   aux[i] = 0;
  return strcpy(var, aux);
 }
  
inline void validation::print()
 {
  int i;
  enum { Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
  /* 
  Se todos os digitos forem iguais
  então o CPF é inválido.
  */
  if(!is_valid(cpf))
   {
    std::cout<< "\n\tCpf: ";
    fmtCpf(_input);
    std::cout<<" is Invalid\n";
    getCpf();
   }
   else
   /*pega o valor do nono digito*/
  for(i = 0; i < 9; i++)cpf[9]=cpf[i];
  
  switch(cpf[i])
  {
   case Zero:
    
    if(!validation(_input).isCpf())break;
    else
    std::cout << "\n\tCpf: ";
    fmtCpf(_input);
    std::cout << " foi emitido no estado de RS.\n";
    
   break;
   
   case One:
    
    if(!validation(_input).isCpf())break;
    else
    std::cout << "\n\tCpf: ";
    fmtCpf(_input);
    std::cout << " foi emitido em algum dos estados: DF-GO-MS-MT-TO.\n";
    
   break;
   
   case Two:
    
    if(!validation(_input).isCpf())break;
    else
    std::cout << "\n\tCpf: ";
    fmtCpf(_input);
    std::cout << " foi emitido em algum dos estados: AC-AM-AP-PA-RO-RR.\n";
    
   break;
   
   case Three:
    
    if(!validation(_input).isCpf())break;
    else
    std::cout << "\n\tCpf: ";
    fmtCpf(_input);
    std::cout << " foi emitido em algum dos estados: CE-MA-PI.\n";
    
   break;
   
   case Four:
    
    if(!validation(_input).isCpf())break;
    else
    std::cout << "\n\tCpf: ";
    fmtCpf(_input);
    std::cout << " foi emitido em algum dos estados: AL-PB-PE-RN.\n";
    
   break;
   
   case Five:
    
    if(!validation(_input).isCpf())break;
    else
    std::cout << "\n\tCpf: ";
    fmtCpf(_input);
    std::cout << " foi emitido em algum dos estados: BA-SE.\n";
    
   break;
   
   case Six:
    
    if(!validation(_input).isCpf())break;
    else
    std::cout << "\n\tCpf: ";
    fmtCpf(_input);
    std::cout << " foi emitido no estado de MG.\n";
    
   break;
   
   case Seven:
    
    if(!validation(_input).isCpf())break;
    else
    std::cout << "\n\tCpf: ";
    fmtCpf(_input);
    std::cout << " foi emitido em algum dos estados: ES-RJ.\n";
    
   break;
   
   case Eight:
    
    if(!validation(_input).isCpf())break;
    else
    std::cout << "\n\tCpf: "; 
    fmtCpf(_input);
    std::cout << " foi emitido no estado de SP.\n";
    
   break;
   
   case Nine:
    
    if(!validation(_input).isCpf())break;
    else 
    std::cout << "\n\tCpf: ";
    fmtCpf(_input);
    std::cout << " foi emitido em algum dos estados: PR-SC.\n";
    
   break;
  }
    /*
    Se o cpf com digitos diferentes
    for invalido retorna mensagen.
    */
    if(!validation(_input).isCpf())    
     std::cout << "\n\tCpf: "
               << fmtCpf(_input,"###.###.###-##")
               << (validation(_input).isCpf()?"":" is Invalid\n");
 }