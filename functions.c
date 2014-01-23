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

void die() {
	fprintf(stderr, "Usage acct <category> <amount> - make query\nUsage acct S - see summary\n");
	exit(1);
}

int sanitize_input(int *argc, char **argv) {

	int i;
	char s[BUFSIZE];

	if (*argc < 2 || *argc > 4)
		die();

	else if (*argv[1] == 's' || *argv[1] == 'S') {
		if (argv[2] != NULL)   {
			if (strchr("ibefm", (*argv[2])) != NULL) {
				print_summary(argv[2]);
				exit(0);
			}
			else
				die();
		}
		else {
			print_summary(NULL);
			exit(0);
		}
	}

	else if ((*argc) == 4) {

		if (strchr("ibefm", (*argv[1])) != NULL) 
		{
			if ((strncpy(s, argv[2], 32)) == NULL)
				die();

			for (i = 0; i < strlen(argv[2]); i++)
			{
				if (isdigit(s[i]) == 0)
					die();
			}

			exit(0);
		}
	}

	else
		exit(0);
}

char * forge_query(int *argc, char **argv) {
	my_ulonglong last;

	last = mysql_insert_id(con);

	snprintf(query, BUFSIZE, "insert into acct values(NULL, CURDATE(), \"%s\", %d, \"%s\")",
			argv[1], atoi(argv[2]), argv[3]);

	return query;

}

int get_month() {
	time_t now;
	struct tm tm;

	time(&now);
	tm = *localtime(&now);

	printf("It is now %s", ctime(&now));
	printf("Month is %d\n", tm.tm_mon + 1);

	return tm.tm_mon + 1;
}

int print_summary(char *category) {

	printf("Printing Summary\n");

	if (category == NULL) {
		mysql_select("select id, day, upper(category), format(amount/100,2), description from acct");
		printf("Balance: ");
		mysql_select("select amount from balance");
	}

	else {
		snprintf(query, BUFSIZE, "select id, day, upper(category), format(amount/100,2), description from acct where"
				" category = '%s'", category);
		mysql_select(query);
	}

	return 0;
}

int balance(int *argc, char **argv) {

	MYSQL_RES *result;
	MYSQL_ROW row;
	int num_fields;
	long long amount;

	mysql_query(con, "select amount from balance");
	result = mysql_store_result(con);
	num_fields = mysql_num_fields(result);
	row = mysql_fetch_row(result);
	printf("%s\n", row[0]);
	mysql_free_result(result);

	printf("%d\n", atoi(row[0]));

	snprintf(query, BUFSIZE, "insert into balance values (%lld)", amount);

}
