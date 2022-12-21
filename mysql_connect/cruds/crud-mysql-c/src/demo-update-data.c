#include <my_global.h>
#include <mysql.h>
#include <string.h>

#define UPDATE_DATA "update product set name = ?, price=? where idproduct=?"

int main(int argc, char **argv)
{

	MYSQL *con = mysql_init(NULL);

	if (con == NULL)
	{
	  fprintf(stderr, "%s\n", mysql_error(con));
	  exit(1);
	}
	printf("connecting to mysql server...\r\n");
	// change host, username, and password
	if (mysql_real_connect(con, "localhost", "root", "password",
		  "mydatabase", 0, NULL, 0) == NULL)
	{
		printf("error: %s\r\n", mysql_error(con));
		mysql_close(con);
		exit(1);
	}
	printf("connected.\r\n");
	printf("inserting 10 data...\r\n");

	MYSQL_STMT  *stmt;

	stmt = mysql_stmt_init(con);
	if (!stmt)
	{
		printf(" mysql_stmt_init(), out of memory\r\n");
		exit(0);
	}

	if (mysql_stmt_prepare(stmt, UPDATE_DATA, strlen(UPDATE_DATA)))
	{
		printf("mysql_stmt_prepare(), UPDATE failed\r\n");
		printf("error: %s\r\n", mysql_stmt_error(stmt));
		exit(0);
	}

	int i;
	char name[10];
	unsigned long str_length;
	float price;
	MYSQL_BIND  bind[3];
	int product_id;

	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type= MYSQL_TYPE_VAR_STRING;
	bind[0].buffer= (char *)&name;
	bind[0].is_null= 0;
	bind[0].length= &str_length;

	bind[1].buffer_type= MYSQL_TYPE_FLOAT;
	bind[1].buffer= (char *)&price;
	bind[1].is_null= 0;
	bind[1].length= 0;

	bind[2].buffer_type= MYSQL_TYPE_LONG;
	bind[2].buffer= (char *)&product_id;
	bind[2].is_null= 0;
	bind[2].length= 0;

	// bind parameters
	if (mysql_stmt_bind_param(stmt, bind))
	{
		printf("mysql_stmt_bind_param() failed\r\n");
		printf("%s\r\n", mysql_stmt_error(stmt));
		exit(0);
	}

	// set updated values
	// change these values!!
	sprintf(name,"product-updated");
	str_length = strlen(name);
	price = 10.33;
	product_id = 25;

	printf("updating data...\r\n");
	if (mysql_stmt_execute(stmt))
	{
		printf("mysql_stmt_execute(), 1 failed\r\n");
		printf("%s\r\n", mysql_stmt_error(stmt));
		exit(0);
	}
	printf("done.\r\n");
	printf("closing connection...\r\n");

	/* Close the statement */
	if (mysql_stmt_close(stmt))
	{
		printf("failed while closing the statement\r\n");
		printf("%s\r\n", mysql_stmt_error(stmt));
		exit(0);
	}
	mysql_close(con);
	printf("closed.\r\n");

	exit(0);
}
