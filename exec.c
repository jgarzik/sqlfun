
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <jansson.h>
#include "sql.tab.h"
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

static const char *op_names[] = {
	[SEO_ADD]	= "SEO_ADD",
	[SEO_SUB]	= "SEO_SUB",
	[SEO_MUL]	= "SEO_MUL",
	[SEO_DIV]	= "SEO_DIV",
	[SEO_MOD]	= "SEO_MOD",
	[SEO_NEG]	= "SEO_NEG",
	[SEO_AND]	= "SEO_AND",
	[SEO_OR]	= "SEO_OR",
	[SEO_XOR]	= "SEO_XOR",
	[SEO_BITOR]	= "SEO_BITOR",
	[SEO_BITAND]	= "SEO_BITAND",
	[SEO_BITXOR]	= "SEO_BITXOR",
	[SEO_SHIFT]	= "SEO_SHIFT",
	[SEO_NOT]	= "SEO_NOT",
	[SEO_ASSIGN]	= "SEO_ASSIGN",
	[SEO_IS_NULL]	= "SEO_IS_NULL",
	[SEO_SHR]	= "SEO_SHR",
	[SEO_SHL]	= "SEO_SHL",
	[SEO_BETWEEN]	= "SEO_BETWEEN",
	[SEO_EXISTS]	= "SEO_EXISTS",
	[SEO_IN_SELECT]	= "SEO_IN_SELECT",
	[SEO_LIKE]	= "SEO_LIKE",
	[SEO_REGEX]	= "SEO_REGEX",
	[SEO_STRTOBIN]	= "SEO_STRTOBIN",
};

static const char *interval_names[] = {
	[SDI_DAY_HOUR]		= "SDI_DAY_HOUR",
	[SDI_DAY_MICROSECOND]	= "SDI_DAY_MICROSECOND",
	[SDI_DAY_MINUTE]	= "SDI_DAY_MINUTE",
	[SDI_DAY_SECOND]	= "SDI_DAY_SECOND",
	[SDI_YEAR_MONTH]	= "SDI_YEAR_MONTH",
	[SDI_YEAR]		= "SDI_YEAR",
	[SDI_HOUR_MICROSECOND]	= "SDI_HOUR_MICROSECOND",
	[SDI_HOUR_MINUTE]	= "SDI_HOUR_MINUTE",
	[SDI_HOUR_SECOND]	= "SDI_HOUR_SECOND",
};

static void print_and_free(json_t *jval)
{
	json_dumpf(jval, stdout, JSON_COMPACT | JSON_SORT_KEYS);
	printf("\n");
	json_decref(jval);
}

static void opout(const char *opname)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string(opname));
	print_and_free(obj);
}

static void opstr(const char *opname,
		  const char *str_name,
		  const char *str_val)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string(opname));
	json_object_set_new(obj, str_name, json_string(str_val));
	print_and_free(obj);
}

void sqlp_alias(struct psql_state *pstate, const char *alias)
{
	printf("exec ALIAS %s\n", alias);
}

void sqlp_assign(struct psql_state *pstate, const char *db_name, const char *name)
{
	printf("exec ASSIGN %s%s%s\n",
	       db_name ? db_name : "",
	       db_name ? "." : "",
	       name);
}

void sqlp_assign_at(struct psql_state *pstate, const char *name)
{
	printf("exec ASSIGN-AT %s\n", name);
}

void sqlp_bool(struct psql_state *pstate, int val)
{
	printf("exec BOOL %d\n", val);
}

void sqlp_call(struct psql_state *pstate, int n_args, const char *name)
{
	printf("exec CALL %d %s\n", n_args, name);
}

void sqlp_call_date(struct psql_state *pstate, int n_args, enum sqlp_expr_ops op)
{
	printf("exec CALL %d DATE %s\n",
	       n_args,
	       op == SEO_ADD ? "ADD" : "SUB");
}

void sqlp_call_trim_opts(struct psql_state *pstate, int trim_opts)
{
	printf("exec CALL TRIM OPTS %d\n", trim_opts);
}

void sqlp_case(struct psql_state *pstate, int n_list, int have_else)
{
	printf("exec CASE %d %d\n", n_list, have_else);
}

void sqlp_caseval(struct psql_state *pstate, int n_list, int have_else)
{
	printf("exec CASEVAL %d %d\n", n_list, have_else);
}

void sqlp_col_attr(struct psql_state *pstate, enum sqlp_col_attribs attr)
{
	opstr("ATTR", "type", attr_names[attr]);
}

void sqlp_col_attr_uniq(struct psql_state *pstate, int n_cols)
{
	printf("exec ATTR UNIQUE-KEY %d\n", n_cols);
}

void sqlp_col_attr_comm(struct psql_state *pstate, const char *comm)
{
	printf("exec ATTR COMMENT %s\n", comm);
}

void sqlp_col_charset(struct psql_state *pstate, const char *charset)
{
	printf("exec ATTR CHARSET %s\n", charset);
}

void sqlp_col_collate(struct psql_state *pstate, const char *collate)
{
	printf("exec ATTR COLLATE %s\n", collate);
}

