CFLAGS=-Wall -g3

all: main

main: 
	cc $(CFLAGS) -o acct main.c -I/usr/include/mysql `mysql_config --libs` 

clean:
	rm -rf acct
