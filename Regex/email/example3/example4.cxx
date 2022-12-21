#include <iostream>

//base class
struct validation
{
 validation(){}
 ~validation(){}
 //pure virtual function to enforce reimplementation
 virtual bool isValid() = 0;
};

void menu();

int main() 
{
 menu();
 return 0;
}

struct uol: validation
{
 uol(std::string mail): _mail(mail){}
 
 ~uol()
  {
   if(!_mail.empty())_mail.clear();  
  }
  
  bool isValid() override
  {
   std::string server("@uol.com.br");
   
   std::string piece(_mail.substr(_mail.size()-server.size()));
   
   return !piece.compare(server);
  }
  
 private:
  std::string _mail; 
};

struct bol: validation
{
 bol(std::string mail): _mail(mail){}
 
 ~bol()
  {
   if(!_mail.empty())_mail.clear();  
  }
  
  bool isValid() override
  {
   std::string server("@bol.com.br");
   
   std::string piece(_mail.substr(_mail.size()-server.size()));
   
   return !piece.compare(server);
  }
  
 private:
  std::string _mail; 
};

struct gmail: validation
{
 gmail(std::string mail): _mail(mail){}
 
 ~gmail()
  {
   if(!_mail.empty())_mail.clear(); 
  }
  
  bool isValid() override
  {
   std::string server("@gmail.com");
   
   std::string piece(_mail.substr(_mail.size()-server.size()));
   
   return !piece.compare(server);
  }
  
 private:
  std::string _mail; 
};

struct yahoo: validation
{
 yahoo(std::string mail): _mail(mail){}
 
 ~yahoo()
  {
   if(!_mail.empty())_mail.clear();  
  }
  
  bool isValid() override
  {
   std::string server("@yahoo.com.br");
   
   std::string piece(_mail.substr(_mail.size()-server.size()));
   
   return !piece.compare(server);
  }
  
 private:
  std::string _mail; 
};

struct earth: validation
{
 earth(std::string mail): _mail(mail){}
 
 ~earth()
  {
   if(!_mail.empty())_mail.clear();  
  }
  
  bool isValid() override
  {
   std::string server("@terra.com.br");
   
   std::string piece(_mail.substr(_mail.size()-server.size()));
   
   return !piece.compare(server);
  }
  
 private:
  std::string _mail; 
};

struct hotmail: validation
{
 hotmail(std::string mail): _mail(mail){}
 
 ~hotmail()
  {
   if(!_mail.empty())_mail.clear();  
  }
  
  bool isValid() override
  {
   std::string server("@hotmail.com.br");
   
   std::string piece(_mail.substr(_mail.size()-server.size()));
   
   return !piece.compare(server);
  }
  
 private:
  std::string _mail; 
};

void menu()
{
 std::string str;
 validation* is_val[6];
 
 enum { Uol=1, Bol, Gmail, Earth, Yahoo, Hotmail, Exit }op;
 
 do{
    std::cout << "\n\tValidation e-mail"
                 "\n\t[1]-Uol"
                 "\n\t[2]-Bol"
                 "\n\t[3]-Gmail" 
                 "\n\t[4]-Earth"
                 "\n\t[5]-Yahoo"
                 "\n\t[6]-Hotmail"
                 "\n\t[7]-Exit"
                 "\n\tEnter: ";
    scanf("%d", &op);
    
    switch(op)
     {      
      case Uol:
      
      do{
         std::cout << "\n\tEnter your uol e-mail: ";
         std::cin >> str;
          
         is_val[0] = new uol(str);
         
         std::cout << "\n\tEmail: " << str << (is_val[0]->isValid()?" is Valid.\n\n":" is Invalid.\n\n");
          
        }while(!is_val[0]->isValid());
      
      break;
      
      case Bol:
      
      do{
         std::cout << "\n\tEnter your bol e-mail: ";
         std::cin >> str;
         
         is_val[1] = new bol(str);
         
         std::cout << "\n\tEmail: " << str << (is_val[1]->isValid()?" is Valid.\n\n":" is Invalid.\n\n");
          
        }while(!is_val[1]->isValid());
      
      break;
      
      case Gmail:
      
      do{
         std::cout << "\n\tEnter your goole e-mail: ";
         std::cin >> str;
         
         is_val[2] = new gmail(str);
         
         std::cout << "\n\tEmail: " << str << (is_val[2]->isValid()?" is Valid.\n\n":" is Invalid.\n\n");
          
        }while(!is_val[2]->isValid());
      
      break;
      
      case Earth:
      
      do{
         std::cout << "\n\tEnter your earth e-mail: ";
         std::cin >> str;
         
         is_val[3] = new earth(str);
         
         std::cout << "\n\tEmail: " << str << (is_val[3]->isValid()?" is Valid.\n\n":" is Invalid.\n\n");
         
        }while(!is_val[3]->isValid());
      
      break;
      
      case Yahoo:
      
      do{
         std::cout << "\n\tEnter your yahoo e-mail: ";
         std::cin >> str;
         
         is_val[4] = new yahoo(str);
         
         std::cout << "\n\tEmail: " << str << (is_val[4]->isValid()?" is Valid.\n\n":" is Invalid.\n\n");
         
        }while(!is_val[4]->isValid());
      
      break;
      
      case Hotmail:
      
      do{
         std::cout << "\n\tEnter your hotmail e-mail: ";
         std::cin >> str;
         
         is_val[5] = new hotmail(str);
         
         std::cout << "\n\tEmail: " << str << (is_val[5]->isValid()?" is Valid.\n\n":" is Invalid.\n\n");
          
        }while(!is_val[5]->isValid());
      
      break;
      
      case Exit:
         std::cout << "\n\tGood Bye!\n\n";
         exit(1);
      break;
      
      default:
         std::cout << "\n\tInvalid Option!\n\n";
      break; 
     }
   }while(1);
}