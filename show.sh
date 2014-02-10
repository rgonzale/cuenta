#!/bin/sh
# runs a select on the tables

mysql -e 'select * from acct'
mysql -e 'select * from balance'
