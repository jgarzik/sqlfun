/* Companion source code for "flex & bison", published by O'Reilly
 * Media, ISBN 978-0-596-15597-1
 * Copyright (c) 2009, Taughannock Networks. All rights reserved.
 * See the README file for license conditions and contact info.
 * $Header: /home/johnl/flnb/code/sql/RCS/lpmysql.y,v 2.1 2009/11/08 02:53:39 johnl Exp $
 */
/*
 * Parser for mysql subset
 */
%{
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

 %}

%code requires {
char *filename;

typedef struct YYLTYPE {
  int first_line;
  int first_column;
  int last_line;
  int last_column;
  char *filename;
} YYLTYPE;
# define YYLTYPE_IS_DECLARED 1

# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)                                                            \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	  (Current).filename     = YYRHSLOC (Rhs, 1).filename;	        \
	}								\
      else								\
	{ /* empty RHS */						\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	  (Current).filename  = NULL;					\
	}								\
    while (0)
}


%union {
	int intval;
	double floatval;
	char *strval;
	int subtok;
}
	
	/* names and literal values */

%token <strval> NAME
%token <strval> STRING
%token <intval> INTNUM
%token <intval> BOOL
%token <floatval> APPROXNUM

       /* user @abc names */

%token <strval> USERVAR

       /* operators and precedence levels */

%right ASSIGN
%left OR
%left XOR
%left ANDOP
%nonassoc IN IS LIKE REGEXP
%left NOT '!'
%left BETWEEN
%left <subtok> COMPARISON /* = <> < > <= >= <=> */
%left '|'
%left '&'
%left <subtok> SHIFT /* << >> */
%left '+' '-'
%left '*' '/' '%' MOD
%left '^'
%nonassoc UMINUS

%token ADD
%token ALL
%token ALTER
%token ANALYZE
%token AND
%token ANY
%token AS
%token ASC
%token AUTO_INCREMENT
%token BEFORE
%token BETWEEN
%token BIGINT
%token BINARY
%token BIT
%token BLOB
%token BOTH
%token BY
%token CALL
%token CASCADE
%token CASE
%token CHANGE
%token CHAR
%token CHECK
%token COLLATE
%token COLUMN
%token COMMENT
%token CONDITION
%token CONSTRAINT
%token CONTINUE
%token CONVERT
%token CREATE
%token CROSS
%token CURRENT_DATE
%token CURRENT_TIME
%token CURRENT_TIMESTAMP
%token CURRENT_USER
%token CURSOR
%token DATABASE
%token DATABASES
%token DATE
%token DATETIME
%token DAY_HOUR
%token DAY_MICROSECOND
%token DAY_MINUTE
%token DAY_SECOND
%token DECIMAL
%token DECLARE
%token DEFAULT
%token DELAYED
%token DELETE
%token DESC
%token DESCRIBE
%token DETERMINISTIC
%token DISTINCT
%token DISTINCTROW
%token DIV
%token DOUBLE
%token DROP
%token DUAL
%token EACH
%token ELSE
%token ELSEIF
%token ENCLOSED
%token END
%token ENUM
%token ESCAPED
%token <subtok> EXISTS
%token EXIT
%token EXPLAIN
%token FETCH
%token FLOAT
%token FOR
%token FORCE
%token FOREIGN
%token FROM
%token FULLTEXT
%token GRANT
%token GROUP
%token HAVING
%token HIGH_PRIORITY
%token HOUR_MICROSECOND
%token HOUR_MINUTE
%token HOUR_SECOND
%token IF
%token IGNORE
%token IN
%token INDEX
%token INFILE
%token INNER
%token INOUT
%token INSENSITIVE
%token INSERT
%token INT
%token INTEGER
%token INTERVAL
%token INTO
%token ITERATE
%token JOIN
%token KEY
%token KEYS
%token KILL
%token LEADING
%token LEAVE
%token LEFT
%token LIKE
%token LIMIT
%token LINES
%token LOAD
%token LOCALTIME
%token LOCALTIMESTAMP
%token LOCK
%token LONG
%token LONGBLOB
%token LONGTEXT
%token LOOP
%token LOW_PRIORITY
%token MATCH
%token MEDIUMBLOB
%token MEDIUMINT
%token MEDIUMTEXT
%token MINUTE_MICROSECOND
%token MINUTE_SECOND
%token MOD
%token MODIFIES
%token NATURAL
%token NOT
%token NO_WRITE_TO_BINLOG
%token NULLX
%token NUMBER
%token ON
%token ONDUPLICATE
%token OPTIMIZE
%token OPTION
%token OPTIONALLY
%token OR
%token ORDER
%token OUT
%token OUTER
%token OUTFILE
%token PRECISION
%token PRIMARY
%token PROCEDURE
%token PURGE
%token QUICK
%token READ
%token READS
%token REAL
%token REFERENCES
%token REGEXP
%token RELEASE
%token RENAME
%token REPEAT
%token REPLACE
%token REQUIRE
%token RESTRICT
%token RETURN
%token REVOKE
%token RIGHT
%token ROLLUP
%token SCHEMA
%token SCHEMAS
%token SECOND_MICROSECOND
%token SELECT
%token SENSITIVE
%token SEPARATOR
%token SET
%token SHOW
%token SMALLINT
%token SOME
%token SONAME
%token SPATIAL
%token SPECIFIC
%token SQL
%token SQLEXCEPTION
%token SQLSTATE
%token SQLWARNING
%token SQL_BIG_RESULT
%token SQL_CALC_FOUND_ROWS
%token SQL_SMALL_RESULT
%token SSL
%token STARTING
%token STRAIGHT_JOIN
%token TABLE
%token TEMPORARY
%token TEXT
%token TERMINATED
%token THEN
%token TIME
%token TIMESTAMP
%token TINYBLOB
%token TINYINT
%token TINYTEXT
%token TO
%token TRAILING
%token TRIGGER
%token UNDO
%token UNION
%token UNIQUE
%token UNLOCK
%token UNSIGNED
%token UPDATE
%token USAGE
%token USE
%token USING
%token UTC_DATE
%token UTC_TIME
%token UTC_TIMESTAMP
%token VALUES
%token VARBINARY
%token VARCHAR
%token VARYING
%token WHEN
%token WHERE
%token WHILE
%token WITH
%token WRITE
%token XOR
%token YEAR
%token YEAR_MONTH
%token ZEROFILL

 /* functions with special syntax */
