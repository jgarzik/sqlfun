
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sql-parser.h"

static const char *attr_names[] = {
	[SCA_NOTNULL]		= "SCA_NOTNULL",
	[SCA_DEF_STR]		= "SCA_DEF_STR",
	[SCA_DEF_NUM]		= "SCA_DEF_NUM",
	[SCA_DEF_FLOAT]		= "SCA_DEF_FLOAT",
	[SCA_DEF_BOOL]		= "SCA_DEF_BOOL",
	[SCA_AUTOINC]		= "SCA_AUTOINC",
	[SCA_UNIQUE_KEY]	= "SCA_UNIQUE_KEY",
	[SCA_PRIMARY_KEY]	= "SCA_PRIMARY_KEY",
	[SCA_COMMENT]		= "SCA_COMMENT",
};

void sqlp_col_attr(enum sqlp_col_attribs attr)
{
	printf("exec ATTR %s\n", attr_names[attr]);
}

void sqlp_col_attr_uniq(int n_cols)
{
	printf("exec ATTR UNIQUE-KEY %d\n", n_cols);
}

void sqlp_col_attr_comm(const char *comm)
{
	printf("exec ATTR COMMENT %s\n", comm);
}

void sqlp_col_charset(const char *charset)
{
	printf("exec ATTR CHARSET %s\n", charset);
}

void sqlp_col_collate(const char *collate)
{
	printf("exec ATTR COLLATE %s\n", collate);
}

void sqlp_col_def_str(const char *str)
{
	printf("exec ATTR DEFAULT-STR %s\n", str);
}

void sqlp_col_def_num(int num)
{
	printf("exec ATTR DEFAULT-NUM %d\n", num);
}

void sqlp_col_def_float(float num)
{
	printf("exec ATTR DEFAULT-FLOAT %g\n", num);
}

void sqlp_col_def_bool(int bool)
{
	printf("exec ATTR DEFAULT-BOOL %d\n", bool);
}

void sqlp_create_db(int if_ne, const char *name)
{
	printf("exec CREATE-DB %d %s\n", if_ne, name);
}

void sqlp_create_tbl(int temp, int if_n_exists, int n_cols,
		     const char *db_name, const char *name)
{
	printf("exec CREATE-TABLE %d %d %d %s%s%s\n",
	       temp, if_n_exists, n_cols,
	       db_name ? db_name : "",
	       db_name ? "." : "",
	       name);
}

void sqlp_def_col(int flags, const char *name)
{
	printf("exec DEFINE-COL %d %s\n", flags, name);
}

void sqlp_drop_db(int if_exists, const char *name)
{
	printf("exec DROP-DB %d %s\n", if_exists, name);
}

void sqlp_drop_table(int temp, int if_exists, int n_tables)
{
	printf("exec DROP-TABLE %d %d %d\n", temp, if_exists, n_tables);
}

void sqlp_enum_val(const char *val)
{
	printf("exec ENUM-VAL %s\n", val);
}

void sqlp_start_col(void)
{
	printf("exec START-COL\n");
}

void sqlp_stmt(void)
{
	printf("exec STMT\n");
}

void sqlp_table(const char *name)
{
	printf("exec TABLE %s\n", name);
}

