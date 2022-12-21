#include <memory>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

/**
 * https://stackoverflow.com/questions/14570347/mysql-connector-c-bad-access-crash/14573340#14573340
 * Usage example for Driver, Connection, (simple) Statement, ResultSet
 */

int main(void)
{
 std::string url("localhost");
 std::string user("root");
 std::string pass("");
 std::string database("teste");

 try{
     sql::Driver *driver;
     sql::Connection *con;
     sql::Statement *stmt;
     sql::ResultSet *res;
     sql::PreparedStatement *pstmt;
    
     driver = sql::mysql::get_driver_instance();
     con = driver->connect(url, user, pass);
     con->setSchema(database);
     stmt = con->createStatement();
    
     res = stmt->executeQuery("select * from person");
     
     while(res->next())
     {
      std::cout << "\n\tId Faz: "<<res->getInt64("id_faz")
                << "\n\tId Pessoa: "<<res->getInt64("person_id")
                << "\n\tName: "<<res->getString("name")
                << "\n\tIdade: "<< res->getInt64("age")
                << "\n\tStatus: "<< res->getString("status")
                << "\n\tDateTime: "<< res->getString("date_person")<< "\n"; // type may need to be different
     }
     delete driver;
     delete con;
     delete stmt;
     delete res;
    } 
    catch (sql::SQLException &e) 
    {
     std::cerr << "# ERR: SQLException in " 
               << __FILE__
               << "(" << __FUNCTION__ 
               << ") on line " 
               << __LINE__
               << "\n# ERR: " 
               << e.what()
               << " (MySQL error code: " 
               << e.getErrorCode()
               << ", SQLState: " 
               << e.getSQLState() 
               << " )\n";
    }
  std::cout<<std::endl;  
  return 0;  
}