#include <iostream>

bool uol(std::string);
bool bol(std::string);
bool gmail(std::string);
bool yahoo(std::string);
bool terra(std::string);
bool hotmail(std::string);

void menu();

int main() 
{
 menu();
 return 0;
}

void menu()
{
 std::string str;
 enum { Uol=1, Bol, Gmail, Terra, Yahoo, Hotmail, Exit }op;
 
 do{
    std::cout << "\n\tValidation Email"
                 "\n\t[1]-Uol"
                 "\n\t[2]-Bol"
                 "\n\t[3]-Gmail" 
                 "\n\t[4]-Terra"
                 "\n\t[5]-Yahoo"
                 "\n\t[6]-Hotmail"
                 "\n\t[7]-Exit"
                 "\n\tEnter: ";
    scanf("%d",&op);
    
    switch(op)
     {      
      case Uol:
      
      do{       
         std::cout << "\n\tEnter your uol e-mail: ";
         std::cin >> str;
         
         std::cout << "\n\tEmail: " << str << (uol(str)?" is Valid.\n\n":" is Invalid.\n\n");
          
        }while(!uol(str));
            
      break;
      
      case Bol:
      
      do{       
         std::cout << "\n\tEnter your bol e-mail: ";
         std::cin >> str;
         
         std::cout << "\n\tEmail: " << str << (bol(str)?" is Valid.\n\n":" is Invalid.\n\n");
          
        }while(!bol(str));
              
      break;
      
      case Gmail:
      
      do{       
         std::cout << "\n\tEnter your google e-mail: ";
         std::cin >> str;
         
         std::cout << "\n\tEmail: " << str << (gmail(str)?" is Valid.\n\n":" is Invalid.\n\n");
          
        }while(!gmail(str));
               
      break;
      
      case Terra:
      
      do{
         std::cout << "\n\tEnter your terra e-mail: ";
         std::cin >> str;
         
         std::cout << "\n\tEmail: " << str << (terra(str)?" is Valid.\n\n":" is Invalid.\n\n");
          
        }while(!terra(str));
              
      break;
      
      case Yahoo:
      
      do{
         std::cout << "\n\tEnter your yahoo e-mail: ";
         std::cin >> str;
         
         std::cout << "\n\tEmail: " << str << (yahoo(str)?" is Valid.\n\n":" is Invalid.\n\n");
          
        }while(!yahoo(str));
             
      break;
      
      case Hotmail:
      
      do{
         std::cout << "\n\tEnter your hotmail e-mail: ";
         std::cin >> str;
       
         std::cout << "\n\tEmail: " << str << (hotmail(str)?" is Valid.\n\n":" is Invalid.\n\n");
          
        }while(!hotmail(str));
      
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

bool uol(std::string email)
{
 std::string servidor("@uol.com.br");
 
 std::string pedaco(email.substr(email.size()-servidor.size()));
 
 return !pedaco.compare(servidor);
}

bool bol(std::string email)
{
 std::string servidor("@bol.com.br");
 
 std::string pedaco(email.substr(email.size()-servidor.size()));
 
 return !pedaco.compare(servidor);
}

bool gmail(std::string email)
{
 std::string servidor("@gmail.com");
 
 std::string pedaco(email.substr(email.size()-servidor.size()));
 
 return !pedaco.compare(servidor);
}

bool terra(std::string email)
{
 std::string servidor("@terra.com.br");
 
 std::string pedaco(email.substr(email.size()-servidor.size()));
 
 return !pedaco.compare(servidor);
}

bool yahoo(std::string email)
{
 std::string servidor("@yahoo.com.br");
 
 std::string pedaco(email.substr(email.size()-servidor.size()));
 
 return !pedaco.compare(servidor);
}

bool hotmail(std::string email)
{
 std::string servidor("@hotmail.com.br");
 
 std::string pedaco(email.substr(email.size()-servidor.size()));
 
 return !pedaco.compare(servidor);
}