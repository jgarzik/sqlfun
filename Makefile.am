ACLOCAL_AMFLAGS = -I build-aux/m4

AM_CFLAGS = -DYYDEBUG=1

bin_PROGRAMS = sql

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libsqlpars.pc

# TESTS = test/unitester

# noinst_PROGRAMS = $(TESTS)
EXTRA_DIST = sql.y sql.l \
	pscan.js \
	test-ok.sh test-tst.sh test-fail.sh \
	tests/fail1.sql \
	tests/fail3.sql \
	tests/fail4.sql \
	tests/fail5.sql \
	tests/xfail2.sql \
	tests/ok1.sql \
	tests/ok2.sql \
	tests/ok3.sql \
	tests/ok4.sql \
	tests/ok5.sql \
	tests/ok6.sql \
	tests/ok7.sql \
	tests/ok8.sql \
	tests/ok9.sql \
	tests/ok10.sql \
	tests/tst-create-db.sql \
	tests/tst-create-table.sql \
	tests/tst-delete.sql \
	tests/tst-drop-db.sql \
	tests/tst-drop-table.sql \
	tests/tst-insert.sql \
	tests/tst-select-null.sql \
	tests/tst-select-null2.sql \
	tests/tst-select-null3.sql

sql_SOURCES = exec.c lib.c main.c sql-parser.h yyl.h
sql_LDADD = @JANSSON_LIBS@
nodist_sql_SOURCES = sql.c sql.tab.c sql.tab.h sql.lex.h

BUILT_SOURCES = $(nodist_sql_SOURCES)
CLEANFILES = $(nodist_sql_SOURCES) sql.output

sql.tab.c sql.tab.h:   sql.y Makefile
	${BISON} -vd $<

sql.c sql.lex.h: sql.l Makefile
	${LEX} -o $@ $<

TESTS = test-ok.sh test-fail.sh test-tst.sh

