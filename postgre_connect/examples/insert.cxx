#include <iostream>
#include <pqxx/pqxx> 
//INSERT Operation

int main(int argc, char* argv[])
{
 const char *sql;
   
 try{
      pqxx::connection login("dbname = teste user = postgres password = cohondob hostaddr = 127.0.0.1 port = 5432");
      
      if(login.is_open())std::cout << "\n\tOpened database successfully: " << login.dbname() << "\n";
      else 
      {
       std::cout << "\n\tCan't open database\n";
       return 1;
      }

      /* Create SQL statement */
      sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) VALUES (1, 'Paul', 32, 'California', 20000.00 );" \
            "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) VALUES (2, 'Allen', 25, 'Texas', 15000.00 );"  \
            "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
            "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

      /* Create a transactional object. */
      pqxx::work tr_obj(login);
      
      /* Execute SQL query */
      tr_obj.exec(sql);
      tr_obj.commit();
      std::cout << "\n\tRecords created successfully\n\n";
      login.disconnect();
    }
    catch(const std::exception &e)
    {
     std::cerr << e.what() << std::endl;
     return 1;
    }
 return 0;
}