//
//  mysql.c
//
//  Tristan Gonzalez, acct, Accountant software that keeps track of your monthly costs
//  Copyright (c) 2013 Tristan Gonzalez. All rights reserved.
//  rgonzale@darkterminal.net
//

#include <mysql/my_global.h>
#include <mysql/mysql.h>

MYSQL *con;
MYSQL_RES *result;
MYSQL_ROW row;
int i, num_fields;

int finish_with_error(MYSQL *con)
{
   fprintf(stderr, "%s\n", mysql_error(con));
   mysql_close(con);
   exit(1);        
}

/*
void get_mysql_version() {
   fprintf(stderr, "MySQL client version: %s\n", mysql_get_client_info());
}
*/

int mysql_start() 
{
	my_ulonglong last;
   if ((con = mysql_init(NULL)) == NULL)
      finish_with_error(con);

   if (mysql_real_connect(con, "localhost", "rgonzale", "hotdog99", 
            "acct", 0, NULL, 0) == NULL) 
      finish_with_error(con);

	  last = mysql_insert_id(con);
	  printf("%llu\n", last);


   return 0;
}

int mysql_insert(MYSQL *con, const char *query)
{
   if (mysql_query(con, query))
      finish_with_error(con);
   
   return 0;
}

int mysql_select(MYSQL *con, const char *query)
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

int mysql_date()
{
   char *buf;
   char query[38];

   if (mysql_query(con, "select day from acct where amount=1002"))
      finish_with_error(con);

   result = mysql_store_result(con);
   num_fields = mysql_num_fields(result);
   row = mysql_fetch_row(result);
   printf("Number of fields: %d\n", num_fields);
   printf("Value: %s\n", row[0]);
   mysql_free_result(result);
   buf = row[0];

   snprintf(query, 38, "select extract(month from %s)", buf); 

   if (mysql_query(con, query))
         finish_with_error(con);

   /*
   if (mysql_query(con, printf("select extract(month from %s)", buf)))
         finish_with_error(con);
   */

   result = mysql_store_result(con);
   num_fields = mysql_num_fields(result);
   row = mysql_fetch_row(result);
   printf("Value: %s\n", row[0]);
   mysql_free_result(result);

   return 0;
}

int mysql_stop() 
{
   mysql_close(con);

   return 0;
}

int mysql_balance()
{

}
void main() {
mysql_start();
}
