#! /bin/sh -x

mysqldump --no-create-info --complete-insert -u LampServer -p LampServer > data.sql

