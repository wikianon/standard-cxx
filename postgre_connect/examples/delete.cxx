#include <iostream>
#include <pqxx/pqxx> 
//DELETE Operation

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
      
     /* Create a transactional object. */
     pqxx::work tr_obj(login);
     
     /* Create  SQL DELETE statement */
     sql = "DELETE from COMPANY where ID = 2";
     
     /* Execute SQL query */
     tr_obj.exec(sql);
     tr_obj.commit();
     std::cout << "\n\tRecords deleted successfully\n";
      
     /* Create SQL SELECT statement */
     sql = "SELECT * from COMPANY";
     
     /* Create a non-transactional object. */
     pqxx::nontransaction non_obj(login);
      
     /* Execute SQL query */
     pqxx::result res(non_obj.exec(sql));
      
     /* List down all the records */
     for(pqxx::result::const_iterator c = res.begin(); c != res.end(); ++c)
      {
       std::cout << "\n\tID......: " << c[0].as<int>()
                 << "\n\tName....: " << c[1].as<std::string>()
                 << "\n\tAge.....: " << c[2].as<int>()
                 << "\n\tAddress.: " << c[3].as<std::string>()
                 << "\n\tSalary..: " << c[4].as<float>()
                 << "\n\n";
      }
      std::cout << "\n\tOperation done successfully\n";
      login.disconnect();
    }
    catch(const std::exception &e)
    {
     std::cerr << e.what() << std::endl;
     return 1;
    }
 return 0;
}