void sqlp_col_def_str(struct psql_state *pstate, const char *str)
{
	opstr("ATTR", "DEFAULT-STR", str);
}

void sqlp_col_def_num(struct psql_state *pstate, int num)
{
	printf("exec ATTR DEFAULT-NUM %d\n", num);
}

void sqlp_col_def_float(struct psql_state *pstate, float num)
{
	printf("exec ATTR DEFAULT-FLOAT %g\n", num);
}

void sqlp_col_def_bool(struct psql_state *pstate, int bool)
{
	printf("exec ATTR DEFAULT-BOOL %d\n", bool);
}

void sqlp_col_key_pri(struct psql_state *pstate, int n_cols)
{
	printf("exec KEY-PRI %d\n", n_cols);
}

void sqlp_col_key(struct psql_state *pstate, int n_cols)
{
	printf("exec KEY %d\n", n_cols);
}

void sqlp_col_key_textidx(struct psql_state *pstate, int n_cols)
{
	printf("exec KEY-TEXTIDX %d\n", n_cols);
}

void sqlp_column(struct psql_state *pstate, const char *name)
{
	printf("exec COLUMN %s\n", name);
}

void sqlp_create_db(struct psql_state *pstate, int if_ne, const char *name)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("CREATE-DB"));
	json_object_set_new(obj, "if_ne", json_boolean(if_ne));
	json_object_set_new(obj, "name", json_string(name));
	print_and_free(obj);
}

void sqlp_create_sel(struct psql_state *pstate, int ignore_replace)
{
	printf("exec CREATE-SELECT %d\n", ignore_replace);
}

void sqlp_create_tbl(struct psql_state *pstate, int temp, int if_n_exists, int n_cols,
		     const char *db_name, const char *name)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("CREATE-TABLE"));
	json_object_set_new(obj, "temp", json_boolean(temp));
	json_object_set_new(obj, "if_n_exists", json_boolean(if_n_exists));
	json_object_set_new(obj, "n_cols", json_integer(n_cols));
	json_object_set_new(obj, "db_name", json_string(db_name ? db_name : ""));
	json_object_set_new(obj, "name", json_string(name));
	print_and_free(obj);
}

void sqlp_create_tbl_sel(struct psql_state *pstate, int temp, int if_n_exists, int n_cols,
		     const char *db_name, const char *name)
{
	printf("exec CREATE-TABLE-SELECT %d %d %d %s%s%s\n",
	       temp, if_n_exists, n_cols,
	       db_name ? db_name : "",
	       db_name ? "." : "",
	       name);
}

void sqlp_date_interval(struct psql_state *pstate, enum sqlp_date_intervals interval)
{
	printf("exec DATE-INTERVAL %s\n", interval_names[interval]);
}

void sqlp_def_col(struct psql_state *pstate, int flags, const char *name)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("DEFINE-COL"));
	json_object_set_new(obj, "flags", json_integer(flags));
	json_object_set_new(obj, "name", json_string(name));
	print_and_free(obj);
}

void sqlp_delete(struct psql_state *pstate, int opts, const char *name)
{
	printf("exec DELETE %d %s\n", opts, name);
}

void sqlp_delete_multi(struct psql_state *pstate, int opts, int n_del, int n_tbl_ref)
{
	printf("exec DELETE-MULTI %d %d %d\n", opts, n_del, n_tbl_ref);
}

void sqlp_drop_db(struct psql_state *pstate, int if_exists, const char *name)
{
	printf("exec DROP-DB %d %s\n", if_exists, name);
}

void sqlp_drop_table(struct psql_state *pstate, int temp, int if_exists, int n_tables)
{
	printf("exec DROP-TABLE %d %d %d\n", temp, if_exists, n_tables);
}

void sqlp_enum_val(struct psql_state *pstate, const char *val)
{
	printf("exec ENUM-VAL %s\n", val);
}

void sqlp_expr_cmp(struct psql_state *pstate, int comp)
{
	printf("exec CMP %d\n", comp);
}

void sqlp_expr_is_bool(struct psql_state *pstate, int val)
{
	printf("exec EXPR-IS-BOOL %d\n", val);
}

void sqlp_expr_is_in(struct psql_state *pstate, int val)
{
	printf("exec EXPR-IS-IN %d\n", val);
}

void sqlp_expr_op(struct psql_state *pstate, enum sqlp_expr_ops op)
{
	printf("exec EXPR-OP %s\n", op_names[op]);
}

void sqlp_expr_cmp_sel(struct psql_state *pstate, int sel_type, int comp)
{
	printf("exec CMP-SELECT %d %d\n", sel_type, comp);
}

void sqlp_fieldname(struct psql_state *pstate, const char *db_name, const char *name)
{
	printf("exec FIELD-NAME %s.%s\n", db_name, name);
}

void sqlp_float(struct psql_state *pstate, float val)
{
	printf("exec FLOAT %g\n", val);
}

void sqlp_group_by_list(struct psql_state *pstate, int n_list, int opts)
{
	printf("exec GROUPBY-LIST %d %d\n", n_list, opts);
}

void sqlp_group_by(struct psql_state *pstate, int opts)
{
	printf("exec GROUPBY %d\n", opts);
}

