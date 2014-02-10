//
//  functions.c
//
//  Tristan Gonzalez, acct, Accountant software that keeps track of your monthly costs
//  Copyright (c) 2013 Tristan Gonzalez. All rights reserved.
//  rgonzale@darkterminal.net
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <strings.h>
#include <time.h>
#include "prototypes.h"

#define BUFSIZE 128

char query[BUFSIZE];

/* 
   Categories

   i = Income
   b = Bill(routine)
   e = Expense(car maintenance, medical, etc)
   f = Food/Grocery
   m = Misc

*/

void die(char *error) 
{
	if (error)
		fprintf(stderr, "Error: %s\n", error);

	fprintf(stderr, "Usage acct <category> <amount> - make query\nUsage acct S - see summary\n");
	exit(1);
}

int sanitize_input(int *argc, char **argv)
{
	int i;
	char comment[BUFSIZE];
	bzero(comment, BUFSIZE);

	if (*argc < 2 || *argc > 4)
		die("Arguments are too few or too many");

	if (*argv[1] == 's' || *argv[1] == 'S') {
		print_summary(NULL);
		exit(0);
	}

	if (argv[2] != NULL) {
		if (strchr("ibefm", (*argv[1])) != NULL) {
			for (i = 0; i < strlen(argv[2]); i++) {
				if (isdigit(argv[2][i]) == 0)
					die("amount contains non-numeric characters");
				else if ((strncpy(comment, argv[3], BUFSIZE)) == NULL)
					die("failed writing to comment");
			}
		}
		else
			die("char category not matched");

	}
	else 
		die("amount and comment missing");	
}

char * forge_query(int *argc, char **argv)
{
	my_ulonglong last;

	last = mysql_insert_id(con);

	snprintf(query, BUFSIZE, "insert into acct values(NULL, CURDATE(), \"%s\", %d, \"%s\")",
			argv[1], atoi(argv[2]), argv[3]);

	return query;
}

int get_month()
{
	time_t now;
	struct tm tm;

	time(&now);
	tm = *localtime(&now);

	printf("It is now %s", ctime(&now));
	printf("Month is %d\n", tm.tm_mon + 1);

	return tm.tm_mon + 1;
}

int print_summary(char *category)
{
	printf("Printing Summary\n");

	if (category == NULL) {
		mysql_select("select id, day, upper(category), format(amount/100,2), description from acct");
		printf("Balance: $");
		mysql_select("select format(amount/100,2) from balance");
	}

	else {
		snprintf(query, BUFSIZE, "select id, day, upper(category), format(amount/100,2), description from acct where"
				" category = '%s'", category);
		mysql_select(query);
	}

	return 0;
}

int check_balance(int *argc, char **argv)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	int num_fields;
	long long amount;

	if (mysql_query(con, "select amount from balance"))
		finish_with_error(con);
	result = mysql_store_result(con);
	num_fields = mysql_num_fields(result);
	row = mysql_fetch_row(result);

	if (row == 0) {
		if (strchr("ibefm", (*argv[1])) != NULL) {
			amount = atoi(argv[2]);
			snprintf(query, BUFSIZE, "insert into balance values (CURDATE(), %lld)", amount);
			mysql_insert(query);
		}
	}
}

int calculate_balance(int *argc, char **argv) 
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	my_ulonglong num_rows;
	unsigned long long balance_amount = 0, amount = 0;
	char * date;
	char query[BUFSIZE];

	date = mysql_date();

	snprintf(query, BUFSIZE, "select amount from balance where day='%s'", date);

	if (mysql_query(con, query))
		finish_with_error(con);

	if ((result = mysql_store_result(con)) == 0)
		finish_with_error(con);

	if ((num_rows = mysql_num_rows(result)) == 0)
		finish_with_error(con);

	mysql_data_seek(result, num_rows - 1);

	if ((row = mysql_fetch_row(result)) == NULL) {
		fprintf(stderr, "error from fetch\n");
		finish_with_error(con);
	}

	balance_amount = atoll(row[0]);

	mysql_free_result(result);

	switch(*argv[1])
	{
		case 'i':
			amount = atoll(argv[2]);
			snprintf(query, BUFSIZE, "insert into balance values (CURDATE(), %lld)", balance_amount + amount);
			mysql_insert(query);
			break;
		case 'b':
			amount = atoll(argv[2]);
			snprintf(query, BUFSIZE, "insert into balance values (CURDATE(), %lld)", balance_amount - amount);
			mysql_insert(query);
			break;
		case 'e':
			amount = atoll(argv[2]);
			snprintf(query, BUFSIZE, "insert into balance values (CURDATE(), %lld)", balance_amount - amount);
			mysql_insert(query);
			break;
		case 'f':
			amount = atoll(argv[2]);
			snprintf(query, BUFSIZE, "insert into balance values (CURDATE(), %lld)", balance_amount - amount);
			mysql_insert(query);
			break;
		case 'm':
			amount = atoll(argv[2]);
			snprintf(query, BUFSIZE, "insert into balance values (CURDATE(), %lld)", balance_amount - amount);
			mysql_insert(query);
			break;
		default:
			break;
	}
}
