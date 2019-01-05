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

struct psql_state {
	yyscan_t	scanner;
};

extern void sqlp_alias(struct psql_state *pstate, const char *alias);
extern void sqlp_assign(struct psql_state *pstate, const char *db_name, const char *name);
extern void sqlp_assign_at(struct psql_state *pstate, const char *name);
extern void sqlp_bool(struct psql_state *pstate, int val);
extern void sqlp_call(struct psql_state *pstate, int n_args, const char *name);
extern void sqlp_call_date(struct psql_state *pstate, int n_args, enum sqlp_expr_ops op);
extern void sqlp_call_trim_opts(struct psql_state *pstate, int trim_opts);
extern void sqlp_case(struct psql_state *pstate, int n_list, int have_else);
extern void sqlp_caseval(struct psql_state *pstate, int n_list, int have_else);
extern void sqlp_col_attr(struct psql_state *pstate, enum sqlp_col_attribs attr);
extern void sqlp_col_attr_uniq(struct psql_state *pstate, int n_cols);
extern void sqlp_col_attr_comm(struct psql_state *pstate, const char *comm);
extern void sqlp_col_charset(struct psql_state *pstate, const char *charset);
extern void sqlp_col_collate(struct psql_state *pstate, const char *collate);
extern void sqlp_col_def_str(struct psql_state *pstate, const char *str);
extern void sqlp_col_def_num(struct psql_state *pstate, int num);
extern void sqlp_col_def_float(struct psql_state *pstate, float num);
extern void sqlp_col_def_bool(struct psql_state *pstate, int bool);
extern void sqlp_col_key_pri(struct psql_state *pstate, int n_cols);
extern void sqlp_col_key(struct psql_state *pstate, int n_cols);
extern void sqlp_col_key_textidx(struct psql_state *pstate, int n_cols);
extern void sqlp_column(struct psql_state *pstate, const char *name);
extern void sqlp_create_db(struct psql_state *pstate, int if_n_exists, const char *name);
extern void sqlp_create_tbl(struct psql_state *pstate, int temp, int if_n_exists, int n_cols,
			    const char *db_name, const char *name);
extern void sqlp_create_tbl_sel(struct psql_state *pstate, int temp, int if_n_exists, int n_cols,
			    const char *db_name, const char *name);
extern void sqlp_create_sel(struct psql_state *pstate, int ignore_replace);
extern void sqlp_date_interval(struct psql_state *pstate, enum sqlp_date_intervals interval);
extern void sqlp_def_col(struct psql_state *pstate, int flags, const char *name);
extern void sqlp_delete(struct psql_state *pstate, int opts, const char *name);
extern void sqlp_delete_multi(struct psql_state *pstate, int opts, int n_del, int n_tbl_ref);
extern void sqlp_drop_db(struct psql_state *pstate, int if_exists, const char *name);
extern void sqlp_drop_table(struct psql_state *pstate, int temp, int if_exists, int n_tables);
extern void sqlp_enum_val(struct psql_state *pstate, const char *val);
extern void sqlp_expr_op(struct psql_state *pstate, enum sqlp_expr_ops op);
extern void sqlp_expr_cmp(struct psql_state *pstate, int comp);
extern void sqlp_expr_cmp_sel(struct psql_state *pstate, int sel_type, int comp);
extern void sqlp_expr_is_bool(struct psql_state *pstate, int val);
extern void sqlp_expr_is_in(struct psql_state *pstate, int val);
extern void sqlp_fieldname(struct psql_state *pstate, const char *db_name, const char *name);
extern void sqlp_float(struct psql_state *pstate, float val);
extern void sqlp_group_by_list(struct psql_state *pstate, int n_list, int opts);
extern void sqlp_group_by(struct psql_state *pstate, int opts);
extern void sqlp_having(struct psql_state *pstate);
extern void sqlp_index(struct psql_state *pstate, const char *name);
extern void sqlp_index_hint(struct psql_state *pstate, int n_indexed, int opts);
extern void sqlp_ins_cols(struct psql_state *pstate, int n_cols);
extern void sqlp_ins_default(struct psql_state *pstate);
extern void sqlp_ins_dup_update(struct psql_state *pstate, int n_assn);
extern void sqlp_insert(struct psql_state *pstate, int opts, int n_vals, int n_cols, const char *tbl_name);
extern void sqlp_insert_assn(struct psql_state *pstate, int opts, int n_assn, const char *tbl_name);
extern void sqlp_insert_sel(struct psql_state *pstate, int opts, const char *tbl_name);
extern void sqlp_into(struct psql_state *pstate, int n_cols);
extern void sqlp_join(struct psql_state *pstate, int opts);
extern void sqlp_join_expr(struct psql_state *pstate);
extern void sqlp_join_using(struct psql_state *pstate, int n_cols);
extern void sqlp_limit(struct psql_state *pstate, int two_expr);
extern void sqlp_name(struct psql_state *pstate, const char *name);
extern void sqlp_now(struct psql_state *pstate);
extern void sqlp_number(struct psql_state *pstate, int val);
extern void sqlp_order_by(struct psql_state *pstate, int n_list);
extern void sqlp_replace_assn(struct psql_state *pstate, int opts, int n_assn, const char *name);
extern void sqlp_replace_vals(struct psql_state *pstate, int opts, int n_vals, const char *name);
extern void sqlp_replace_sel(struct psql_state *pstate, int opts, const char *name);
extern void sqlp_select(struct psql_state *pstate, int opts, int n_expr, int n_tbl_ref);
extern void sqlp_select_nodata(struct psql_state *pstate, int opts, int n_expr);
extern void sqlp_select_all(struct psql_state *pstate);
extern void sqlp_set(struct psql_state *pstate, const char *name);
extern void sqlp_start_col(struct psql_state *pstate);
extern void sqlp_string(struct psql_state *pstate, const char *str);
extern void sqlp_stmt(struct psql_state *pstate);
extern void sqlp_subquery(struct psql_state *pstate);
extern void sqlp_subquery_as(struct psql_state *pstate, const char *name);
extern void sqlp_table(struct psql_state *pstate, const char *db_name, const char *name);
extern void sqlp_table_refs(struct psql_state *pstate, int n_refs);
extern void sqlp_update(struct psql_state *pstate, int opts, int n_tbl_ref, int n_assn);
extern void sqlp_uservar(struct psql_state *pstate, const char *str);
extern void sqlp_values(struct psql_state *pstate, int n_vals);
extern void sqlp_where(struct psql_state *pstate);

#endif /* __SQL_PARSER_H__ */