%token FSUBSTRING
%token FTRIM
%token FDATE_ADD FDATE_SUB
%token FCOUNT

%type <intval> select_opts select_expr_list
%type <intval> val_list opt_val_list case_list
%type <intval> groupby_list opt_with_rollup opt_asc_desc
%type <intval> table_references opt_inner_cross opt_outer
%type <intval> left_or_right opt_left_or_right_outer column_list
%type <intval> index_list opt_for_join

%type <intval> delete_opts delete_list
%type <intval> insert_opts insert_vals insert_vals_list
%type <intval> insert_asgn_list opt_if_not_exists update_opts update_asgn_list
%type <intval> opt_temporary opt_length opt_binary opt_uz enum_list
%type <intval> column_atts data_type opt_ignore_replace create_col_list

%start stmt_list

%{
void yyerror(char *s, ...);
void lyyerror(YYLTYPE, char *s, ...);
void emit(char *s, ...);
 %}
  /* free discarded tokens */
%destructor { printf ("free at %d %s\n",@$.first_line, $$); free($$); } <strval>

%%

stmt_list: stmt ';'
  | stmt_list stmt ';'
  ;

stmt_list: error ';'
   | stmt_list error ';' ;

   /* statements: select statement */

stmt: select_stmt { emit("STMT"); }
   ;

select_stmt: SELECT select_opts select_expr_list
                        { emit("SELECTNODATA %d %d", $2, $3); } ;
    | SELECT select_opts select_expr_list
     FROM table_references
     opt_where opt_groupby opt_having opt_orderby opt_limit
     opt_into_list { emit("SELECT %d %d %d", $2, $3, $5); } ;
;

opt_where: /* nil */ 
   | WHERE expr { emit("WHERE"); };

opt_groupby: /* nil */ 
   | GROUP BY groupby_list opt_with_rollup
                             { emit("GROUPBYLIST %d %d", $3, $4); }
;

groupby_list: expr opt_asc_desc
                             { emit("GROUPBY %d",  $2); $$ = 1; }
   | groupby_list ',' expr opt_asc_desc
                             { emit("GROUPBY %d",  $4); $$ = $1 + 1; }
   ;

opt_asc_desc: /* nil */ { $$ = 0; }
   | ASC                { $$ = 0; }
   | DESC               { $$ = 1; }
    ;

opt_with_rollup: /* nil */  { $$ = 0; }
   | WITH ROLLUP  { $$ = 1; }
   ;

opt_having: /* nil */ | HAVING expr { emit("HAVING"); };

opt_orderby: /* nil */ | ORDER BY groupby_list { emit("ORDERBY %d", $3); }
   ;

opt_limit: /* nil */ | LIMIT expr { emit("LIMIT 1"); }
  | LIMIT expr ',' expr             { emit("LIMIT 2"); }
  ; 

opt_into_list: /* nil */ 
   | INTO column_list { emit("INTO %d", $2); }
   ;

