#! /bin/sh -x

cat tables.sql data.sql > tmp.sql

mysql -u LampServer -p LampServer < tmp.sql

