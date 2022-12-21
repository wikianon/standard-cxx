#include <sstream>
#include <iostream>
#include <mysql/mysql.h>

#define USER "root"
#define DATABASE "teste"
#define DBHOST "localhost"
#define PASSWORD ""

int main()
{
 MYSQL conn; //variavel de conexao
 MYSQL_RES *res_set; //variável que recebe o resultado
 MYSQL_ROW row; //variável que recebe os dados
 mysql_init(&conn);  // Inicia a variável conn
 
   if(!mysql_real_connect(&conn,DBHOST,USER,PASSWORD,DATABASE,0,NULL,0))
   {
    std::cerr<<"\n\tFailed connect to database: "<<DATABASE
             <<"\n\t"<<mysql_error(&conn)<<"\n\n";
   }
   else
   {
    std::cerr<<"\n\t\tSuccessfully connected to Database: "<<DATABASE<<".\n\n";
    
    int status = mysql_query(&conn,"select *from cadastros");
    
    res_set = mysql_store_result(&conn); 
    
    status = mysql_num_rows(res_set);
    
    std::cout<<"\n\t\tNum of rows: "<<status<<" row(s).\n\n";
    
      while((row = mysql_fetch_row(res_set)) != NULL) 
       {
        std::cout<<"\t"; 
        for(int i=0; i<mysql_num_fields(res_set); i++) 
         { 
          std::cout<<"\t"<<(row[i] != NULL ? row[i] : "NULL"); 
         }
         std::cout<<"\n";
       }
    // Limpa da memória
    mysql_free_result(res_set);   
   }
 
  mysql_close(&conn);
  std::cout<<"\n\n";
  return 0;
}