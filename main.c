#include <string.h>
#include <ctype.h>
#include "mysql.c"
#include "functions.c"

int main(int argc, char **argv) {

	char *queryp;

	sanitize_input(&argc, argv);

	mysql_start();

	mysql_date();
/*
	check_balance(&argc, argv);

	queryp = forge_query(&argc, argv);

	mysql_insert(queryp);

	calculate_balance(&argc, argv);
*/
	mysql_stop();

	return 0;
}
