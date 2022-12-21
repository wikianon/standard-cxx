#include <memory>
#include <iostream>

//base class validation
struct validation
{
 validation(){}
 virtual ~validation(){}
  
 //pure virtual function to enforce reimplementation
 virtual void menu() = 0;
 virtual bool isValid() = 0;
 
 protected:
  std::string str;
  std::unique_ptr<validation> is_val; //smart pointer
};

//function foward declaration
void menu_All();

int main()
{
 menu_All();
 return 0;
}

struct uol: validation
{
 uol(){}
 
 uol(std::string mail): _mail(mail){}
 
 ~uol()
  {
   str.clear(); 
   _mail.clear(); 
  }
  
  bool isValid() override
  {
   std::string server("@uol.com.br");
   
   std::string piece(_mail.substr(_mail.size()-server.size()));
   
   return !piece.compare(server);
  }
  
  void menu() override
  {
   do{
      std::cout << "\n\tEnter your uol e-mail: ";
      std::cin >> str;
      
      is_val = std::make_unique<uol>(str);
      //is_val = std::unique_ptr<uol>(new uol(str));
      
      std::cout << "\n\tEmail: " << str << (is_val->isValid()?" is Valid.\n\n":" is Invalid.\n\n");
      
     }while(!is_val->isValid());
  }
  
 private:
  std::string _mail;
};

struct bol: validation
{
 bol(){}
 
 bol(std::string mail): _mail(mail){}
 
 ~bol()
  {
   str.clear(); 
   _mail.clear();
  }
  
  bool isValid() override
  {
   std::string server("@bol.com.br");
  
   std::string piece(_mail.substr(_mail.size()-server.size()));
   
   return !piece.compare(server);
  }
  
  void menu() override
  {
   do{
      std::cout << "\n\tEnter your bol e-mail: ";
      std::cin >> str;
      
      is_val = std::make_unique<bol>(str);
      //is_val = std::unique_ptr<bol>(new bol(str));
      
      std::cout << "\n\tEmail: " << str << (is_val->isValid()?" is Valid.\n\n":" is Invalid.\n\n");
      
     }while(!is_val->isValid());
  }
  
 private:
  std::string _mail;
};

struct gmail: validation
{
 gmail(){}
 
 gmail(std::string mail): _mail(mail){}
 
 ~gmail()
  {
   str.clear(); 
   _mail.clear();
  }
  
  bool isValid() override
  {
   std::string server("@gmail.com");
   
   std::string piece(_mail.substr(_mail.size()-server.size()));
   
   return !piece.compare(server);
  }
  
  void menu() override
  {
   do{
      std::cout << "\n\tEnter your google e-mail: ";
      std::cin >> str;
      
      is_val = std::make_unique<gmail>(str); 
      //is_val = std::unique_ptr<gmail>(new gmail(str));
      
      std::cout << "\n\tEmail: " << str << (is_val->isValid()?" is Valid.\n\n":" is Invalid.\n\n");
       
     }while(!is_val->isValid());
  }
  
 private:
  std::string _mail;
};

struct earth: validation
{
 earth(){}
 
 earth(std::string mail): _mail(mail){}
 
 ~earth()
  {
   str.clear(); 
   _mail.clear();
  }
  
  bool isValid() override
  {
   std::string server("@terra.com.br");
   
   std::string piece(_mail.substr(_mail.size()-server.size()));
   
   return !piece.compare(server);
  }
  
  void menu() override
  {
   do{
      std::cout << "\n\tEnter your earth e-mail: ";
      std::cin >> str;
      
      is_val = std::make_unique<earth>(str);
      //is_val = std::unique_ptr<earth>(new earth(str));
      
      std::cout << "\n\tEmail: " << str << (is_val->isValid()?" is Valid.\n\n":" is Invalid.\n\n");
      
     }while(!is_val->isValid());
  }
  
 private:
  std::string _mail;
};

struct yahoo: validation
{
 yahoo(){}
 
 yahoo(std::string mail): _mail(mail){}
 
 ~yahoo()
  {
   str.clear(); 
   _mail.clear();
  }
  
  bool isValid() override
  {
   std::string server("@yahoo.com.br");
   
   std::string piece(_mail.substr(_mail.size()-server.size()));
   
   return !piece.compare(server);
  }
  
  void menu() override
  {
   do{
      std::cout << "\n\tEnter your yahoo e-mail: ";
      std::cin >> str;
      
      is_val = std::make_unique<yahoo>(str);
      //is_val = std::unique_ptr<yahoo>(new yahoo(str));
      
      std::cout << "\n\tEmail: " << str << (is_val->isValid()?" is Valid.\n\n":" is Invalid.\n\n");
      
     }while(!is_val->isValid());
  }
  
 private:
  std::string _mail;
};

struct hotmail: validation
{
 hotmail(){}
 
 hotmail(std::string mail): _mail(mail){}
 
 ~hotmail()
  {
   str.clear(); 
   _mail.clear();
  }
  
  bool isValid() override
  {
   std::string server("@hotmail.com.br");
   
   std::string piece(_mail.substr(_mail.size()-server.size()));
   
   return !piece.compare(server);
  }
  
  void menu() override
  {
   do{
      std::cout << "\n\tEnter your hotmail e-mail: ";
      std::cin >> str;
      
      is_val = std::make_unique<hotmail>(str); 
      //is_val = std::unique_ptr<hotmail>(new hotmail(str));
      
      std::cout << "\n\tEmail: " << str << (is_val->isValid()?" is Valid.\n\n":" is Invalid.\n\n");
      
     }while(!is_val->isValid());
  }
  
 private:
  std::string _mail;
};

void menu_All()
{
 std::unique_ptr<validation> m_val;
 
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
       m_val = std::make_unique<uol>();
       m_val->menu();
      break;
      
      case Bol:
       m_val = std::make_unique<bol>();
       m_val->menu();
      break;
      
      case Gmail:
       m_val = std::make_unique<gmail>();
       m_val->menu();
      break;
      
      case Earth:
       m_val = std::make_unique<earth>();
       m_val->menu();
      break;
      
      case Yahoo:
       m_val = std::make_unique<yahoo>();
       m_val->menu();
      break;
      
      case Hotmail:
       m_val = std::make_unique<hotmail>();
       m_val->menu();
      break;
      
      case Exit:
       std::cout << "\n\tGood Bye!\n\n";
      break;
      
      default:
       std::cout << "\n\tInvalid Option!\n\n";
      break;
     }
   }while(op != Exit);
}