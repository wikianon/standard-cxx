#include <sstream>
#include <iostream>
#include <mysql/mysql.h>

#define USER "root"
#define DATABASE "teste"
#define DBHOST "localhost"
#define PASSWORD ""

int main(void)
{
 MYSQL conn; //variavel de conexao
 MYSQL_RES *res_set; //variável que recebe o resultado
 MYSQL_ROW row; //variável que recebe os dados
 mysql_init(&conn);  // Inicia a variável conn
 
  if(!mysql_real_connect(&conn,DBHOST,USER,PASSWORD,DATABASE,0,NULL,0))
   {
    std::cerr<<"\n\tFailed connect to database: "<<DATABASE
             <<"\n\t"<<mysql_error(&conexao)<<"\n\n";
   }
   else
   { 
    if(mysql_query(&conn,"select * from person"))
    //if(mysql_query(&conn,"select * from cadastros"))  
    {
     std::cerr<<"\n\t\tErro: "<<mysql_errno(&conn)<<" : "<<mysql_error(&conn)<<"\n";
     std::cin.get();
     std::cin.get();
     return 1;
    }
    
    // Recebe os dados da cunsulta
    res_set = mysql_store_result(&conn);
    
    // Se consultou (sem erros)
    if(!res_set)
     {
      std::cerr<<"\n\t\tErro: "<<mysql_errno(&conn)<<" : "<<mysql_error(&conn)<<"\n";
      std::cin.get();
      std::cin.get();
      return 1; 
     } 
     else
     {
       std::cerr<<"\n\t\tSuccessfully connected to Database: "<<DATABASE<<".\n\n";
       int ID;
       ID = mysql_num_rows(res_set);
       std::cout<<"\n\t\tNum of rows: "<<ID<<"\n";
       
       // Enquanto receber dados vai escrevendo
       while((row=mysql_fetch_row(res_set)) != NULL)
        {
         // Escreve os dados formatados sende estes a ID e o NOME

         std::istringstream buffer(row[0]); // Cria a variável que recebera a string a ser convertida para int
         buffer >> ID; // Convert a string para int
         std::cout<<"\n\t\t"<<ID<<"\t"<<row[1]<<"\t"<<row[2]; // Escreve formatado
       }
     }
    // Limpa da memória
    mysql_free_result(res_set);
   }
   
  mysql_close(&conn);
  std::cout<<"\n\n\n";
  return 0;
}