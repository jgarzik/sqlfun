
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

static const char *cmp_names[] = {
	[4] = "EQ",
	[12] = "<=>",
	[6] = "GE",
	[2] = "GT",
	[5] = "LE",
	[1] = "LT",
	[3] = "<>",
};

static void print_and_free(json_t *jval)
{
	json_dumpf(jval, stdout, JSON_COMPACT | JSON_SORT_KEYS);
	printf("\n");
	json_decref(jval);
}

static void boolout(const char *bool_name, int bool_val)
{
	json_t *obj = json_object();
	json_object_set_new(obj, bool_name, json_boolean(bool_val));
	print_and_free(obj);
}

static void intout(const char *int_name, int int_val)
{
	json_t *obj = json_object();
	json_object_set_new(obj, int_name, json_integer(int_val));
	print_and_free(obj);
}

static void opii(const char *op_name,
		 const char *i1_name, int i1_val,
		 const char *i2_name, int i2_val)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string(op_name));
	json_object_set_new(obj, i1_name, json_integer(i1_val));
	json_object_set_new(obj, i2_name, json_integer(i2_val));
	print_and_free(obj);
}

static void opib(const char *op_name,
		 const char *i1_name, int i1_val,
		 const char *bool_name, int bool_val)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string(op_name));
	json_object_set_new(obj, i1_name, json_integer(i1_val));
	json_object_set_new(obj, bool_name, json_boolean(bool_val));
	print_and_free(obj);
}

static void strout(const char *str_name, const char *str_val)
{
	json_t *obj = json_object();
	json_object_set_new(obj, str_name, json_string(str_val));
	print_and_free(obj);
}

static void strout2(const char *str_name, const char *str_val,
		    const char *str2_name, const char *str2_val)
{
	json_t *obj = json_object();
	json_object_set_new(obj, str_name, json_string(str_val));
	json_object_set_new(obj, str2_name, json_string(str2_val));
	print_and_free(obj);
}

static void strouti(const char *str_name, const char *str_val,
		    const char *str2_name, int int_val)
{
	json_t *obj = json_object();
	json_object_set_new(obj, str_name, json_string(str_val));
	json_object_set_new(obj, str2_name, json_integer(int_val));
	print_and_free(obj);
}

static void opout(const char *opname)
{
	strout("op", opname);
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
	strout("ALIAS", alias);
}

void sqlp_assign(struct psql_state *pstate, const char *db_name, const char *name)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("ASSIGN"));
	json_object_set_new(obj, "db_name", json_string(db_name));
	json_object_set_new(obj, "name", json_string(name));
	print_and_free(obj);
}

void sqlp_assign_at(struct psql_state *pstate, const char *name)
{
	strout("ASSIGN-AT", name);
}

void sqlp_bool(struct psql_state *pstate, int val)
{
	boolout("BOOL", val);
}

void sqlp_call(struct psql_state *pstate, int n_args, const char *name)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("CALL"));
	json_object_set_new(obj, "n_args", json_integer(n_args));
	json_object_set_new(obj, "name", json_string(name));
	print_and_free(obj);
}

void sqlp_call_date(struct psql_state *pstate, int n_args, enum sqlp_expr_ops op)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("CALL"));
	json_object_set_new(obj, "n_args", json_integer(n_args));
	json_object_set_new(obj, "DATE", json_string(op_names[op]));
	print_and_free(obj);
}

void sqlp_call_trim_opts(struct psql_state *pstate, int trim_opts)
{
	intout("CALL TRIM OPTS", trim_opts);
}

void sqlp_case(struct psql_state *pstate, int n_list, int have_else)
{
	opib("CASE", "n_list", n_list, "have_else", have_else);
}

void sqlp_caseval(struct psql_state *pstate, int n_list, int have_else)
{
	opib("CASEVAL", "n_list", n_list, "have_else", have_else);
}

void sqlp_col_attr(struct psql_state *pstate, enum sqlp_col_attribs attr)
{
	strout("attr", attr_names[attr]);
}

