#include <string.h>
#include <ctype.h>
#include "mysql.c"
#include "functions.c"

int main(int argc, char **argv) {

	char *queryp;

	mysql_start();

	sanitize_input(&argc, argv);

	queryp = forge_query(&argc, argv);

	mysql_insert(queryp);

	balance(&argc, argv);

	mysql_stop();

	return 0;
}
