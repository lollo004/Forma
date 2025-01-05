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

enum {STMTS = 1000, FDEF=1100, FPARAMS=1101, FPARAM=1102, FCALL=1110, FARGS=1111, FARG=1112 };
%}

%union {
	int integer;
	double real;
	double _Complex compx;
	char set;
	char *id;
	char *str;
	struct ast *ast;
}

%define parse.error detailed

%token <ast> _if _else _return 
%token let def arrow <ast> _read _write comp <set> set
%token <integer> integer <real> real <str> str imgr 
%token <id> int_var float_var str_var cmpx_var intlist_var floatlist_var strlist_var cmpxlist_var int_fun float_fun str_fun cmpx_fun intlist_fun floatlist_fun strlist_fun cmpxlist_fun new_id

%type <ast> VDEC FBARGS
%type <ast> NEWID ANYID INTVAR FLOATVAR STRVAR CMPXVAR ILISTVAR FLISTVAR SLISTVAR CLISTVAR
%type <ast> INTFUN FLOATFUN STRFUN CMPXFUN ILISTFUN FLISTFUN SLISTFUN CLISTFUN
%type <ast> INT FLOAT STR S STMTS STMT TERM FDEF FBODY LISTVAR NTERM STERM CTERM LTERM LIST STRING NUMERIC FUNC IFC FCARGS FFC SFC CFC ILFC FLFC SLFC CLFC VALUE


%right '='
%left '-' '+'
%left '*' '/'
%left '^'

%start S
%%

S:	STMTS { printf("\n"); print_ast($1, 0); printf("\n"); }

STMTS:	STMTS STMT ';' { $$ = node2(STMTS, $1, $2); }
     |	%empty { }

STMT:	TERM //d
    |	_write NTERM { $$ = node1(_write, $1); }
    |	_write STERM { $$ = node1(_write, $1); }
    |	_read { $$ = node0(_read); }
    |   VDEC //d
    |   FDEF //d SA purpose
    |   FBODY //d
    |	_return TERM { $$ = node1(_return, $1); }

/* BEGIN - Static purpose only */
FDEF:	NEWID ':' FDECARGS arrow set '[' ']' { 
	add_static_symbol(_staticstack, $1->val.id, set_to_enumtype($5, 1), NATURE_FUNCTION, 0); }
    |	NEWID ':' FDECARGS arrow set { add_static_symbol(_staticstack, $1->val.id, set_to_enumtype($5, 0), NATURE_FUNCTION, 0); }

FDECARGS:	FDECARGS '*' set
	|	FDECARGS '*' set '[' ']'
	|	set
	|	set '[' ']'
	|	'(' FDECARGS arrow set ')'
	|	'(' FDECARGS arrow set '[' ']' ')'
/* END - Static purpose only */

FBODY:	def FUNC '(' FBARGS ')' arrow '{' STMTS '}' { $$ = node3(FDEF, $2, $4, $8); }

FBARGS:	FBARGS ',' ANYID ':' set { $$ = node2(FPARAMS, $1, $3); }
      | FBARGS ',' ANYID '[' ']' ':' set  { $$ = node2(FPARAMS, $1, $3); }
      | ANYID ':' set {	
	add_static_symbol(_staticstack, $1->val.id, set_to_enumtype($3, 0), NATURE_VARIABLE, 0);
	$$ = node1(FPARAM, $1); } 
      |	ANYID '[' ']' ':' set {	
	add_static_symbol(_staticstack, $1->val.id, set_to_enumtype($5, 1), NATURE_VARIABLE, 0); 
	$$ = node1(FPARAM, $1); }
      | %empty { return 0; }

IFC:	INTFUN '(' FCARGS ')' { $$ = node2(FCALL, $1, $3); }
FFC:	FLOATFUN '(' FCARGS ')' { $$ = node2(FCALL, $1, $3); }
SFC:	STRFUN '(' FCARGS ')' { $$ = node2(FCALL, $1, $3); }
CFC:	CMPXFUN '(' FCARGS ')' { $$ = node2(FCALL, $1, $3); }
ILFC:	ILISTFUN '(' FCARGS ')' { $$ = node2(FCALL, $1, $3); }
FLFC:	FLISTFUN '(' FCARGS ')' { $$ = node2(FCALL, $1, $3); }
SLFC: 	SLISTFUN '(' FCARGS ')' { $$ = node2(FCALL, $1, $3); }
CLFC:	CLISTFUN '(' FCARGS ')' { $$ = node2(FCALL, $1, $3); }

