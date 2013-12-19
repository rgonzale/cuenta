#include <string.h>
#include <ctype.h>
#include "mysql.c"
#include "functions.c"

int main(int argc, char **argv) {

	char *queryp;
	long long amt;

	mysql_start();

	sanitize(&argc, argv);

	queryp = forge_query(&argc, argv);

	mysql_insert(queryp);

	balance(amt);

	mysql_stop();

	return 0;
}
