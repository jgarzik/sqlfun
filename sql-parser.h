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

enum sqlp_expr_ops {
	SEO_ADD		= 0,
	SEO_SUB		= 1,
	SEO_MUL		= 2,
	SEO_DIV		= 3,
	SEO_MOD		= 4,
	SEO_NEG		= 5,
	SEO_AND		= 6,
	SEO_OR		= 7,
	SEO_XOR		= 8,
	SEO_BITOR	= 9,
	SEO_BITAND	= 10,
	SEO_BITXOR	= 11,
	SEO_SHIFT	= 12,
	SEO_NOT		= 13,
	SEO_ASSIGN	= 14,
	SEO_IS_NULL	= 15,
	SEO_SHR		= 16,
	SEO_SHL		= 17,
	SEO_BETWEEN	= 18,
	SEO_EXISTS	= 19,
	SEO_IN_SELECT	= 20,
	SEO_LIKE	= 21,
	SEO_REGEX	= 22,
	SEO_STRTOBIN	= 23,
};

enum sqlp_date_intervals {
	SDI_DAY_HOUR		= 0,
	SDI_DAY_MICROSECOND	= 1,
	SDI_DAY_MINUTE		= 2,
	SDI_DAY_SECOND		= 3,
	SDI_YEAR_MONTH		= 4,
	SDI_YEAR		= 5,
	SDI_HOUR_MICROSECOND	= 6,
	SDI_HOUR_MINUTE		= 7,
	SDI_HOUR_SECOND		= 8,
};

extern void sqlp_alias(const char *alias);
extern void sqlp_assign(const char *db_name, const char *name);
extern void sqlp_assign_at(const char *name);
extern void sqlp_bool(int val);
extern void sqlp_call(int n_args, const char *name);
extern void sqlp_call_date(int n_args, enum sqlp_expr_ops op);
extern void sqlp_call_trim_opts(int trim_opts);
extern void sqlp_case(int n_list, int have_else);
extern void sqlp_caseval(int n_list, int have_else);
extern void sqlp_col_attr(enum sqlp_col_attribs attr);
extern void sqlp_col_attr_uniq(int n_cols);
extern void sqlp_col_attr_comm(const char *comm);
extern void sqlp_col_charset(const char *charset);
extern void sqlp_col_collate(const char *collate);
extern void sqlp_col_def_str(const char *str);
extern void sqlp_col_def_num(int num);
extern void sqlp_col_def_float(float num);
extern void sqlp_col_def_bool(int bool);
extern void sqlp_col_key_pri(int n_cols);
extern void sqlp_col_key(int n_cols);
extern void sqlp_col_key_textidx(int n_cols);
extern void sqlp_column(const char *name);
extern void sqlp_create_db(int if_n_exists, const char *name);
extern void sqlp_create_tbl(int temp, int if_n_exists, int n_cols,
			    const char *db_name, const char *name);
extern void sqlp_create_tbl_sel(int temp, int if_n_exists, int n_cols,
			    const char *db_name, const char *name);
extern void sqlp_create_sel(int ignore_replace);
extern void sqlp_date_interval(enum sqlp_date_intervals interval);
extern void sqlp_def_col(int flags, const char *name);
extern void sqlp_delete(int opts, const char *name);
extern void sqlp_delete_multi(int opts, int n_del, int n_tbl_ref);
extern void sqlp_drop_db(int if_exists, const char *name);
extern void sqlp_drop_table(int temp, int if_exists, int n_tables);
extern void sqlp_enum_val(const char *val);
extern void sqlp_expr_op(enum sqlp_expr_ops op);
extern void sqlp_expr_cmp(int comp);
extern void sqlp_expr_cmp_sel(int sel_type, int comp);
extern void sqlp_expr_is_bool(int val);
extern void sqlp_expr_is_in(int val);
extern void sqlp_fieldname(const char *db_name, const char *name);
extern void sqlp_float(float val);
extern void sqlp_group_by_list(int n_list, int opts);
extern void sqlp_group_by(int opts);
extern void sqlp_having(void);
extern void sqlp_index(const char *name);
extern void sqlp_index_hint(int n_indexed, int opts);
extern void sqlp_ins_cols(int n_cols);
extern void sqlp_ins_default(void);
extern void sqlp_ins_dup_update(int n_assn);
extern void sqlp_insert(int opts, int n_vals, const char *tbl_name);
extern void sqlp_insert_assn(int opts, int n_assn, const char *tbl_name);
extern void sqlp_insert_sel(int opts, const char *tbl_name);
extern void sqlp_into(int n_cols);
extern void sqlp_join(int opts);
extern void sqlp_join_expr(void);
extern void sqlp_join_using(int n_cols);
extern void sqlp_limit(int two_expr);
extern void sqlp_name(const char *name);
extern void sqlp_now(void);
extern void sqlp_number(int val);
extern void sqlp_order_by(int n_list);
extern void sqlp_replace_assn(int opts, int n_assn, const char *name);
extern void sqlp_replace_vals(int opts, int n_vals, const char *name);
extern void sqlp_replace_sel(int opts, const char *name);
extern void sqlp_select(int opts, int n_expr, int n_tbl_ref);
extern void sqlp_select_nodata(int opts, int n_expr);
extern void sqlp_select_all(void);
extern void sqlp_set(const char *name);
extern void sqlp_start_col(void);
extern void sqlp_string(const char *str);
extern void sqlp_stmt(void);
extern void sqlp_subquery(void);
extern void sqlp_subquery_as(const char *name);
extern void sqlp_table(const char *db_name, const char *name);
extern void sqlp_table_refs(int n_refs);
extern void sqlp_update(int opts, int n_tbl_ref, int n_assn);
extern void sqlp_uservar(const char *str);
extern void sqlp_values(int n_vals);
extern void sqlp_where(void);

#endif /* __SQL_PARSER_H__ */
