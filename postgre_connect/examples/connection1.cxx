#include <iostream>
#include <pqxx/pqxx> 
//Connecting To Database





















int main(int argc, char* argv[])
{
 try{
      pqxx::connection login("dbname = teste user = postgres password = cohondob hostaddr = 127.0.0.1 port = 5432");
      
      if(login.is_open())std::cout << "\n\tOpened database successfully: " << login.dbname() << "\n\n";
      else 
      {
       std::cout << "\n\tCan't open database\n\n";
        return 1;
      }
      login.disconnect();
    }
    catch(const std::exception &e)
    {
     std::cerr << e.what() << std::endl;
     return 1;
    }
  return 0;  
}