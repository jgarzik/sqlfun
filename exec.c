
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sql-parser.h"

void sqlp_create_db(int if_ne, const char *name)
{
	printf("exec CREATE-DB %d %s\n", if_ne, name);
}

void sqlp_drop_db(int if_exists, const char *name)
{
	printf("exec DROP-DB %d %s\n", if_exists, name);
}

void sqlp_drop_table(int temp, int if_exists, int n_tables)
{
	printf("exec DROP-TABLE %d %d %d\n", temp, if_exists, n_tables);
}

void sqlp_stmt(void)
{
	printf("exec STMT\n");
}

void sqlp_table(const char *name)
{
	printf("exec TABLE %s\n", name);
}

