#ifndef __SQL_PARSER_H__
#define __SQL_PARSER_H__

enum sqlp_col_attribs {
	SCA_NOTNULL		= (1 << 0),
	SCA_DEF_STR		= (1 << 1),
	SCA_DEF_NUM		= (1 << 2),
	SCA_DEF_FLOAT		= (1 << 3),
	SCA_DEF_BOOL		= (1 << 4),
	SCA_AUTOINC		= (1 << 5),
	SCA_UNIQUE_KEY		= (1 << 6),
	SCA_PRIMARY_KEY		= (1 << 7),
	SCA_COMMENT		= (1 << 8),
};

extern void sqlp_col_attr(enum sqlp_col_attribs attr);
extern void sqlp_col_attr_uniq(int n_cols);
extern void sqlp_col_attr_comm(const char *comm);
extern void sqlp_col_charset(const char *charset);
extern void sqlp_col_collate(const char *collate);
extern void sqlp_col_def_str(const char *str);
extern void sqlp_col_def_num(int num);
extern void sqlp_col_def_float(float num);
extern void sqlp_col_def_bool(int bool);
extern void sqlp_create_db(int if_n_exists, const char *name);
extern void sqlp_create_tbl(int temp, int if_n_exists, int n_cols,
			    const char *db_name, const char *name);
extern void sqlp_def_col(int flags, const char *name);
extern void sqlp_drop_db(int if_exists, const char *name);
extern void sqlp_drop_table(int temp, int if_exists, int n_tables);
extern void sqlp_enum_val(const char *val);
extern void sqlp_start_col(void);
extern void sqlp_stmt(void);
extern void sqlp_table(const char *name);

#endif /* __SQL_PARSER_H__ */
