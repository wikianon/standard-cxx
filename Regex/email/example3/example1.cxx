#include <regex> //count
#include <iostream>
/*
int checkEmail(std::string email)
{
 email.find("@");
 if(email.find("@")>email.length())
  return 0;
 else 
  return 1;
}

bool isMail(std::string _mail)
{
  if(count(_mail.begin(), _mail.end(), '@') != 1)
  return false;
  
  const std::string::size_type pos_at = _mail.find('@');
  
  if(pos_at == 0 || (pos_at == (_mail.length() - 1)))
  return false;
  
  if(_mail.find('.', pos_at) == std::string::npos)
  return false;
  else
  return true;
}

bool ValidarEmail(std::string email)
{
 bool validEmail = false;
 
 int indexArr = email.find('@');
 
 if(indexArr > 0)
  {
   int indexDot = email.find('.', indexArr);
   
   if(indexDot - 1 > indexArr)
    {
     if(indexDot + 1 < email.length())
      {
       std::string indexDot2 = email.substr(indexDot + 1, 1);
       if(indexDot2 != ".")validEmail = true;
      }
    }
  }
 return validEmail;
}

bool validaEmail(std::string email)
{
 std::string servidor("@gmail.com");
 
 std::string pedaco(email.substr(email.size()-servidor.size()));
 
 return !pedaco.compare(servidor);
}

const char* validMails(std::string input)
{
 size_t at = input.find('@');
 
 if(at == std::string::npos)return "Falta @ simbolo\n";
 
 size_t dot1 = input.find('.', at+1);
 if(dot1 == std::string::npos)return "Falta . simbolo after @\n";
  
 size_t dot2 = input.find('.', dot1+1);
 if(dot2 == std::string::npos)return "Falta . simbolo after first .\n";
}

int validMail(std::string input)
{
 size_t at = input.find('@');
 
 if(at == std::string::npos)
  {
   std::cout << "Falta @ simbolo\n";
   return 1;
  }
  
 size_t dot1 = input.find('.', at+1); 
 if(at < std::string::npos)
  { 
   if(dot1 == std::string::npos)
    {
     std::cout << "Falta . simbolo after @\n";
     return 2;
    }
  }
  
 size_t dot2 = input.find('.', dot1+1);
 if(dot2 == std::string::npos)
  {
   std::cout << "Falta . simbolo after first .\n";
   return 2;
  }
}
*/

void valid(std::string email)
{
 int at = email.find('@');

 int ponto1 = email.find('.',at+1);
 int ponto2 = email.find('.',ponto1+1);
 
 int br = email.find(".br",ponto2+1);
 int com = email.find(".com",ponto1+1);
 int combr = email.find(".com.br",ponto1+1);
 
 if(email.length() > email.find('@') && email.length() > ponto1 && email.length() < combr)std::cout << "Falta . simbolo antes de segundo .\n"; //usuario@yahoocom.br usuario@terracom.br usuario@hotmailcom.br
 else
 if(email.length() > email.find('@') && email.length() > ponto1 && email.length() < br)std::cout << "Falta . simbolo apos primeiro .\n"; //usuario@yahoo.combr usuario@terra.combr usuario@hotmail.combr
 else
 if(email.length() < email.find('@') && email.length() > ponto1 && email.length() < br)std::cout << "Falta @ simbolo e . simbolo\n"; //usuarioyahoo.combr  usuarioterra.combr usuariohotmail.combr
 else
 if(email.length() < email.find('@') && email.length() < com)std::cout << "Falta @ simbolo e . simbolo\n"; //usuariogmailcom
 else
 if(email.length() > email.find('@') && email.length() < ponto1)std::cout << "Falta . simbolo apos @\n"; //usuario@gmailcom
 else
 if(email.length() < email.find('@')) std::cout << "Falta @ simbolo\n"; //usuariogmail.com , usuario{yahoo,terra,hotmail}.com.br
}

int main()
{
 std::string email;
 
 std::cout<<"\nDigite seu e-mail: ";
 std::cin>>email;
 valid(email);
 /*
 if(checkEmail(email))
  std::cout<<"Email is Valid.\n\n";
 else
  std::cout<<"Email is inValid.\n\n";
 
 if(isMail(email))
  std::cout<<"É uma conta google.\n\n";
 else
  std::cout<<"Não é uma conta google.\n\n";
 */
 
 return 0;
}