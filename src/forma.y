%{
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <string.h>
#include "static_analysis.h"
#include "abstract_tree.h"

int yydebug=0;
extern FILE *yyin;
int yylex(void);
extern int yylineno; // Dichiarazione di yylineno
extern void yyerror(const char *msg) {
	printf("Parse error at Line %d: %s\n", yylineno, msg);
}

StaticScopeStack *_staticstack;

%}

%union {
	unsigned long natural;
	long int integer;
	long double real;
	double _Complex compx;
	char *id;
	char *str;
	struct ast *ast;
}

%define parse.error detailed

%token _if _else _return 
%token let def arrow print comp set
%token integer floating str imgr 
%token int_var float_var str_var intlist_var floatlist_var strlist_var int_fun float_fun str_fun intlist_fun floatlist_fun strlist_fun <id> new_id

%type <ast> NEWID VDEC

%right '='
%left '-' '+'
%left '*' '/'
%left '^'

%start S
%%

S:	STMTS

STMTS:	STMTS STMT ';'
     |	%empty

STMT:	TERM
    |	print NTERM
    |	print STERM
    |   VDEC
    |   FDEF
    |   FBODY
    |	FCALL
    |	_return TERM

FDEF:	def NEWID ':' FDECARGS arrow set '[' ']'
    |	def NEWID ':' FDECARGS arrow set

FDECARGS:	FDECARGS '*' set
	|	FDECARGS '*' set '[' ']'
	|	set
	|	set '[' ']'
	|	'(' FDECARGS arrow set ')'
	|	'(' FDECARGS arrow set '[' ']' ')'

FBODY:	NEWID '(' FBARGS ')' arrow '{' STMTS '}'

FBARGS:	FBARGS ',' ANYID
      | FBARGS ',' ANYID '[' ']'
      | ANYID
      |	ANYID '[' ']'
      | %empty

FCALL:	FUNC '(' FCARGS ')' 

FCARGS: FCARGS ',' VALUE
      |	VALUE
      |	%empty

VDEC:	let NEWID ':' SET '=' TERM { 
	add_static_symbol(_staticstack, $2->id, TYPE_INT, NATURE_VARIABLE, 0);
	}
    |	let NEWID ':' '=' TERM

ANYID:	INTVAR | FLOATVAR | STRVAR | LISTVAR | FUNC | NEWID
VALUE:	NUMERIC | STRING | LIST /* A value is known to be existing in this static scope */
FUNC:	INTFUN | FLOATFUN | STRFUN | LISTFUN /*  a Func is an existing function name */

TERM:	NTERM
    |	STERM
    |	LTERM
    |	CTERM

// ---- LIST TERM
LTERM:	LTERM '+' LIST
     |	LTERM comp LIST
     |	LTERM SLICE
     |	LIST
 
LIST:	'[' NARGS ']' /* check if all same type */
    |	'[' SARGS ']'
    |	'[' ']'
    |	LISTVAR
    |	LISTFUN

/* Should be more complete */
LISTVAR:	ILISTVAR | FLISTVAR | SLISTVAR
LISTFUN:	ILISTFUN | FLISTFUN | SLISTFUN

SARGS:	SARGS ',' STERM
     |	STERM

// ---- STRING TERM
STERM:	STERM '+' STRING
     |  STERM comp STRING
     |  STERM SLICE
     |	STRING

STRING:		STR | STRVAR | STRFUN

/* UTILS */
SLICE:	'[' ':' NTERM ']'
     |	'[' NTERM ':' ']'
     |	'[' NTERM ':' NTERM ']'
     |	'[' NTERM ']'

NARGS:	NARGS ',' NTERM
     |	NTERM

// ---- COMPLEX TERM
CTERM:	NTERM imgr '+' NTERM
     |  NTERM imgr '-' NTERM 

// ---- NUMERIC TERM
NTERM:	NTERM '-' NUMERIC
     |	'-' NUMERIC /* 0 is hidden */
     |	'+' NUMERIC /* same */
     |	NTERM '+' NUMERIC
     |	NTERM '*' NUMERIC
     |	NTERM '/' NUMERIC
     |	NTERM comp NUMERIC
     | 	NTERM '^' NUMERIC
     |	'(' NTERM ')'
     |  NUMERIC

NUMERIC:	INT | FLOAT | INTVAR | FLOATVAR | INTFUN | FLOATFUN

/* VALUE TOKENS */
INT:		integer
FLOAT:		floating
STR: 		str
INTVAR:		int_var
FLOATVAR:	float_var
STRVAR:		str_var
ILISTVAR:	intlist_var
FLISTVAR:	floatlist_var
SLISTVAR:	strlist_var
INTFUN:		int_fun
FLOATFUN:	float_fun
STRFUN:		str_fun
ILISTFUN:	intlist_fun
FLISTFUN:	floatlist_fun
SLISTFUN:	strlist_fun

NEWID:		new_id { $$->id = $1; }

SET:		set

%%

int main(int argc, char **argv) {
	_staticstack = create_static_scope_stack(10);
	yyin = fopen(argv[1], "r");
	yyparse();
}
