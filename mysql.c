//
//  mysql.c
//
//  Tristan Gonzalez, acct, Accountant software that keeps track of your monthly costs
//  Copyright (c) 2013 Tristan Gonzalez. All rights reserved.
//  user@darkterminal.net
//

#include <mysql/my_global.h>
#include <mysql/mysql.h>

MYSQL *con;
MYSQL_RES *result;
MYSQL_ROW row;
int i, num_fields;

int finish_with_error()
{
   fprintf(stderr, "%s\n", mysql_error(con));
   mysql_close(con);
   mysql_library_end();
   exit(1);        
}

/*
void get_mysql_version() {
   fprintf(stderr, "MySQL client version: %s\n", mysql_get_client_info());
}
*/

int mysql_start() 
{
	if (mysql_library_init(0, NULL, NULL)) {
		fprintf(stderr, "could not initialize MySQL library\n");
		exit(1);
	}

	if ((con = mysql_init(NULL)) == NULL)
		finish_with_error(con);

	if (mysql_real_connect(con, "localhost", "user", "password", 
				"acct", 0, NULL, 0) == NULL) 
		finish_with_error(con);

	return 0;
}

int mysql_insert(const char *query)
{
	if (mysql_query(con, query))
		finish_with_error(con);

	return 0;
}

int mysql_select(const char *query)
{
	if (mysql_query(con, query))
		finish_with_error(con);

	result = mysql_store_result(con);

	num_fields = mysql_num_fields(result);

	while ((row = mysql_fetch_row(result))) 
	{ 
		for(i = 0; i < num_fields; i++) 
		{ 
			printf("%s ", row[i]); 
		} 
		printf("\n"); 
	}

	mysql_free_result(result);

	return 0;
}

char * mysql_date()
{
	if (mysql_query(con, "select CURDATE()"))
		finish_with_error(con);

	result = mysql_store_result(con);
	num_fields = mysql_num_fields(result);
	row = mysql_fetch_row(result);
	mysql_free_result(result);

	return row[0];
}

int mysql_stop() 
{
	mysql_close(con);
	mysql_library_end();

	return 0;
}
