ACLOCAL_AMFLAGS = -I build-aux/m4

AM_CFLAGS = -DYYDEBUG=1

bin_PROGRAMS = sql

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libsqlpars.pc

# TESTS = test/unitester

# noinst_PROGRAMS = $(TESTS)
EXTRA_DIST = sql.y sql.l \
	pscan.js \
	test/test-ok.sh test/test-fail.sh \
	test/fail1.sql \
	test/fail3.sql \
	test/fail4.sql \
	test/fail5.sql \
	test/xfail2.sql \
	test/ok1.sql \
	test/ok2.sql \
	test/ok3.sql \
	test/ok4.sql \
	test/ok5.sql \
	test/ok6.sql \
	test/ok7.sql \
	test/ok8.sql \
	test/ok9.sql \
	test/ok10.sql

sql_SOURCES = source/exec.c source/lib.c source/main.c source/sql-parser.h source/yyl.h
sql_LDADD = @JANSSON_LIBS@
nodist_sql_SOURCES = source/sql.c source/sql.tab.c source/sql.tab.h source/sql.lex.h

BUILT_SOURCES = $(nodist_sql_SOURCES)
CLEANFILES = $(nodist_sql_SOURCES) sql.output

source/sql.tab.c source/sql.tab.h:   source/sql.y Makefile
	${BISON} -o $@ -vd $<

source/sql.c source/sql.lex.h: source/sql.l Makefile
	${LEX} -o $@ $<

TESTS = test-ok.sh test-fail.sh

