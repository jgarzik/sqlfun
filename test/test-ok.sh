#!/bin/sh

export PATH=.:..:$PATH

for testfn in ok*.sql
do
	cat $testfn | sql
	if [ $? -ne 0 ]
	then
		echo "Failed on $testfn"
		exit 1
	fi
done