column_list: NAME          { emit("COLUMN %s", $1); free($1); $$ = 1; }
  | STRING                 { lyyerror(@1, "string %s found where name required", $1);
                              emit("COLUMN %s", $1); free($1); $$ = 1; }
  | column_list ',' NAME   { emit("COLUMN %s", $3); free($3); $$ = $1 + 1; }
  | column_list ',' STRING { lyyerror(@3, "string %s found where name required", $1);
                            emit("COLUMN %s", $3); free($3); $$ = $1 + 1; }
  ;

select_opts:                          { $$ = 0; }
| select_opts ALL                 { if($$ & 01) lyyerror(@2,"duplicate ALL option"); $$ = $1 | 01; }
| select_opts DISTINCT            { if($$ & 02) lyyerror(@2,"duplicate DISTINCT option"); $$ = $1 | 02; }
| select_opts DISTINCTROW         { if($$ & 04) lyyerror(@2,"duplicate DISTINCTROW option"); $$ = $1 | 04; }
| select_opts HIGH_PRIORITY       { if($$ & 010) lyyerror(@2,"duplicate HIGH_PRIORITY option"); $$ = $1 | 010; }
| select_opts STRAIGHT_JOIN       { if($$ & 020) lyyerror(@2,"duplicate STRAIGHT_JOIN option"); $$ = $1 | 020; }
| select_opts SQL_SMALL_RESULT    { if($$ & 040) lyyerror(@2,"duplicate SQL_SMALL_RESULT option"); $$ = $1 | 040; }
| select_opts SQL_BIG_RESULT      { if($$ & 0100) lyyerror(@2,"duplicate SQL_BIG_RESULT option"); $$ = $1 | 0100; }
| select_opts SQL_CALC_FOUND_ROWS { if($$ & 0200) lyyerror(@2,"duplicate SQL_CALC_FOUND_ROWS option"); $$ = $1 | 0200; }
    ;

select_expr_list: select_expr { $$ = 1; }
    | select_expr_list ',' select_expr {$$ = $1 + 1; }
    | '*' { emit("SELECTALL"); $$ = 1; }
    ;

select_expr: expr opt_as_alias ;

table_references:    table_reference { $$ = 1; }
    | table_references ',' table_reference { $$ = $1 + 1; }
    ;

table_reference:  table_factor
  | join_table
;

table_factor:
    NAME opt_as_alias index_hint { emit("TABLE %s", $1); free($1); }
  | NAME '.' NAME opt_as_alias index_hint { emit("TABLE %s.%s", $1, $3);
                               free($1); free($3); }
  | table_subquery opt_as NAME { emit("SUBQUERYAS %s", $3); free($3); }
  | '(' table_references ')' { emit("TABLEREFERENCES %d", $2); }
  ;

opt_as: AS 
  | /* nil */
  ;

opt_as_alias: AS NAME { emit ("ALIAS %s", $2); free($2); }
  | NAME              { emit ("ALIAS %s", $1); free($1); }
  | /* nil */
  ;

join_table:
    table_reference opt_inner_cross JOIN table_factor opt_join_condition
                  { emit("JOIN %d", 0100+$2); }
  | table_reference STRAIGHT_JOIN table_factor
                  { emit("JOIN %d", 0200); }
  | table_reference STRAIGHT_JOIN table_factor ON expr
                  { emit("JOIN %d", 0200); }
  | table_reference left_or_right opt_outer JOIN table_factor join_condition
                  { emit("JOIN %d", 0300+$2+$3); }
  | table_reference NATURAL opt_left_or_right_outer JOIN table_factor
                  { emit("JOIN %d", 0400+$3); }
  ;

opt_inner_cross: /* nil */ { $$ = 0; }
   | INNER { $$ = 1; }
   | CROSS  { $$ = 2; }
;

opt_outer: /* nil */  { $$ = 0; }
   | OUTER {$$ = 4; }
   ;

left_or_right: LEFT { $$ = 1; }
    | RIGHT { $$ = 2; }
    ;

opt_left_or_right_outer: LEFT opt_outer { $$ = 1 + $2; }
   | RIGHT opt_outer  { $$ = 2 + $2; }
   | /* nil */ { $$ = 0; }
   ;

opt_join_condition: join_condition | /* nil */ ;

join_condition:
    ON expr { emit("ONEXPR"); }
    | USING '(' column_list ')' { emit("USING %d", $3); }
    ;

index_hint:
     USE KEY opt_for_join '(' index_list ')'
                  { emit("INDEXHINT %d %d", $5, 010+$3); }
   | IGNORE KEY opt_for_join '(' index_list ')'
                  { emit("INDEXHINT %d %d", $5, 020+$3); }
   | FORCE KEY opt_for_join '(' index_list ')'
                  { emit("INDEXHINT %d %d", $5, 030+$3); }
   | /* nil */
   ;

