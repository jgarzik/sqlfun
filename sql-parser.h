#ifndef __SQL_PARSER_H__
#define __SQL_PARSER_H__

extern void sqlp_create_db(int if_n_exists, const char *name);
extern void sqlp_drop_db(int if_exists, const char *name);
extern void sqlp_drop_table(int temp, int if_exists, int n_tables);
extern void sqlp_stmt(void);
extern void sqlp_table(const char *name);

#endif /* __SQL_PARSER_H__ */
