CFLAGS=-Wall -g3 -Wno-return-type

all: main

main: 
	cc $(CFLAGS) -o cuentame main.c -I/usr/include/mysql `mysql_config --libs` 

clean:
	rm -rf cuentame
