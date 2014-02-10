#include <string.h>
#include <ctype.h>
#include "mysql.c"
#include "functions.c"

int main(int argc, char **argv) {

	char *queryp;
	int bool_balance;

	sanitize_input(&argc, argv);

	if (argv[1][0] == 's') {
		print_summary(argv[2]);
		exit(0);
	}

	mysql_start();

	bool_balance = check_balance(&argc, argv);

	queryp = forge_query(&argc, argv);

	mysql_insert(queryp);

	if (bool_balance)
		calculate_balance(&argc, argv);

	mysql_stop();

	return 0;
}
