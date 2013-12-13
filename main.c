#include <string.h>
#include <ctype.h>
#include "mysql.c"
#include "functions.c"

int main(int argc, char **argv) {

	char *queryp;

	queryp = parse(&argc, argv);

	if (queryp != NULL)
	{
		mysql_start();

		mysql_insert(con, queryp);

		mysql_stop();
	}

	return 0;
}
