m4_define([libsqlpars_major_version], [1])
m4_define([libsqlpars_minor_version], [0])
m4_define([libsqlpars_micro_version], [0])
m4_define([libsqlpars_interface_age], [1])
# If you need a modifier for the version number. 
# Normally empty, but can be used to make "fixup" releases.
m4_define([libsqlpars_extraversion], [])

dnl libtool versioning from libsqlpars
m4_define([libsqlpars_current], [m4_eval(100 * libsqlpars_minor_version + libsqlpars_micro_version - libsqlpars_interface_age)])
m4_define([libsqlpars_binary_age], [m4_eval(100 * libsqlpars_minor_version + libsqlpars_micro_version)])
m4_define([libsqlpars_revision], [libsqlpars_interface_age])
m4_define([libsqlpars_age], [m4_eval(libsqlpars_binary_age - libsqlpars_interface_age)])
m4_define([libsqlpars_version], [libsqlpars_major_version().libsqlpars_minor_version().libsqlpars_micro_version()libsqlpars_extraversion()])


AC_INIT([sqlpars], [0.0.2],
        [http://github.com/jgarzik/sqlpars/])

dnl make the compilation flags quiet unless V=1 is used
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_PREREQ(2.60)
AC_CONFIG_SRCDIR([source/exec.c])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([build-aux/m4])
AC_CONFIG_HEADERS([source/sqlpars-config.h])
AM_INIT_AUTOMAKE([subdir-objects foreign])

LIBSQLPARS_MAJOR_VERSION=libsqlpars_major_version
LIBSQLPARS_MINOR_VERSION=libsqlpars_minor_version
LIBSQLPARS_MICRO_VERSION=libsqlpars_micro_version
LIBSQLPARS_INTERFACE_AGE=libsqlpars_interface_age

# ABI version
# http://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
LIBSQLPARS_CURRENT=libsqlpars_current
LIBSQLPARS_REVISION=libsqlpars_revision
LIBSQLPARS_AGE=libsqlpars_age

AC_SUBST(LIBSQLPARS_CURRENT)
AC_SUBST(LIBSQLPARS_REVISION)
AC_SUBST(LIBSQLPARS_AGE)

LT_INIT
LT_LANG([C])

AC_PROG_LEX
AC_PROG_YACC
AC_CHECK_PROG(BISON, bison, bison, false)

dnl Coverage
AC_MSG_CHECKING([whether to code coverage])
AC_ARG_ENABLE([coverage],
	[AS_HELP_STRING([--enable-coverage],[generate code coverage instrumentation])],
	[],
	[enable_coverage=no])
AC_MSG_RESULT([$enable_coverage])

AS_IF([test "$enable_coverage" = "yes"], [
	AC_PATH_PROG(GCOV, gcov, no)
	AS_IF([test "$GCOV" = "no"], [
		AC_MSG_ERROR(gcov tool is not available)])
	AC_PATH_PROG(LCOV, lcov, no)
	AS_IF([test "$LCOV" = "no"], [
		AC_MSG_ERROR(lcov tool is not installed)])
	CXXFLAGS="$CXXFLAGS -O1 -fprofile-arcs -ftest-coverage"
	CFLAGS="$CFLAGS -O1 -fprofile-arcs -ftest-coverage"
])

case $host in
  *mingw*)
    LIBTOOL_APP_LDFLAGS="$LIBTOOL_APP_LDFLAGS -all-static"
  ;;
esac

BUILD_EXEEXT=
case $build in
  *mingw*)
    BUILD_EXEEXT=".exe"
  ;;
esac

AC_CHECK_LIB(jansson, json_loads, JANSSON_LIBS=-ljansson)

AC_CONFIG_FILES([
    Makefile
    libsqlpars.pc
    libsqlpars-uninstalled.pc])

AC_SUBST(JANSSON_LIBS)
AC_SUBST(LIBTOOL_APP_LDFLAGS)
AC_SUBST(BUILD_EXEEXT)
AC_OUTPUT

