# $Header: /home/johnl/flnb/code/sql/RCS/Makefile,v 2.1 2009/11/08 02:53:39 johnl Exp $
# Companion source code for "flex & bison", published by O'Reilly
# Media, ISBN 978-0-596-15597-1
# Copyright (c) 2009, Taughannock Networks. All rights reserved.
# See the README file for license conditions and contact info.

CC = cc -g
LEX = flex
YACC = bison
CFLAGS = -DYYDEBUG=1

PROGRAMS = sql

all:	${PROGRAMS}

# chapter 4

sql:	sql.tab.o sql.o
	${CC} -o $@ sql.tab.o sql.o

sql.tab.c sql.tab.h:	sql.y
	${YACC} -vd sql.y

sql.c:	sql.l
	${LEX} -o $@ $<

sql.o:	sql.c sql.tab.h

clean:
	rm -f sql sql.tab.c sql.tab.h sql.c sql.tab.o sql.o \
	sql.output

.SUFFIXES:	.l .y .c