void sqlp_col_attr_uniq(struct psql_state *pstate, int n_cols)
{
	strouti("attr", "SCA_UNIQUE_KEY",
		"n_cols", n_cols);
}

void sqlp_col_attr_comm(struct psql_state *pstate, const char *comm)
{
	strout2("attr", "SCA_COMMENT", "val", comm);
}

void sqlp_col_charset(struct psql_state *pstate, const char *charset)
{
	strout2("attr", "SCA_CHARSET", "val", charset);
}

void sqlp_col_collate(struct psql_state *pstate, const char *collate)
{
	strout2("attr", "SCA_COLLATE", "val", collate);
}

void sqlp_col_def_str(struct psql_state *pstate, const char *str)
{
	strout2("attr", "SCA_DEF_STR", "val", str);
}

void sqlp_col_def_num(struct psql_state *pstate, int num)
{
	strouti("attr", "SCA_DEF_NUM", "val", num);
}

void sqlp_col_def_float(struct psql_state *pstate, float num)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "attr", json_string("SCA_DEF_FLOAT"));
	json_object_set_new(obj, "val", json_real(num));
	print_and_free(obj);
}

void sqlp_col_def_bool(struct psql_state *pstate, int bool_val)
{
	strouti("attr", "SCA_DEF_BOOL",
		"val", bool_val);
}

void sqlp_col_key_pri(struct psql_state *pstate, int n_cols)
{
	intout("KEY-PRI", n_cols);
}

void sqlp_col_key(struct psql_state *pstate, int n_cols)
{
	intout("KEY", n_cols);
}

void sqlp_col_key_textidx(struct psql_state *pstate, int n_cols)
{
	intout("KEY-TEXTIDX", n_cols);
}

void sqlp_column(struct psql_state *pstate, const char *name)
{
	strout("COLUMN", name);
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
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("CREATE-SELECT"));
	json_object_set_new(obj, "ignore_replace", json_boolean(ignore_replace));
	print_and_free(obj);
}

void sqlp_create_tbl(struct psql_state *pstate, int temp, int if_n_exists, int n_cols,
		     const char *db_name, const char *name)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("CREATE-TABLE"));
	json_object_set_new(obj, "temp", json_boolean(temp));
	json_object_set_new(obj, "if_n_exists", json_boolean(if_n_exists));
	json_object_set_new(obj, "n_cols", json_integer(n_cols));
	json_object_set_new(obj, "db_name", json_string(db_name));
	json_object_set_new(obj, "name", json_string(name));
	print_and_free(obj);
}

void sqlp_create_tbl_sel(struct psql_state *pstate, int temp, int if_n_exists, int n_cols,
		     const char *db_name, const char *name)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("CREATE-TABLE-SELECT"));
	json_object_set_new(obj, "temp", json_boolean(temp));
	json_object_set_new(obj, "if_n_exists", json_boolean(if_n_exists));
	json_object_set_new(obj, "n_cols", json_integer(n_cols));
	json_object_set_new(obj, "db_name", json_string(db_name));
	json_object_set_new(obj, "name", json_string(name));
	print_and_free(obj);
}

void sqlp_date_interval(struct psql_state *pstate, enum sqlp_date_intervals interval)
{
	strout("DATE-INTERVAL", interval_names[interval]);
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
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("DELETE"));
	json_object_set_new(obj, "opts", json_integer(opts));
	json_object_set_new(obj, "name", json_string(name));
	print_and_free(obj);
}

void sqlp_delete_multi(struct psql_state *pstate, int opts, int n_del, int n_tbl_ref)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("DELETE-MULTI"));
	json_object_set_new(obj, "opts", json_integer(opts));
	json_object_set_new(obj, "n_del", json_integer(n_del));
	json_object_set_new(obj, "n_tbl_ref", json_integer(n_tbl_ref));
	print_and_free(obj);
}

void sqlp_drop_db(struct psql_state *pstate, int if_exists, const char *name)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("DROP-DB"));
	json_object_set_new(obj, "if_exists", json_boolean(if_exists));
	json_object_set_new(obj, "name", json_string(name));
	print_and_free(obj);
}

