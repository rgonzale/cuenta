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

#define BUFSIZE 64

char query[BUFSIZE];

/* 
   Categories

   I/i = Income
   B/b = Bill(routine)
   E/e = Expense(car maintenance, medical, etc)
   F/f = Food/Grocery
   M/m = Misc

*/

void die() {
	fprintf(stderr, "Usage acct <category> <amount> - make query\nUsage acct S - see summary\n");
	exit(1);
}

char * parse(int *argc, char **argv) {

	int i;
	char s[1];

	if (((*argc == 2) || (*argc == 3)) && ((*argv[1] == 'S') || (*argv[1] == 's'))) {

		if ((argv[2] != NULL) && (strchr("ibefmIBEFM", (*argv[2])) == NULL))
			print_summary(NULL);
		else 
			print_summary(argv[2]);
	}

	else if ((*argc) == 4) {

		if (strchr("ibefmIBEFM", (*argv[1])) == NULL) 
			die();

		if ((strncpy(s, argv[2], 1)) == NULL)
			die();

		for (i = 0; i < strlen(argv[2]); i++)
		{
			if (isdigit(s[i]) == 0)
				die();
		}

		snprintf(query, 64, "insert into acct values(CURDATE(), \"%s\", %d, \"%s\")"
				, argv[1], atoi(argv[2]), argv[3]);

		return query;
	}
	return NULL;
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

	mysql_start();

	if (category == NULL) {

		mysql_select(con, "select day, category, format(amount/100,2), description from acct");

		printf("Balance: ");

		mysql_select(con, "select format((select sum(amount/100) from acct where category = 'I')"
				"- (select sum(amount/100) from acct where category REGEXP '[BEFM]'),2);");

		mysql_stop();
	}
	else {

		snprintf(query, 128, "select day, category, format(amount/100,2), description from acct where"
				" category = '%s'", category);

		mysql_select(con, query);

	}
	return 0;
}