opt_for_join: FOR JOIN { $$ = 1; }
   | /* nil */ { $$ = 0; }
   ;

index_list: NAME  { emit("INDEX %s", $1); free($1); $$ = 1; }
   | index_list ',' NAME { emit("INDEX %s", $3); free($3); $$ = $1 + 1; }
   ;

table_subquery: '(' select_stmt ')' { emit("SUBQUERY"); }
   ;

   /* statements: delete statement */

stmt: delete_stmt { emit("STMT"); }
   ;

delete_stmt: DELETE delete_opts FROM NAME
    opt_where opt_orderby opt_limit
                  { emit("DELETEONE %d %s", $2, $4); free($4); }
;

delete_opts: delete_opts LOW_PRIORITY { $$ = $1 + 01; }
   | delete_opts QUICK { $$ = $1 + 02; }
   | delete_opts IGNORE { $$ = $1 + 04; }
   | /* nil */ { $$ = 0; }
   ;

delete_stmt: DELETE delete_opts
    delete_list
    FROM table_references opt_where
            { emit("DELETEMULTI %d %d %d", $2, $3, $5); }

delete_list: NAME opt_dot_star { emit("TABLE %s", $1); free($1); $$ = 1; }
   | delete_list ',' NAME opt_dot_star
            { emit("TABLE %s", $3); free($3); $$ = $1 + 1; }
   ;

opt_dot_star: /* nil */ | '.' '*' ;

delete_stmt: DELETE delete_opts
    FROM delete_list
    USING table_references opt_where
            { emit("DELETEMULTI %d %d %d", $2, $4, $6); }
;

   /* statements: insert statement */

stmt: insert_stmt { emit("STMT"); }
   ;

insert_stmt: INSERT insert_opts opt_into NAME
     opt_col_names
     VALUES insert_vals_list
     opt_ondupupdate { emit("INSERTVALS %d %d %s", $2, $7, $4); free($4) }
   ;

opt_ondupupdate: /* nil */
   | ONDUPLICATE KEY UPDATE insert_asgn_list { emit("DUPUPDATE %d", $4); }
   ;

insert_opts: /* nil */ { $$ = 0; }
   | insert_opts LOW_PRIORITY { $$ = $1 | 01 ; }
   | insert_opts DELAYED { $$ = $1 | 02 ; }
   | insert_opts HIGH_PRIORITY { $$ = $1 | 04 ; }
   | insert_opts IGNORE { $$ = $1 | 010 ; }
   ;

opt_into: INTO | /* nil */
   ;

opt_col_names: /* nil */
   | '(' column_list ')' { emit("INSERTCOLS %d", $2); }
   ;

insert_vals_list: '(' insert_vals ')' { emit("VALUES %d", $2); $$ = 1; }
   | insert_vals_list ',' '(' insert_vals ')' { emit("VALUES %d", $4); $$ = $1 + 1; }

insert_vals:
     expr { $$ = 1; }
   | DEFAULT { emit("DEFAULT"); $$ = 1; }
   | insert_vals ',' expr { $$ = $1 + 1; }
   | insert_vals ',' DEFAULT { emit("DEFAULT"); $$ = $1 + 1; }
   ;

insert_stmt: INSERT insert_opts opt_into NAME
    SET insert_asgn_list
    opt_ondupupdate
     { emit("INSERTASGN %d %d %s", $2, $6, $4); free($4) }
   ;

insert_stmt: INSERT insert_opts opt_into NAME opt_col_names
    select_stmt
    opt_ondupupdate { emit("INSERTSELECT %d %s", $2, $4); free($4); }
  ;

insert_asgn_list:
     NAME COMPARISON expr 
       { if ($2 != 4) { lyyerror(@2,"bad insert assignment to %s", $1); YYERROR; }
       emit("ASSIGN %s", $1); free($1); $$ = 1; }
   | NAME COMPARISON DEFAULT
       { if ($2 != 4) { lyyerror(@2,"bad insert assignment to %s", $1); YYERROR; }
                 emit("DEFAULT"); emit("ASSIGN %s", $1); free($1); $$ = 1; }
   | insert_asgn_list ',' NAME COMPARISON expr
       { if ($4 != 4) { lyyerror(@4,"bad insert assignment to %s", $1); YYERROR; }
                 emit("ASSIGN %s", $3); free($3); $$ = $1 + 1; }
   | insert_asgn_list ',' NAME COMPARISON DEFAULT
       { if ($4 != 4) { lyyerror(@4,"bad insert assignment to %s", $1); YYERROR; }
                 emit("DEFAULT"); emit("ASSIGN %s", $3); free($3); $$ = $1 + 1; }
   ;

   /** replace just like insert **/
