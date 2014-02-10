#!/bin/sh
# clears tables

mysql -e 'truncate table acct'
mysql -e 'truncate table balance'