void sqlp_drop_table(struct psql_state *pstate, int temp, int if_exists, int n_tables)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("DROP-TABLE"));
	json_object_set_new(obj, "temp", json_boolean(temp));
	json_object_set_new(obj, "if_exists", json_boolean(if_exists));
	json_object_set_new(obj, "n_tables", json_integer(n_tables));
	print_and_free(obj);
}

void sqlp_enum_val(struct psql_state *pstate, const char *val)
{
	strout("ENUM-VAL", val);
}

void sqlp_expr_cmp(struct psql_state *pstate, int comp)
{
	strout("CMP", cmp_names[comp]);
}

void sqlp_expr_is_bool(struct psql_state *pstate, int val)
{
	boolout("EXPR-IS-BOOL", val);
}

void sqlp_expr_is_in(struct psql_state *pstate, int val)
{
	boolout("EXPR-IS-IN", val);
}

void sqlp_expr_op(struct psql_state *pstate, enum sqlp_expr_ops op)
{
	strout("EXPR-OP", op_names[op]);
}

void sqlp_expr_cmp_sel(struct psql_state *pstate, int sel_type, int comp)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("CMP-SELECT"));
	json_object_set_new(obj, "sel_type", json_integer(sel_type));
	json_object_set_new(obj, "comp", json_string(cmp_names[comp]));
	print_and_free(obj);
}

void sqlp_fieldname(struct psql_state *pstate, const char *db_name, const char *name)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("FIELD-NAME"));
	json_object_set_new(obj, "db_name", json_string(db_name));
	json_object_set_new(obj, "name", json_string(name));
	print_and_free(obj);
}

void sqlp_float(struct psql_state *pstate, float val)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "FLOAT", json_real(val));
	print_and_free(obj);
}

void sqlp_group_by_list(struct psql_state *pstate, int n_list, int opts)
{
	opii("GROUPBY-LIST", "n_list", n_list, "opts", opts);
}

void sqlp_group_by(struct psql_state *pstate, int opts)
{
	intout("GROUPBY", opts);
}

void sqlp_having(struct psql_state *pstate)
{
	opout("HAVING");
}

void sqlp_index(struct psql_state *pstate, const char *name)
{
	strout("INDEX", name);
}

void sqlp_index_hint(struct psql_state *pstate, int n_indexed, int opts)
{
	opii("INDEX-HINT", "n_indexed", n_indexed, "opts", opts);
}

void sqlp_ins_cols(struct psql_state *pstate, int n_cols)
{
	intout("INSERT-COLS", n_cols);
}

void sqlp_ins_default(struct psql_state *pstate)
{
	opout("INSERT-DEFAULT");
}

void sqlp_ins_dup_update(struct psql_state *pstate, int n_assn)
{
	intout("INSERT DUP-ONUPDATE", n_assn);
}

void sqlp_insert(struct psql_state *pstate, int opts, int n_vals, int n_cols, const char *tbl_name)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("INSERT"));
	json_object_set_new(obj, "opts", json_integer(opts));
	json_object_set_new(obj, "n_vals", json_integer(n_vals));
	json_object_set_new(obj, "n_cols", json_integer(n_cols));
	json_object_set_new(obj, "tbl_name", json_string(tbl_name));
	print_and_free(obj);
}

void sqlp_insert_assn(struct psql_state *pstate, int opts, int n_assn, const char *tbl_name)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("INSERT-ASSN"));
	json_object_set_new(obj, "opts", json_integer(opts));
	json_object_set_new(obj, "n_assn", json_integer(n_assn));
	json_object_set_new(obj, "tbl_name", json_string(tbl_name));
	print_and_free(obj);
}

void sqlp_insert_sel(struct psql_state *pstate, int opts, const char *tbl_name)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("INSERT-SEL"));
	json_object_set_new(obj, "opts", json_integer(opts));
	json_object_set_new(obj, "tbl_name", json_string(tbl_name));
	print_and_free(obj);
}

void sqlp_into(struct psql_state *pstate, int n_cols)
{
	intout("INTO", n_cols);
}

void sqlp_join(struct psql_state *pstate, int opts)
{
	intout("JOIN", opts);
}

