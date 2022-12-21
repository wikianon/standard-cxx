#include <iostream>
#include <mysql/mysql.h>
//https://www.vivaolinux.com.br/artigo/Usando-MySQL-na-linguagem-C/?pagina=3
#define USER "root"
#define DATABASE "teste"
#define DBHOST "localhost"
#define PASSWORD ""














int main()
{
 MYSQL conexao;
 mysql_init(&conexao);
 
 if(!mysql_real_connect(&conexao,DBHOST,USER,PASSWORD,DATABASE,0,NULL,0))
  {
    fprintf(stderr, "\n\t\tFailed to connect to database: %s\n",mysql_error(&conexao));
  }
 else
  {
   fprintf(stderr, "\n\t\tSuccessfully connected to Database %s.\n",DATABASE);
      
   int res = mysql_query(&conexao,"insert into person(person_id,name,age,status,id_faz) values('07', 'Marcos Silva', 42, 'Administrador', '02');");

   if(!res)std::cout<<"\n\tRegistros inseridos: "<<mysql_affected_rows(&conexao)<<"\n";
    else
   std::cout<<"\n\tConnection Failed\n\tErro: "<<mysql_errno(&conexao)<<" : "<<mysql_error(&conexao)<<"\n\n";

   mysql_close(&conexao);
  }
 return 0;
}