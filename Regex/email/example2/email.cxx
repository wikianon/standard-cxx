#include <regex>
#include <iostream>

class email
{
 struct Person
 {
  std::string eMail;
 };
 
 struct Contact
 {
  std::string eMail;
 };
  
 std::string _mail;
 
 public:
 
 email(){}
 
 email(Person& p): _mail(p.eMail){}
 
 email(Contact& c): _mail(c.eMail){}
 
 email(std::string mail): _mail(mail){}
 
 ~email()
  {
   if(!_email.empty())_mail.clear();  
  }
 
 bool isMail()
 {
  const std::regex pattern("^([a-zA-Z0-9._]+@((hotmail|terra|yahoo|uol|bol)[.](com[.]br))?((gmail)[.](com))?)$");
  
  return std::regex_match(_mail, pattern);
 }
 
 void print()
 {
  std::cout << "\n\tEmail: "+_mail+(isMail()?" is Valid\n":" is Invalid\n");
 }
 
 Contact getContact()
 {
  Contact c;
  
  do{
     std::cout << "\n\tEnter email: ";
     getline(std::cin, c.eMail);
     
     email(c).print();
     std::cout << "\n";
     
    }while(!email(c).isMail());
 
  return c;
 }
 
 void getEmail()
 {
  do{
     std::cout << "\n\tEnter email: ";
     getline(std::cin, _mail);
     
     email(_mail).print();
     std::cout << "\n";
     
    }while(!email(_mail).isMail());
 }
 
}email;

int main(void)
{
 //email.getEmail();
 email.getContact();
 std::cout << "\n";
 return 0;
}