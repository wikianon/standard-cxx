#include <memory>
#include <sstream>
#include <iostream>
#include <stdexcept>

/*http://download.nust.na/pub6/mysql/tech-resources/articles/mysql-connector-cpp.html
 *MySQL Connector/C++ specific headers */

#include <prepared_statement.h>
#include <resultset.h>
#include <metadata.h>
#include <resultset_metadata.h>
#include <warning.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/connection.h>

#define DBHOST "localhost"
#define USER "root"
#define PASSWORD ""
#define DATABASE "teste"

#define NUMOFFSET 100
#define COLNAME 200

using namespace std;

static void retrieve_data_and_print(sql::ResultSet *rs, int type, int colidx, std::string colname)
{
 /* retrieve the row count in the result set */
 std::cout << "\n\tRetrieved " << rs -> rowsCount() << " row(s).\n";

 std::cout << "\n\tFazNames"<< "\n--------\n\n";

 /* fetch the data : retrieve all the rows in the result set */
 while(rs->next())
 {
  if(type == NUMOFFSET)std::cout << rs -> getString(colidx) << std::endl;
  else 
  if(type == COLNAME)std::cout << rs -> getString(colname) << std::endl;
 } // while
 std::cout << std::endl;
} // retrieve_data_and_print()

static void retrieve_dbmetadata_and_print (sql::Connection *dbcon)
{
 if(dbcon -> isClosed())throw runtime_error("DatabaseMetaData FAILURE - database connection closed");

 std::cout << "\n\tDatabase Metadata\n\t-----------------\n\t"<< boolalpha;

 /* The following commented statement won't work with Connector/C++ 1.0.5 and later */
 //std::auto_ptr < DatabaseMetaData > dbcon_meta (dbcon -> getMetaData());

 sql::DatabaseMetaData *dbcon_meta = dbcon -> getMetaData();

   std::cout << "\n\tDatabase Product Name: " << dbcon_meta -> getDatabaseProductName()
             << "\n\tDatabase Product Version: " << dbcon_meta -> getDatabaseProductVersion()
             << "\n\tDatabase User Name: " << dbcon_meta -> getUserName()
             << "\n\n\tDriver name: " << dbcon_meta -> getDriverName()
             << "\n\tDriver version: " << dbcon_meta -> getDriverVersion()
             << "\n\n\tDatabase in Read-Only Mode?: " << dbcon_meta -> isReadOnly()
             << "\n\tSupports Transactions?: " << dbcon_meta -> supportsTransactions()
             << "\n\tSupports DML Transactions only?: " << dbcon_meta -> supportsDataManipulationTransactionsOnly()
             << "\n\tSupports Batch Updates?: " << dbcon_meta -> supportsBatchUpdates()
             << "\n\tSupports Outer Joins?: " << dbcon_meta -> supportsOuterJoins()
             << "\n\tSupports Multiple Transactions?: " << dbcon_meta -> supportsMultipleTransactions()
             << "\n\tSupports Named Parameters?: " << dbcon_meta -> supportsNamedParameters()
             << "\n\tSupports Statement Pooling?: " << dbcon_meta -> supportsStatementPooling()
             << "\n\tSupports Stored Procedures?: " << dbcon_meta -> supportsStoredProcedures()
             << "\n\tSupports Union?: " << dbcon_meta -> supportsUnion()
             << "\n\n\tMaximum Connections: " << dbcon_meta -> getMaxConnections()
             << "\n\tMaximum Columns per Table: " << dbcon_meta -> getMaxColumnsInTable()
             << "\n\tMaximum Columns per Index: " << dbcon_meta -> getMaxColumnsInIndex()
             << "\n\tMaximum Row Size per Table: " << dbcon_meta -> getMaxRowSize() << " bytes\n";

   std::cout << "\n\tDatabase schemas: ";

  //auto_ptr<sql::ResultSet> rs(dbcon_meta -> getSchemas());
  sql::ResultSet *rs(dbcon_meta -> getSchemas());

  std::cout << "\n\tTotal number of schemas: " << rs -> rowsCount() << std::endl;
  std::cout << std::endl;

  int row = 1;

  while(rs -> next())
  {
   std::cout << "\t" << row << ". " << rs -> getString("TABLE_SCHEM");
   ++row;
  } // while

  std::cout <<"\n\n";

} // retrieve_dbmetadata_and_print()

static void retrieve_rsmetadata_and_print(sql::ResultSet *rs)
{
 if(rs -> rowsCount() == 0)
 {
  throw runtime_error("ResultSetMetaData FAILURE - no records in the result set");
 }

 std::cout << "\n\tResultSet Metadata\n\t------------------";

 /* The following commented statement won't work with Connector/C++ 1.0.5 and later */
 //auto_ptr < ResultSetMetaData > res_meta ( rs -> getMetaData() );

 sql::ResultSetMetaData *res_meta = rs -> getMetaData();

 int numcols = res_meta -> getColumnCount();
 std::cout << "\nNumber of columns in the result set: " << numcols <<"\n\n";

 std::cout.width(20);
 std::cout << "Column Name/Label";
 std::cout.width(20);
 std::cout << "Column Type";
 std::cout.width(20);
 std::cout << "Column Size\n";

 for(int i = 0; i < numcols; ++i)
  {
   std::cout.width(20);
   std::cout << res_meta -> getColumnLabel (i+1);
   std::cout.width(20); 
   std::cout << res_meta -> getColumnTypeName (i+1);
   std::cout.width(20); 
   std::cout << res_meta -> getColumnDisplaySize (i+1) <<"\n\n";
  }

 std::cout << "\nColumn \"" << res_meta -> getColumnLabel(1)
           << "\" belongs to the Table: \"" << res_meta -> getTableName(1)
           << "\" which belongs to the Schema: \"" << res_meta -> getSchemaName(1) << "\"\n\n";

} // retrieve_rsmetadata_and_print()