stmt: replace_stmt { emit("STMT"); }
   ;

replace_stmt: REPLACE insert_opts opt_into NAME
     opt_col_names
     VALUES insert_vals_list
     opt_ondupupdate { emit("REPLACEVALS %d %d %s", $2, $7, $4); free($4) }
   ;

replace_stmt: REPLACE insert_opts opt_into NAME
    SET insert_asgn_list
    opt_ondupupdate
     { emit("REPLACEASGN %d %d %s", $2, $6, $4); free($4) }
   ;

replace_stmt: REPLACE insert_opts opt_into NAME opt_col_names
    select_stmt
    opt_ondupupdate { emit("REPLACESELECT %d %s", $2, $4); free($4); }
  ;

/** update **/
stmt: update_stmt { emit("STMT"); }
   ;

update_stmt: UPDATE update_opts table_references
    SET update_asgn_list
    opt_where
    opt_orderby
opt_limit { emit("UPDATE %d %d %d", $2, $3, $5); }
;

update_opts: /* nil */ { $$ = 0; }
   | insert_opts LOW_PRIORITY { $$ = $1 | 01 ; }
   | insert_opts IGNORE { $$ = $1 | 010 ; }
   ;

update_asgn_list:
     NAME COMPARISON expr 
     { if ($2 != 4) { lyyerror(@2,"bad update assignment to %s", $1); YYERROR; }
	 emit("ASSIGN %s", $1); free($1); $$ = 1; }
   | NAME '.' NAME COMPARISON expr 
   { if ($4 != 4) { lyyerror(@4,"bad update assignment to %s", $1); YYERROR; }
	 emit("ASSIGN %s.%s", $1, $3); free($1); free($3); $$ = 1; }
   | update_asgn_list ',' NAME COMPARISON expr
   { if ($4 != 4) { lyyerror(@4,"bad update assignment to %s", $3); YYERROR; }
	 emit("ASSIGN %s.%s", $3); free($3); $$ = $1 + 1; }
   | update_asgn_list ',' NAME '.' NAME COMPARISON expr
   { if ($6 != 4) { lyyerror(@6,"bad update  assignment to %s.$s", $3, $5); YYERROR; }
	 emit("ASSIGN %s.%s", $3, $5); free($3); free($5); $$ = 1; }
   ;


   /** create database **/

stmt: create_database_stmt { emit("STMT"); }
   ;

create_database_stmt: 
     CREATE DATABASE opt_if_not_exists NAME { emit("CREATEDATABASE %d %s", $3, $4); free($4); }
   | CREATE SCHEMA opt_if_not_exists NAME { emit("CREATEDATABASE %d %s", $3, $4); free($4); }
   ;

opt_if_not_exists:  /* nil */ { $$ = 0; }
   | IF EXISTS        { if(!$2) { lyyerror(@2,"IF EXISTS doesn't exist"); YYERROR; }
                        $$ = $2; /* NOT EXISTS hack */ }
   ;


   /** create table **/
stmt: create_table_stmt { emit("STMT"); }
   ;

create_table_stmt: CREATE opt_temporary TABLE opt_if_not_exists NAME
   '(' create_col_list ')' { emit("CREATE %d %d %d %s", $2, $4, $7, $5); free($5); }
   ;

create_table_stmt: CREATE opt_temporary TABLE opt_if_not_exists NAME '.' NAME
   '(' create_col_list ')' { emit("CREATE %d %d %d %s.%s", $2, $4, $9, $5, $7);
                          free($5); free($7); }
   ;

create_table_stmt: CREATE opt_temporary TABLE opt_if_not_exists NAME
   '(' create_col_list ')'
create_select_statement { emit("CREATESELECT %d %d %d %s", $2, $4, $7, $5); free($5); }
    ;

create_table_stmt: CREATE opt_temporary TABLE opt_if_not_exists NAME
   create_select_statement { emit("CREATESELECT %d %d 0 %s", $2, $4, $5); free($5); }
    ;

create_table_stmt: CREATE opt_temporary TABLE opt_if_not_exists NAME '.' NAME
   '(' create_col_list ')'
   create_select_statement  { emit("CREATESELECT %d %d 0 %s.%s", $2, $4, $5, $7);
                              free($5); free($7); }
    ;

create_table_stmt: CREATE opt_temporary TABLE opt_if_not_exists NAME '.' NAME
   create_select_statement { emit("CREATESELECT %d %d 0 %s.%s", $2, $4, $5, $7);
                          free($5); free($7); }
    ;

