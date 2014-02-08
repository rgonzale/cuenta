#include <string.h>
#include <ctype.h>
#include "mysql.c"
#include "functions.c"

int main(int argc, char **argv) {

	char *queryp;
	char *test;

	sanitize_input(&argc, argv);

	mysql_start();

	test = mysql_date();
	fprintf(stderr, "%s\n", test);
/*
	check_balance(&argc, argv);

	queryp = forge_query(&argc, argv);

	mysql_insert(queryp);

	calculate_balance(&argc, argv);
*/
	mysql_stop();

	return 0;
}