FCARGS: FCARGS ',' VALUE { $$ = node2(FARGS, $1, $3); }
      |	VALUE { $$ = node1(FARG, $1); }
      |	%empty { return 0; };

// here

VDEC:	let NEWID ':' set '=' NTERM { add_static_symbol(_staticstack, $2->val.id, set_to_enumtype($4, 0), NATURE_VARIABLE, 0); }
    |	let NEWID ':' set '=' STERM { add_static_symbol(_staticstack, $2->val.id, set_to_enumtype($4, 0), NATURE_VARIABLE, 0); }
    |	let NEWID ':' set '=' CTERM { add_static_symbol(_staticstack, $2->val.id, set_to_enumtype($4, 0), NATURE_VARIABLE, 0); }
    |	let NEWID '[' ']' ':' set '=' LTERM { add_static_symbol(_staticstack, $2->val.id, set_to_enumtype($6, 0), NATURE_VARIABLE, 0); }
    // |	let NEWID ':' '=' TERM future impl.

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
LISTVAR:	ILISTVAR | FLISTVAR | SLISTVAR | CLISTVAR
LISTFUN:	ILFC | FLFC | SLFC | CLFC

SARGS:	SARGS ',' STERM
     |	STERM

// ---- STRING TERM
STERM:	STERM '+' STRING
     |  STERM comp STRING
     |  STERM SLICE
     |	STRING

STRING:		STR | STRVAR | SFC

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
     |  CMPXVAR
     |  CFC

// ---- NUMERIC TERM
NTERM:	NTERM '-' NUMERIC { $$ = node2('-', $1, $3); }
     |	'-' NUMERIC { ast_t *z = node0(integer); z->val.integer = 0; $$ = node2('-', z, $2); }
     |	'+' NUMERIC { ast_t *z = node0(integer); z->val.integer = 0; $$ = node2('+', z, $2); }
     |	NTERM '+' NUMERIC { $$ = node2('+', $1, $3); }
     |	NTERM '*' NUMERIC { $$ = node2('*', $1, $3); }
     |	NTERM '/' NUMERIC { $$ = node2('/', $1, $3); }
     |	NTERM comp NUMERIC {}
     | 	NTERM '^' NUMERIC { $$ = node2('^', $1, $3); }
     |	'(' NTERM ')' { $$ = $2; }
     |  NUMERIC //d

NUMERIC:	INT | FLOAT | INTVAR | FLOATVAR | IFC | FFC

/* VALUE TOKENS */
INT:		integer { $$ = node0(integer); $$->val.integer = $1; }
FLOAT:		real { $$ = node0(real); $$->val.real = $1; }
STR: 		str { $$ = node0(str); $$->val.str = $1; }

INTVAR:		int_var { $$ = node0(int_var); $$->val.id = $1; }
FLOATVAR:	float_var { $$ = node0(float_var); $$->val.id = $1; }
STRVAR:		str_var { $$ = node0(str_var); $$->val.id = $1; }
CMPXVAR:	cmpx_var { $$ = node0(cmpx_var); $$->val.id = $1; }
ILISTVAR:	intlist_var { $$ = node0(intlist_var); $$->val.id = $1; }
FLISTVAR:	floatlist_var { $$ = node0(floatlist_var); $$->val.id = $1; }
SLISTVAR:	strlist_var { $$ = node0(strlist_var); $$->val.id = $1; }
CLISTVAR:	cmpxlist_var { $$ = node0(cmpxlist_var); $$->val.id = $1; }

INTFUN:		int_fun { $$ = node0(int_fun); $$->val.id = $1; }
FLOATFUN:	float_fun { $$ = node0(float_fun); $$->val.id = $1; }
STRFUN:		str_fun { $$ = node0(str_fun); $$->val.id = $1; }
CMPXFUN:	cmpx_fun  { $$ = node0(cmpx_fun); $$->val.id = $1; }
ILISTFUN:	intlist_fun { $$ = node0(intlist_fun); $$->val.id = $1; }
FLISTFUN:	floatlist_fun { $$ = node0(floatlist_fun); $$->val.id = $1; }
SLISTFUN:	strlist_fun { $$ = node0(strlist_fun); $$->val.id = $1; }
CLISTFUN:	cmpxlist_fun { $$ = node0(cmpxlist_fun); $$->val.id = $1; }

NEWID:		new_id { $$ = node0(new_id); $$->val.id = $1; }

%%

int main(int argc, char **argv) {
	_staticstack = create_static_scope_stack(10);
	yyin = fopen(argv[1], "r");
	yyparse();
}
