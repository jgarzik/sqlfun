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

OBJS	= sql.tab.o sql.o exec.o

sql:	${OBJS} sql-parser.h
	${CC} -o $@ ${OBJS}

sql.tab.c sql.tab.h:	sql.y
	${YACC} -vd sql.y

sql.c:	sql.l
	${LEX} -o $@ $<

sql.o:	sql.c sql.tab.h

clean:
	rm -f sql sql.tab.c sql.tab.h sql.c ${OBJS} \
	sql.output

.SUFFIXES:	.l .y .c

