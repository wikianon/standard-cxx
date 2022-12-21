#include <iostream>
#include <pqxx/pqxx> 
//Create a Table




int main(int argc, char* argv[])
{
 const char *sql;
   
 try{
      pqxx::connection login("dbname = teste user = postgres password = cohondob hostaddr = 127.0.0.1 port = 5432");
      
      if(login.is_open())std::cout << "\n\tOpened database successfully: " << login.dbname() << "\n";
      else
      {
       std::cout << "\n\tCan't open database\n\n";
       return 1;
      }      
      /* Create SQL statement */
      sql = "CREATE TABLE COMPANY("  \
      "ID INT PRIMARY KEY     NOT NULL," \
      "NAME           TEXT    NOT NULL," \
      "AGE            INT     NOT NULL," \
      "ADDRESS        CHAR(50)," \
      "SALARY         REAL );";

      /* Create a transactional object. */
      pqxx::work tr_obj(login);
      
      /* Execute SQL query */
      tr_obj.exec(sql);
      tr_obj.commit();
      std::cout << "\n\tTable created successfully\n\n";
      login.disconnect();
    }
    catch (const std::exception &e)
    {
     std::cerr << e.what() << std::endl;
      return 1;
    }
 return 0;
}