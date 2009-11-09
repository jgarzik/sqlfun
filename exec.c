
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sql-parser.h"

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
	printf("exec DEF-COL %d %s\n", flags, name);
}

void sqlp_drop_db(int if_exists, const char *name)
{
	printf("exec DROP-DB %d %s\n", if_exists, name);
}

void sqlp_drop_table(int temp, int if_exists, int n_tables)
{
	printf("exec DROP-TABLE %d %d %d\n", temp, if_exists, n_tables);
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

