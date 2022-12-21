#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <mysql_connection.h>
#include <include/mysql_driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

int main(void)
{
  try{
      sql::Driver *driver;
      sql::Connection *con;
      sql::PreparedStatement *prep_stmt;
      
      std::cout << "connecting to mysql server....";
      driver = get_driver_instance();
      con = driver->connect("tcp://127.0.0.1:3306", "root", "");
      std::cout << "connected\n";
      
      con->setSchema("teste");
      prep_stmt = con->prepareStatement("insert into person(person_id,name,age,status,id_faz) values (?,?,?,?,?)");
      
      int i = 0;
      char name[50];
      int  age;
      char status[20];
      int  id_faz;
      int  person_id;
      
      std::cout << "inserting 10 data....";
      
      for(i=1;i<=10;i++)
       {
        prep_stmt->setInt(i,person_id);
        prep_stmt->setString(1,name);
        prep_stmt->setInt(2,age);
        prep_stmt->setString(3,status);
        prep_stmt->setInt(4,id_faz);
        prep_stmt->execute();
       }
       
       std::cout << "done\n";
       delete prep_stmt;
       delete con;
      }
      catch (sql::SQLException &e)
      {
       std::cout << "# ERR: " << e.what()
                 << " (MySQL error code: " << e.getErrorCode()
                 << ", SQLState: " << e.getSQLState() << " )\n\n";
      }
  return EXIT_SUCCESS;
}