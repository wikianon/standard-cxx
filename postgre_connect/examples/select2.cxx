#include <iostream>
#include <pqxx/pqxx>

/// Query employees from database.  Return result.
pqxx::result query()
{
  pqxx::connection login("dbname = teste user = darkstar password = darkstar hostaddr = 127.0.0.1 port = 5432");
  pqxx::work txn{login};
  
  pqxx::result r = txn.exec("select person_id, name, age, status, date_person from person");
  
  for (auto row: r)
  std::cout <<"\n"
            << row[0].as<int>()      <<", "
            << row["name"].c_str()   <<", "
            << row[2].as<int>()      <<", "
            << row["status"].c_str() <<", "
            << row["date_person"].c_str();
       
  std::cout << "\n\n";   
  // Not really needed, since we made no changes, but good habit to be
  // explicit about when the transaction is done.
  txn.commit();

  // Connection object goes out of scope here.  It closes automatically.
  return r;
}

/// Query employees from database, print results.
int main(int, char *argv[])
{
  try
  {
    pqxx::result r = query();

    // Results can be accessed and iterated again.  Even after the connection
    // has been closed.
    for (auto row: r)
    {
      std::cout << "Row: ";
      // Iterate over fields in a row.
      for (auto field: row) std::cout << field.c_str() << ", ";
      std::cout<<std::endl;
    }
   std::cout << "\n\n"; 
  }
  catch (const pqxx::sql_error &e)
  {
    std::cerr << "SQL error: " << e.what()
              << "\nQuery was: " << e.query() << "\n";
    return 2;
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
}