void sqlp_join_expr(struct psql_state *pstate)
{
	opout("JOIN-ON EXPR");
}

void sqlp_join_using(struct psql_state *pstate, int n_cols)
{
	intout("JOIN-USING", n_cols);
}

void sqlp_limit(struct psql_state *pstate, int two_expr)
{
	intout("LIMIT", two_expr);
}

void sqlp_name(struct psql_state *pstate, const char *name)
{
	strout("NAME", name);
}

void sqlp_now(struct psql_state *pstate)
{
	opout("NOW");
}

void sqlp_number(struct psql_state *pstate, int val)
{
	intout("INT/NUMBER", val);
}

void sqlp_order_by(struct psql_state *pstate, int n_list)
{
	intout("ORDER-BY", n_list);
}

void sqlp_replace_assn(struct psql_state *pstate, int opts, int n_assn, const char *name)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("REPLACE-ASSN"));
	json_object_set_new(obj, "opts", json_integer(opts));
	json_object_set_new(obj, "n_assn", json_integer(n_assn));
	json_object_set_new(obj, "name", json_string(name));
	print_and_free(obj);
}

void sqlp_replace_vals(struct psql_state *pstate, int opts, int n_vals, const char *name)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("REPLACE-VALS"));
	json_object_set_new(obj, "opts", json_integer(opts));
	json_object_set_new(obj, "n_vals", json_integer(n_vals));
	json_object_set_new(obj, "name", json_string(name));
	print_and_free(obj);
}

void sqlp_replace_sel(struct psql_state *pstate, int opts, const char *name)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("REPLACE-SEL"));
	json_object_set_new(obj, "opts", json_integer(opts));
	json_object_set_new(obj, "name", json_string(name));
	print_and_free(obj);
}

void sqlp_select(struct psql_state *pstate, int opts, int n_expr, int n_tbl_ref)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("SELECT"));
	json_object_set_new(obj, "opts", json_integer(opts));
	json_object_set_new(obj, "n_expr", json_integer(n_expr));
	json_object_set_new(obj, "n_tbl_ref", json_integer(n_tbl_ref));
	print_and_free(obj);
}

void sqlp_select_nodata(struct psql_state *pstate, int opts, int n_expr)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("SELECT-NODATA"));
	json_object_set_new(obj, "opts", json_integer(opts));
	json_object_set_new(obj, "n_expr", json_integer(n_expr));
	print_and_free(obj);
}

void sqlp_select_all(struct psql_state *pstate)
{
	opout("SELECT-ALL");
}

void sqlp_set(struct psql_state *pstate, const char *name)
{
	strout("SET", name);
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
	strout("STRING", str);
}

void sqlp_subquery(struct psql_state *pstate)
{
	opout("SUBQUERY");
}

void sqlp_subquery_as(struct psql_state *pstate, const char *name)
{
	strout("SUBQUERY-AS", name);
}

void sqlp_table(struct psql_state *pstate, const char *db_name, const char *name)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("TABLE"));
	json_object_set_new(obj, "db_name", json_string(db_name));
	json_object_set_new(obj, "name", json_string(name));
	print_and_free(obj);
}

void sqlp_table_refs(struct psql_state *pstate, int n_refs)
{
	intout("TABLE-REFERENCES", n_refs);
}

void sqlp_update(struct psql_state *pstate, int opts, int n_tbl_ref, int n_assn)
{
	json_t *obj = json_object();
	json_object_set_new(obj, "op", json_string("UPDATE"));
	json_object_set_new(obj, "opts", json_integer(opts));
	json_object_set_new(obj, "n_tbl_ref", json_integer(n_tbl_ref));
	json_object_set_new(obj, "n_assn", json_integer(n_assn));
	print_and_free(obj);
}

void sqlp_uservar(struct psql_state *pstate, const char *str)
{
	strout("USER-VAR", str);
}

void sqlp_values(struct psql_state *pstate, int n_vals)
{
	intout("VALUES", n_vals);
}

void sqlp_where(struct psql_state *pstate)
{
	opout("WHERE");
}