int main(int argc, const char *argv[])
{
 sql::Driver *driver;
 sql::Connection *con;
 sql::Statement *stmt;
 sql::ResultSet *res;
 sql::PreparedStatement *prep_stmt;
 sql::Savepoint *savept;

 int updatecount = 0;

 /* initiate url, user, password and database variables */
 std::string url(argc >= 2 ? argv[1] : DBHOST);
 const std::string user(argc >= 3 ? argv[2] : USER);
 const std::string password(argc >= 4 ? argv[3] : PASSWORD);
 const std::string database(argc >= 5 ? argv[4] : DATABASE);

 try{
     driver = get_driver_instance();
     /* create a database connection using the Driver */
     con = driver -> connect(url, user, password);
     
     /* alternate syntax using auto_ptr to create the db connection */
     //auto_ptr  con (driver -> connect(url, user, password));
     
     /* turn off the autocommit */
     con -> setAutoCommit(0);
     
     std::cout << "\nDatabase connection\'s autocommit mode: " << con -> getAutoCommit() << std::endl;
     
     /* select appropriate database schema */
     con -> setSchema(database);
     
     /* retrieve and display the database metadata */
     retrieve_dbmetadata_and_print (con);
     
     /* create a statement object */
     stmt = con -> createStatement();
     
     std::cout << "Executing the Query: \"SELECT * FROM cadastros\" ..\n";
     
     /* run a query which returns exactly one result set */
     res = stmt -> executeQuery ("SELECT * FROM cadastros");
     
     std::cout << "Retrieving the result set ..\n";

     /* retrieve the data from the result set and display on stdout */
     retrieve_data_and_print(res, NUMOFFSET, 1, std::string("nome_faz"));

     /* retrieve and display the result set metadata */
     retrieve_rsmetadata_and_print (res);

     std::cout << "Demonstrating Prepared Statements ..\n\n";

     /* insert couple of rows of data into City table using Prepared Statements */
     prep_stmt = con -> prepareStatement ("INSERT INTO cadastros (nome_faz) VALUES (?)");
     
     std::cout << "\tInserting \"Agua Branca\" into the table, cadastros ..\n";

     prep_stmt -> setString (1, "Agua branca");
     updatecount = prep_stmt -> executeUpdate();

     std::cout << "\tCreating a save point \"SAVEPT1\" ..\n";
     savept = con -> setSavepoint ("SAVEPT1");
     
     std::cout << "\tInserting \"Boi Preto\" into the table, cadastros ..\n";

     prep_stmt -> setString (1, "Boi Preto");
     updatecount = prep_stmt -> executeUpdate();

     std::cout << "\tRolling back until the last save point \"SAVEPT1\" ..\n";
     con -> rollback (savept);
     con -> releaseSavepoint (savept);

     std::cout << "\tCommitting outstanding updates to the database ..\n";
     con -> commit();

     std::cout << "\nQuerying the City table again ..\n";

     /* re-use result set object */
     res = NULL;
     res = stmt -> executeQuery ("SELECT * FROM cadastros");

     /* retrieve the data from the result set and display on stdout */
     retrieve_data_and_print (res, COLNAME, 1, std::string ("CityName"));
     
     std::cout << "Cleaning up the resources ..\n";

     /* Clean up */
     delete res;
     delete stmt;
     delete prep_stmt;
     con -> close();
     delete con;
    }
    catch(sql::SQLException &e)
    {
     std::cerr << "ERROR: SQLException in " << __FILE__
               << " (" << __func__<< ") on line " << __LINE__
               << "ERROR: " << e.what()
               << " (MySQL error code: " << e.getErrorCode()
               << ", SQLState: " << e.getSQLState() << ")\n";
            
     if(e.getErrorCode() == 1047)
      {
       /*
        Error: 1047 SQLSTATE: 08S01 (ER_UNKNOWN_COM_ERROR)
        Message: Unknown command
       */
       std::cout << "\nYour server does not seem to support Prepared Statements at all.\nPerhaps MYSQL < 4.1?\n";
      }
     return EXIT_FAILURE;
    }
    catch(std::runtime_error &e)
    {
     std::cerr << "ERROR: runtime_error in " << __FILE__
               << " (" << __func__ << ") on line " << __LINE__
               << "ERROR: " << e.what()
               << "\n";
     return EXIT_FAILURE;
    }
  return EXIT_SUCCESS; 
}