create_col_list: create_definition { $$ = 1; }
    | create_col_list ',' create_definition { $$ = $1 + 1; }
    ;

create_definition: { emit("STARTCOL"); } NAME data_type column_atts
                   { emit("COLUMNDEF %d %s", $3, $2); free($2); }

    | PRIMARY KEY '(' column_list ')'    { emit("PRIKEY %d", $4); }
    | KEY '(' column_list ')'            { emit("KEY %d", $3); }
    | INDEX '(' column_list ')'          { emit("KEY %d", $3); }
    | FULLTEXT INDEX '(' column_list ')' { emit("TEXTINDEX %d", $4); }
    | FULLTEXT KEY '(' column_list ')'   { emit("TEXTINDEX %d", $4); }
    ;

column_atts: /* nil */ { $$ = 0; }
    | column_atts NOT NULLX             { emit("ATTR NOTNULL"); $$ = $1 + 1; }
    | column_atts NULLX
    | column_atts DEFAULT STRING        { emit("ATTR DEFAULT STRING %s", $3); free($3); $$ = $1 + 1; }
    | column_atts DEFAULT INTNUM        { emit("ATTR DEFAULT NUMBER %d", $3); $$ = $1 + 1; }
    | column_atts DEFAULT APPROXNUM     { emit("ATTR DEFAULT FLOAT %g", $3); $$ = $1 + 1; }
    | column_atts DEFAULT BOOL          { emit("ATTR DEFAULT BOOL %d", $3); $$ = $1 + 1; }
    | column_atts AUTO_INCREMENT        { emit("ATTR AUTOINC"); $$ = $1 + 1; }
    | column_atts UNIQUE '(' column_list ')' { emit("ATTR UNIQUEKEY %d", $4); $$ = $1 + 1; }
    | column_atts UNIQUE KEY { emit("ATTR UNIQUEKEY"); $$ = $1 + 1; }
    | column_atts PRIMARY KEY { emit("ATTR PRIKEY"); $$ = $1 + 1; }
    | column_atts KEY { emit("ATTR PRIKEY"); $$ = $1 + 1; }
    | column_atts COMMENT STRING { emit("ATTR COMMENT %s", $3); free($3); $$ = $1 + 1; }
    ;

opt_length: /* nil */ { $$ = 0; }
   | '(' INTNUM ')' { $$ = $2; }
   | '(' INTNUM ',' INTNUM ')' { $$ = $2 + 1000*$4; }
   ;

opt_binary: /* nil */ { $$ = 0; }
   | BINARY { $$ = 4000; }
   ;

opt_uz: /* nil */ { $$ = 0; }
   | opt_uz UNSIGNED { $$ = $1 | 1000; }
   | opt_uz ZEROFILL { $$ = $1 | 2000; }
   ;

opt_csc: /* nil */
   | opt_csc CHAR SET STRING { emit("COLCHARSET %s", $4); free($4); }
   | opt_csc COLLATE STRING { emit("COLCOLLATE %s", $3); free($3); }
   ;

data_type:
     BIT opt_length { $$ = 10000 + $2; }
   | TINYINT opt_length opt_uz { $$ = 10000 + $2; }
   | SMALLINT opt_length opt_uz { $$ = 20000 + $2 + $3; }
   | MEDIUMINT opt_length opt_uz { $$ = 30000 + $2 + $3; }
   | INT opt_length opt_uz { $$ = 40000 + $2 + $3; }
   | INTEGER opt_length opt_uz { $$ = 50000 + $2 + $3; }
   | BIGINT opt_length opt_uz { $$ = 60000 + $2 + $3; }
   | REAL opt_length opt_uz { $$ = 70000 + $2 + $3; }
   | DOUBLE opt_length opt_uz { $$ = 80000 + $2 + $3; }
   | FLOAT opt_length opt_uz { $$ = 90000 + $2 + $3; }
   | DECIMAL opt_length opt_uz { $$ = 110000 + $2 + $3; }
   | DATE { $$ = 100001; }
   | TIME { $$ = 100002; }
   | TIMESTAMP { $$ = 100003; }
   | DATETIME { $$ = 100004; }
   | YEAR { $$ = 100005; }
   | CHAR opt_length opt_csc { $$ = 120000 + $2; }
   | VARCHAR '(' INTNUM ')' opt_csc { $$ = 130000 + $3; }
   | BINARY opt_length { $$ = 140000 + $2; }
   | VARBINARY '(' INTNUM ')' { $$ = 150000 + $3; }
   | TINYBLOB { $$ = 160001; }
   | BLOB { $$ = 160002; }
   | MEDIUMBLOB { $$ = 160003; }
   | LONGBLOB { $$ = 160004; }
   | TINYTEXT opt_binary opt_csc { $$ = 170000 + $2; }
   | TEXT opt_binary opt_csc { $$ = 171000 + $2; }
   | MEDIUMTEXT opt_binary opt_csc { $$ = 172000 + $2; }
   | LONGTEXT opt_binary opt_csc { $$ = 173000 + $2; }
   | ENUM '(' enum_list ')' opt_csc { $$ = 200000 + $3; }
   | SET '(' enum_list ')' opt_csc { $$ = 210000 + $3; }
   ;