void sqlp_having(struct psql_state *pstate)
{
	opout("HAVING");
}

void sqlp_index(struct psql_state *pstate, const char *name)
{
	printf("exec INDEX %s\n", name);
}

void sqlp_index_hint(struct psql_state *pstate, int n_indexed, int opts)
{
	printf("exec INDEX-HINT %d %d\n", n_indexed, opts);
}

void sqlp_ins_cols(struct psql_state *pstate, int n_cols)
{
	printf("exec INSERT-COLS %d\n", n_cols);
}

void sqlp_ins_default(struct psql_state *pstate)
{
	opout("INSERT-DEFAULT");
}

void sqlp_ins_dup_update(struct psql_state *pstate, int n_assn)
{
	printf("exec INSERT DUP-ONUPDATE %d\n", n_assn);
}

void sqlp_insert(struct psql_state *pstate, int opts, int n_vals, const char *tbl_name)
{
	printf("exec INSERT %d %d %s\n", opts, n_vals, tbl_name);
}

void sqlp_insert_assn(struct psql_state *pstate, int opts, int n_assn, const char *tbl_name)
{
	printf("exec INSERT-ASSN %d %d %s\n", opts, n_assn, tbl_name);
}

void sqlp_insert_sel(struct psql_state *pstate, int opts, const char *tbl_name)
{
	printf("exec INSERT-SEL %d %s\n", opts, tbl_name);
}

void sqlp_into(struct psql_state *pstate, int n_cols)
{
	printf("exec INTO %d\n", n_cols);
}

void sqlp_join(struct psql_state *pstate, int opts)
{
	printf("exec JOIN %d\n", opts);
}

void sqlp_join_expr(struct psql_state *pstate)
{
	opout("JOIN-ON EXPR");
}

void sqlp_join_using(struct psql_state *pstate, int n_cols)
{
	printf("exec JOIN-USING %d\n", n_cols);
}

void sqlp_limit(struct psql_state *pstate, int two_expr)
{
	printf("exec LIMIT %d\n", two_expr);
}

void sqlp_name(struct psql_state *pstate, const char *name)
{
	printf("exec NAME %s\n", name);
}

void sqlp_now(struct psql_state *pstate)
{
	opout("NOW");
}

void sqlp_number(struct psql_state *pstate, int val)
{
	printf("exec INT/NUMBER %d\n", val);
}

void sqlp_order_by(struct psql_state *pstate, int n_list)
{
	printf("exec ORDER-BY %d\n", n_list);
}

void sqlp_replace_assn(struct psql_state *pstate, int opts, int n_assn, const char *name)
{
	printf("exec REPLACE-ASSN %d %d %s\n", opts, n_assn, name);
}

void sqlp_replace_vals(struct psql_state *pstate, int opts, int n_vals, const char *name)
{
	printf("exec REPLACE-VALS %d %d %s\n", opts, n_vals, name);
}

void sqlp_replace_sel(struct psql_state *pstate, int opts, const char *name)
{
	printf("exec REPLACE-SEL %d %s\n", opts, name);
}

void sqlp_select(struct psql_state *pstate, int opts, int n_expr, int n_tbl_ref)
{
	printf("exec SELECT %d %d %d\n", opts, n_expr, n_tbl_ref);
}

void sqlp_select_nodata(struct psql_state *pstate, int opts, int n_expr)
{
	printf("exec SELECT-NODATA %d %d\n", opts, n_expr);
}

void sqlp_select_all(struct psql_state *pstate)
{
	opout("SELECT-ALL");
}

void sqlp_set(struct psql_state *pstate, const char *name)
{
	printf("exec SET %s\n", name);
}

void sqlp_start_col(struct psql_state *pstate)
{
	opout("START-COL");
}

void sqlp_stmt(struct psql_state *pstate)
{
	opout("STMT");
}

void sqlp_string(struct psql_state *pstate, const char *str)
{
	printf("exec STRING %s\n", str);
}

void sqlp_subquery(struct psql_state *pstate)
{
	opout("SUBQUERY");
}

void sqlp_subquery_as(struct psql_state *pstate, const char *name)
{
	printf("exec SUBQUERY-AS %s\n", name);
}

void sqlp_table(struct psql_state *pstate, const char *db_name, const char *name)
{
	printf("exec TABLE %s%s%s\n",
	       db_name ? db_name : "",
	       db_name ? "." : "",
	       name);
}

void sqlp_table_refs(struct psql_state *pstate, int n_refs)
{
	printf("exec TABLE-REFERENCES %d\n", n_refs);
}

void sqlp_update(struct psql_state *pstate, int opts, int n_tbl_ref, int n_assn)
{
	printf("exec UPDATE %d %d %d\n", opts, n_tbl_ref, n_assn);
}

void sqlp_uservar(struct psql_state *pstate, const char *str)
{
	printf("exec USER-VAR %s\n", str);
}

void sqlp_values(struct psql_state *pstate, int n_vals)
{
	printf("exec VALUES %d\n", n_vals);
}

void sqlp_where(struct psql_state *pstate)
{
	opout("WHERE");
}

