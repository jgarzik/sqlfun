
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "sql.tab.h"
#include "yyl.h"
#include "sql.lex.h"
#include "sql-parser.h"

extern char *filename;

int
main(int ac, char **av)
{
  FILE *in_f;
  struct psql_state *pstate;

  if(ac > 1 && !strcmp(av[1], "-d")) {
    yydebug = 1; ac--; av++;	// TODO: move global into lib (layering issue)
  }

  pstate = psql_new();
  if (!pstate)
	abort();

  if(ac > 1) {
    puts("wut?");
    if((in_f = fopen(av[1], "r")) == NULL) {
      perror(av[1]);
      exit(1);
    }
    filename = av[1];
  } else {
    filename = "(stdin)";
    in_f = stdin;
  }

  psql_set_input(pstate, in_f);

  int res = psql_parse(pstate);

  psql_free(pstate);

  if (!res) {
    printf("{\"result\":true}\n");
    return 0;
  } else {
    printf("{\"result\":false}\n");
    return 1;
  }
} /* main */