enum_list: STRING { emit("ENUMVAL %s", $1); free($1); $$ = 1; }
   | enum_list ',' STRING { emit("ENUMVAL %s", $3); free($3); $$ = $1 + 1; }
   ;

create_select_statement: opt_ignore_replace opt_as select_stmt { emit("CREATESELECT %d", $1) }
   ;

opt_ignore_replace: /* nil */ { $$ = 0; }
   | IGNORE { $$ = 1; }
   | REPLACE { $$ = 2; }
   ;

opt_temporary:   /* nil */ { $$ = 0; }
   | TEMPORARY { $$ = 1;}
   ;

   /**** set user variables ****/

stmt: set_stmt { emit("STMT"); }
   ;

set_stmt: SET set_list ;

set_list: set_expr | set_list ',' set_expr ;

set_expr:
USERVAR COMPARISON expr { if ($2 != 4) { lyyerror(@2,"bad set to @%s", $1); YYERROR; }
		 emit("SET %s", $1); free($1); }
    | USERVAR ASSIGN expr { emit("SET %s", $1); free($1); }
    ;

   /**** expressions ****/

expr: NAME          { emit("NAME %s", $1); free($1); }
   | USERVAR         { emit("USERVAR %s", $1); free($1); }
   | NAME '.' NAME { emit("FIELDNAME %s.%s", $1, $3); free($1); free($3); }
   | STRING        { emit("STRING %s", $1); free($1); }
   | INTNUM        { emit("NUMBER %d", $1); }
   | APPROXNUM     { emit("FLOAT %g", $1); }
   | BOOL          { emit("BOOL %d", $1); }
   ;

expr: expr '+' expr { emit("ADD"); }
   | expr '-' expr { emit("SUB"); }
   | expr '*' expr { emit("MUL"); }
   | expr '/' expr { emit("DIV"); }
   | expr '%' expr { emit("MOD"); }
   | expr MOD expr { emit("MOD"); }
   | '-' expr %prec UMINUS { emit("NEG"); }
   | expr ANDOP expr { emit("AND"); }
   | expr OR expr { emit("OR"); }
   | expr XOR expr { emit("XOR"); }
   | expr COMPARISON expr { emit("CMP %d", $2); }
   | expr COMPARISON '(' select_stmt ')' { emit("CMPSELECT %d", $2); }
   | expr COMPARISON ANY '(' select_stmt ')' { emit("CMPANYSELECT %d", $2); }
   | expr COMPARISON SOME '(' select_stmt ')' { emit("CMPANYSELECT %d", $2); }
   | expr COMPARISON ALL '(' select_stmt ')' { emit("CMPALLSELECT %d", $2); }
   | expr '|' expr { emit("BITOR"); }
   | expr '&' expr { emit("BITAND"); }
   | expr '^' expr { emit("BITXOR"); }
   | expr SHIFT expr { emit("SHIFT %s", $2==1?"left":"right"); }
   | NOT expr { emit("NOT"); }
   | '!' expr { emit("NOT"); }
   | USERVAR ASSIGN expr { emit("ASSIGN @%s", $1); free($1); }
   ;    

expr:  expr IS NULLX     { emit("ISNULL"); }
   |   expr IS NOT NULLX { emit("ISNULL"); emit("NOT"); }
   |   expr IS BOOL      { emit("ISBOOL %d", $3); }
   |   expr IS NOT BOOL  { emit("ISBOOL %d", $4); emit("NOT"); }
   ;

expr: expr BETWEEN expr AND expr %prec BETWEEN { emit("BETWEEN"); }
   ;


val_list: expr { $$ = 1; }
   | expr ',' val_list { $$ = 1 + $3; }
   ;

opt_val_list: /* nil */ { $$ = 0 }
   | val_list
   ;

expr: expr IN '(' val_list ')'       { emit("ISIN %d", $4); }
   | expr NOT IN '(' val_list ')'    { emit("ISIN %d", $5); emit("NOT"); }
   | expr IN '(' select_stmt ')'     { emit("INSELECT"); }
   | expr NOT IN '(' select_stmt ')' { emit("INSELECT"); emit("NOT"); }
   | EXISTS '(' select_stmt ')'      { emit("EXISTS"); if($1)emit("NOT"); }
   ;

