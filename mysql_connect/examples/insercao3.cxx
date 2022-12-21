#include <cstdlib>
#include <iostream>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

int main(void)
{
 std::cout << "Let's have MySQL count from 10 to 1...\n";
 
 try
  {
   sql::Driver *driver;
   sql::Connection *con;
   sql::Statement *stmt;
   sql::ResultSet *res;
   sql::PreparedStatement *pstmt;

   /* Create a connection */
   driver = get_driver_instance();
   con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
   /* Connect to the MySQL test database */
   con->setSchema("teste");
   
   stmt = con->createStatement();
   stmt->execute("DROP TABLE IF EXISTS test");
   stmt->execute("CREATE TABLE test(id INT)");
   delete stmt;

   /* '?' is the supported placeholder syntax */
   pstmt = con->prepareStatement("INSERT INTO test(id) VALUES (?)");
   for (int i = 1; i <= 10; i++)
   {
    pstmt->setInt(1, i);
    pstmt->executeUpdate();
   }
   delete pstmt;
  
   /* Select in ascending order */
   pstmt = con->prepareStatement("SELECT id FROM test ORDER BY id ASC");
   res = pstmt->executeQuery();
   
   /* Fetch in reverse = descending order! */
   res->afterLast();
   while (res->previous())
   std::cout << "\t... MySQL counts: " << res->getInt("id") << "\n";
  delete res;

  delete pstmt;
  delete con;
  
  }
  catch(sql::SQLException &e)
  {
   std::cout << "# ERR: SQLException in " << __FILE__
             << "(" << __FUNCTION__ << ") on line "
             << __LINE__
             << "# ERR: "
             << e.what()
             << " (MySQL error code: "
             << e.getErrorCode()
             << ", SQLState: "
             << e.getSQLState() <<" )\n";
  }

 std::cout << "\n\n";
 
 return EXIT_SUCCESS;
 
}