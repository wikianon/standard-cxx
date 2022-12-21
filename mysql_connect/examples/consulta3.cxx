#include <iostream>
#include <mysql/mysql.h>

#define USER "root"
#define DATABASE "teste"
#define DBHOST "localhost"
#define PASSWORD ""

int main(void)
{
 MYSQL conexao;
 MYSQL_RES *resp;
 MYSQL_ROW linhas;
 MYSQL_FIELD *campos;
 
 char query[]="select *from person;";

   mysql_init(&conexao);
   if(!mysql_real_connect(&conexao,DBHOST,USER,PASSWORD,DATABASE,0,NULL,0))
   {
    std::cerr<<"\n\tFailed connect to database: "<<DATABASE
             <<"\n\t"<<mysql_error(&conexao)<<"\n\n";
   }
   else
   {
    if(mysql_query(&conexao,query))
     { 
      std::cerr<<"\n\t\tErro: "<<mysql_errno(&conexao)<<" : "<<mysql_error(&conexao)<<"\n";
      std::cin.get();
      std::cin.get();
      return 1;
     }
     else
     {
      std::cerr<<"\n\t\tSuccessfully connected to Database: "<<DATABASE<<".\n\n";
        
        resp = mysql_store_result(&conexao);//recebe a consulta
        
       // Se consultou (sem erros)
     if(!resp)
      {
       std::cerr<<"\n\t\tErro: "<<mysql_errno(&conexao)<<" : "<<mysql_error(&conexao)<<"\n";
       std::cin.get();
       std::cin.get();
       return 1; 
      }
      else
      {
       //passa os dados dos campos para a variável campos
       //escreve na tela os nomes dos campos dando
       //um tab somente
       campos = mysql_fetch_fields(resp);
       
       std::cout<<"\t";
       
       for(int i = 0; i<mysql_num_fields(resp); i++)
        {
         std::cout<<campos[i].name;
          if(mysql_num_fields(resp)>1)
           std::cout<<"\t";
        }
        
       std::cout<<"\n";   
        //enquanto retonrnar registros, conta até o
        //número de colunas que a tabela tem e escreve na
        //tela com um tab, depois pula a linha e tenta
        //pegar outro registro
       while((linhas=mysql_fetch_row(resp)) != NULL)
        {
         for(int i = 0; i<mysql_num_fields(resp); i++)
          std::cout<<"\t"<<linhas[i];
           std::cout<<"\n";
        }
      }
     mysql_free_result(resp);//limpa a variável do resultado: resp
    }
    mysql_close(&conexao);
   }
  std::cout<<"\n\n"; 
 return 0;
}