expr: NAME '(' opt_val_list ')' {  emit("CALL %d %s", $3, $1); free($1); }
   ;

  /* functions with special syntax */
expr: FCOUNT '(' '*' ')' { emit("COUNTALL") }
   | FCOUNT '(' expr ')' { emit(" CALL 1 COUNT"); } 

expr: FSUBSTRING '(' val_list ')' {  emit("CALL %d SUBSTR", $3);}
   | FSUBSTRING '(' expr FROM expr ')' {  emit("CALL 2 SUBSTR"); }
   | FSUBSTRING '(' expr FROM expr FOR expr ')' {  emit("CALL 3 SUBSTR"); }
| FTRIM '(' val_list ')' { emit("CALL %d TRIM", $3); }
   | FTRIM '(' trim_ltb expr FROM val_list ')' { emit("CALL 3 TRIM"); }
   ;

trim_ltb: LEADING { emit("INT 1"); }
   | TRAILING { emit("INT 2"); }
   | BOTH { emit("INT 3"); }
   ;

expr: FDATE_ADD '(' expr ',' interval_exp ')' { emit("CALL 3 DATE_ADD"); }
   |  FDATE_SUB '(' expr ',' interval_exp ')' { emit("CALL 3 DATE_SUB"); }
   ;

interval_exp: INTERVAL expr DAY_HOUR { emit("NUMBER 1"); }
   | INTERVAL expr DAY_MICROSECOND { emit("NUMBER 2"); }
   | INTERVAL expr DAY_MINUTE { emit("NUMBER 3"); }
   | INTERVAL expr DAY_SECOND { emit("NUMBER 4"); }
   | INTERVAL expr YEAR_MONTH { emit("NUMBER 5"); }
   | INTERVAL expr YEAR       { emit("NUMBER 6"); }
   | INTERVAL expr HOUR_MICROSECOND { emit("NUMBER 7"); }
   | INTERVAL expr HOUR_MINUTE { emit("NUMBER 8"); }
   | INTERVAL expr HOUR_SECOND { emit("NUMBER 9"); }
   ;

expr: CASE expr case_list END           { emit("CASEVAL %d 0", $3); }
   |  CASE expr case_list ELSE expr END { emit("CASEVAL %d 1", $3); }
   |  CASE case_list END                { emit("CASE %d 0", $2); }
   |  CASE case_list ELSE expr END      { emit("CASE %d 1", $2); }
   ;

case_list: WHEN expr THEN expr     { $$ = 1; }
         | case_list WHEN expr THEN expr { $$ = $1+1; } 
   ;

expr: expr LIKE expr { emit("LIKE"); }
   | expr NOT LIKE expr { emit("LIKE"); emit("NOT"); }
   ;

expr: expr REGEXP expr { emit("REGEXP"); }
   | expr NOT REGEXP expr { emit("REGEXP"); emit("NOT"); }
   ;

expr: CURRENT_TIMESTAMP { emit("NOW") };
   | CURRENT_DATE	{ emit("NOW") };
   | CURRENT_TIME	{ emit("NOW") };
   ;

expr: BINARY expr %prec UMINUS { emit("STRTOBIN"); }
   ;

%%

void
emit(char *s, ...)
{
  extern yylineno;

  va_list ap;
  va_start(ap, s);

  printf("rpn: ");
  vfprintf(stdout, s, ap);
  printf("\n");
}

void
yyerror(char *s, ...)
{
  va_list ap;
  va_start(ap, s);

  if(yylloc.first_line)
    fprintf(stderr, "%s:%d.%d-%d.%d: error: ", yylloc.filename, yylloc.first_line, yylloc.first_column,
	    yylloc.last_line, yylloc.last_column);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");

}

void
lyyerror(YYLTYPE t, char *s, ...)
{
  va_list ap;
  va_start(ap, s);

  if(t.first_line)
    fprintf(stderr, "%s:%d.%d-%d.%d: error: ", t.filename, t.first_line, t.first_column,
	    t.last_line, t.last_column);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}

main(int ac, char **av)
{
  extern FILE *yyin;

  if(ac > 1 && !strcmp(av[1], "-d")) {
    yydebug = 1; ac--; av++;
  }

  if(ac > 1) {
    if((yyin = fopen(av[1], "r")) == NULL) {
      perror(av[1]);
      exit(1);
    }
    filename = av[1];
  } else
    filename = "(stdin)";

  if(!yyparse())
    printf("SQL parse worked\n");
  else
    printf("SQL parse failed\n");
} /* main */
