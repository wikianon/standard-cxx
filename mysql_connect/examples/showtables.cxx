#include <mysql.h>
#include <iostream>
//https://mandeepsimak.wordpress.com/tag/mysql-c-example/
// Defining Constant Variables

#define HOSTS    "localhost"
#define USERS    "root"
#define PASSWORD ""
#define DATABASE "teste"

int main()
{
    MYSQL *connect;
    connect = mysql_init(NULL);
    
    if(!connect)std::cout << "Mysql Initialization Failed";
    
    connect = mysql_real_connect(connect, HOSTS, USERS, PASSWORD, DATABASE, 0,NULL,0);
    
    if(connect)
     std::cout << "\n\tConnection Succeeded\n";
      else
     std::cout << "\n\tConnection Failed\n";
    
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    
    // Replace MySQL query with your query
    
    mysql_query(connect,"show tables");
    
    unsigned int i=0;
    
    res_set=mysql_store_result(connect);
    
    unsigned int numrows = mysql_num_rows(res_set);
    
    std::cout << "\n\tTable: ";

    while((row = mysql_fetch_row(res_set)) != NULL)std::cout << row[i];
   
    std::cout<<" in database: "<< DATABASE <<"\n\n";
    
    mysql_close(connect);

    return 0;
}