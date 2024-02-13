#!/bin/sh

TESTS=\
	$srcdir/tst-create-db.sql \
	$srcdir/tst-create-table.sql \
	$srcdir/tst-delete.sql \
	$srcdir/tst-drop-db.sql \
	$srcdir/tst-drop-table.sql \
	$srcdir/tst-insert.sql

for testfn in $TESTS
do
	cat $testfn | ./sql
	if [ $? -ne 0 ]
	then
		echo "Failed on $testfn"
		exit 1
	fi
done

