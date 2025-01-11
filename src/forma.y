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

static int ASTDEBUG=1;
#define ASTD if (ASTDEBUG)

%}

%union {
	int integer;
	double real;
	char ch;
	char *id;
	char *str;
	struct fargs *fargs;
	struct fparams *fparams;
	struct ast *ast;
}

%define parse.error detailed

%token <ast> _if _else _return _header _do
%token let def arrow _read _write eq neq mine grte <ch> set
%token <integer> integer <real> real <str> str imgr 
%token <id> int_var float_var str_var cmpx_var intlist_var floatlist_var strlist_var cmpxlist_var int_fun float_fun str_fun cmpx_fun intlist_fun floatlist_fun strlist_fun cmpxlist_fun new_id

%type <ast> VDEC
%type <ast> NEWID ANYID INTVAR FLOATVAR STRVAR CMPXVAR ILISTVAR FLISTVAR SLISTVAR CLISTVAR
%type <ast> INTFUN FLOATFUN STRFUN CMPXFUN ILISTFUN FLISTFUN SLISTFUN CLISTFUN LISTFCALL
%type <ast> INT FLOAT STR S STMTS STMT TERM FDEF FBODY LISTVAR NTERM STERM CTERM LTERM LIST STRING FUNC IFC FCARGS FFC SFC CFC ILFC FLFC SLFC CLFC 
%type <ast> SLICE INARGS FNARGS CNARGS SARGS INTERM FNTERM INUM FNUM COND FBODYS
%type <fparams> FDECARGS
%type <fargs> FBARGS

%right '='
%left '-' '+'
%left '*' '/'
%left '^'
%%

S:	STMTS { ASTD printf("\n"); ASTD print_ast($1, 0, ""); ASTD printf("\n"); /*ASTD print_static_scope_stack(_staticstack);*/ }
 ;
STMTS:	STMTS STMT ';' { $$ = node2(STMTS, $1, $2);}
     |  STMTS SASTMT ';' {} // no action on SA
     |	%empty { $$ = NULL; }
     ;

SASTMT:	FDEF {}

STMT:	TERM //d
    |	_write NTERM { $$ = node1(_write, $2); }
    |	_write CTERM { $$ = node1(_write, $2); }
    |	_write STERM { $$ = node1(_write, $2); }
    |	_read { $$ = node0(_read); }
    |	'?' COND '?' { $$ = $2; }
    |   VDEC //d
    |   FBODY //d
    |	_return TERM { $$ = node1(_return, $2); }

COND:	STMT ',' _if TERM { $$ = node3(IF, $1, $4, NULL); }
    |	STMT ',' _else { $$ = node1(ELSE, $1); }
    |	STMT ',' _if TERM ',' COND { $$ = node3(IF, $1, $4, $6); }

/* BEGIN - Static purpose only */
FDEF:	new_id ':' FDECARGS arrow set '[' ']' { 
	add_static_symbol(_staticstack, $1, set_to_enumtype($5, 1), NATURE_FUNCTION, 0); 
	addparam_static_symbol(_staticstack, $1, $3->params, $3->natparams, $3->nparams); 
	free($3);
	/* print_static_scope_stack(_staticstack); */
	}
    |	new_id ':' FDECARGS arrow set { 
	add_static_symbol(_staticstack, $1, set_to_enumtype($5, 0), NATURE_FUNCTION, 0); 
	addparam_static_symbol(_staticstack, $1, $3->params, $3->natparams, $3->nparams);
	free($3);
	/* print_static_scope_stack(_staticstack); */
	}

FDECARGS:	FDECARGS '*' set {$$ = $1; $$->params[$$->nparams] = set_to_enumtype($3, 0); $$->natparams[($$->nparams)++] = NATURE_VARIABLE;}
	|	FDECARGS '*' set '[' ']' {$$ = $1; $$->params[$$->nparams] = set_to_enumtype($3, 1); $$->natparams[($$->nparams)++] = NATURE_VARIABLE; }
        |	FDECARGS '*' '(' FDECARGS arrow set ')' {$$ = $1; $$->params[$$->nparams] = set_to_enumtype($6, 0); free($4); $$->natparams[($$->nparams)++] = NATURE_FUNCTION; }
        |	FDECARGS '*' '(' FDECARGS arrow set '[' ']' ')' { $$ = $1; $$->params[$$->nparams] = set_to_enumtype($6, 1); free($4); $$->natparams[($$->nparams)++] = NATURE_FUNCTION; }
	|	set { $$ = calloc(1, sizeof(struct fparams)); $$->params[$$->nparams] = set_to_enumtype($1, 0); $$->natparams[($$->nparams)++] = NATURE_VARIABLE; }
	|	set '[' ']' {$$ = calloc(1, sizeof(struct fparams)); $$->params[$$->nparams] = set_to_enumtype($1, 1); $$->natparams[($$->nparams)++] = NATURE_VARIABLE;}
	|	'(' FDECARGS arrow set ')' {$$ = calloc(1, sizeof(struct fparams)); $$->params[$$->nparams] = set_to_enumtype($4, 0); free($2); $$->natparams[($$->nparams)++] = NATURE_FUNCTION; }
	|	'(' FDECARGS arrow set '[' ']' ')' {$$ = calloc(1, sizeof(struct fparams)); $$->params[$$->nparams] = set_to_enumtype($4, 1); free($2); $$->natparams[($$->nparams)++] = NATURE_FUNCTION;}
/* END - Static purpose only */

FBODY:	FBODYS '{' STMTS '}' { $$ = node2(FUN, $1, $3); }

FBODYS:	def FUNC '(' FBARGS ')' { 
	Symbol *symbol = find_static_symbol(_staticstack, $2->val1.id);
	if($4 == NULL) {
		if(symbol->nparams != 0) { 
			printf("FuncError: %s signature not matching declaration!\n", $2->val1.id);return -1;}
		// no params
		$$ = node1(FUNSIG, NULL);
		$$->val1.id = strdup($2->val1.id);
	}
	else if(symbol->nparams != $4->nargs) { 
		printf("FuncError: %s signature not matching declaration!\n", $2->val1.id);return -1;
	}
	else {
		// signature is matching
		ast_t **pnodes = malloc(sizeof(ast_t *) * $4->nargs);	
		add_static_symbol(_staticstack, $4->args[0], symbol->params[0], symbol->natparams[0], 0);
		ast_t *tmp = node0(set); tmp->val1.integer = symbol->params[0]; 
		pnodes[0] = node1(FPARAM, tmp);
		pnodes[0]->val1.id = $4->args[0];
		for(int i=1;i<$4->nargs;i++){
			add_static_symbol(_staticstack, $4->args[i], symbol->params[i], symbol->natparams[i], 0);
			ast_t *tmp = node0(set); tmp->val1.integer = symbol->params[i];
			pnodes[i] = node2(FPARAMS, tmp, pnodes[i-1]);
			pnodes[i]->val1.id = $4->args[i];
		} // not only variables !!
		$$ = node1(FUNSIG, pnodes[$4->nargs-1]);
		$$->val1.id = strdup($2->val1.id);
		free(pnodes);
	}
}

FBARGS:	FBARGS ',' ANYID { $$ = $1; $$->args[($$->nargs)++] = $3->val1.id;}
      | ANYID { $$ = calloc(1, sizeof(struct fargs)); $$->args[($$->nargs)++] = $1->val1.id; }
      | %empty { $$ = NULL; }

IFC:	int_fun '(' FCARGS ')' { $$ = node1(FCALL, $3); $$->val1.id = $1; }
FFC:	float_fun '(' FCARGS ')' { $$ = node1(FCALL, $3); $$->val1.id = $1; }
SFC:	str_fun '(' FCARGS ')' { $$ = node1(FCALL, $3); $$->val1.id = $1; }
CFC:	cmpx_fun '(' FCARGS ')' { $$ = node1(FCALL, $3); $$->val1.id = $1; }
ILFC:	intlist_fun '(' FCARGS ')' { $$ = node1(FCALL, $3); $$->val1.id = $1; }
FLFC:	floatlist_fun '(' FCARGS ')' { $$ = node1(FCALL, $3); $$->val1.id = $1; }
SLFC: 	strlist_fun '(' FCARGS ')' { $$ = node1(FCALL, $3); $$->val1.id = $1; }
CLFC:	cmpxlist_fun '(' FCARGS ')' { $$ = node1(FCALL, $3); $$->val1.id = $1; }

FCARGS: FCARGS ',' TERM { $$ = node2(FARGS, $1, $3); }
      |	FCARGS ',' FUNC { $$ = node2(FARGS, $1, $3); }
      |	TERM { $$ = node1(FARGV, $1); } // arg value
      | FUNC { $$ = node1(FARGF, $1); } // arg function
      |	%empty { $$ = NULL; };

VDEC:	let new_id ':' set '=' NTERM { 
	add_static_symbol(_staticstack, $2, set_to_enumtype($4, 0), NATURE_VARIABLE, 0);
	$$ = node1(VDEC, $6); $$->val1.id = $2; /*$$->val2.set = set_to_enumtype($4, 0);*/ }
    |	let new_id ':' set '=' STERM { 
	add_static_symbol(_staticstack, $2, set_to_enumtype($4, 0), NATURE_VARIABLE, 0); 
	$$ = node1(VDEC, $6); $$->val1.id = $2; $$->val2.set = set_to_enumtype($4, 0); }
    |	let new_id ':' set '=' CTERM { 
	add_static_symbol(_staticstack, $2, set_to_enumtype($4, 0), NATURE_VARIABLE, 0); 
	$$ = node1(VDEC, $6); $$->val1.id = $2; $$->val2.set = set_to_enumtype($4, 0); }
    |	let new_id '[' ']' ':' set '=' LTERM { 
	add_static_symbol(_staticstack, $2, set_to_enumtype($6, 1), NATURE_VARIABLE, 0); 
	$$ = node1(VDEC, $8); $$->val1.id = $2; $$->val2.set = set_to_enumtype($6, 0); }

ANYID:	INTVAR | FLOATVAR | STRVAR | LISTVAR | FUNC | NEWID
// VALUE:	INUM | FNUM | CTERM | STRING | LIST 
FUNC:	INTFUN | FLOATFUN | STRFUN | CMPXFUN | ILISTFUN | FLISTFUN | SLISTFUN | CLISTFUN  /*  a Func is an existing function name */

TERM:	NTERM
    |	STERM
    |	LTERM
    |	CTERM

// ---- LIST TERM
LTERM:	LTERM '+' LIST { $$ = node2(LADD, $1, $3); }
     |  LTERM eq LIST { $$ = node2(LEQ, $1, $3); }
     |	LTERM neq LIST { $$ = node2(LNEQ, $1, $3); }
     |	LTERM SLICE { $$ = node2(LSLICE, $1, $2); }
     |	LIST //d
 
LIST:	'[' INARGS ']' { $$ = $2; } 
    |	'[' FNARGS ']' { $$ = $2; }
    |	'[' CNARGS ']' { $$ = $2; }
    |	'[' SARGS ']' { $$ = $2; }
    |	'[' ']' { $$ = node0(ELIST); }
    |	LISTVAR //d
    |	LISTFCALL //d

LISTVAR:	ILISTVAR | FLISTVAR | SLISTVAR | CLISTVAR //d
LISTFCALL:	ILFC | FLFC | SLFC | CLFC //d

SARGS:	SARGS ',' STERM { $$ = node2(SLIST, $1, $3); }
     |	STERM { $$ = node1(SLIST, $1); }

// ---- STRING TERM
STERM:	STERM '+' STRING { $$ = node2(SADD, $1, $3); }
     |  STERM eq STRING { $$ = node2(SEQ, $1, $3); }
     |  STERM neq STRING { $$ = node2(SNEQ, $1, $3); }
     |  STERM SLICE { $$ = node2(SSLICE, $1, $2); }
     |	STRING //d

STRING:		STR | STRVAR | SFC

/* UTILS */
SLICE:	'[' ':' INTERM ']' { $$ = node1(SLICEL, $3); }
     |	'[' INTERM ':' ']' { $$ = node1(SLICER, $2); }
     |	'[' INTERM ':' INTERM ']' { $$ = node2(SLICE, $2, $4); }
     |	'[' INTERM ']' { $$ = node1(SLICEI, $2); }

INARGS:	INARGS ',' INTERM { $$ = node2(ILIST, $1, $3); }
      |	INTERM { $$ = node1(ILIST, $1); }

FNARGS: FNARGS ',' FNTERM { $$ = node2(FLIST, $1, $3); }
      | FNTERM { $$ = node1(FLIST, $1); }

CNARGS: CNARGS ',' CTERM { $$ = node2(CLIST, $1, $3); }
      | CTERM { $$ = node1(CLIST, $1); }

// ---- COMPLEX TERM ( this should be CNUM, and then implement rest of operations.. )
CTERM:	NTERM imgr '+' NTERM { $$ = node2(CADD, $1, $4); }
     |  NTERM imgr '-' NTERM { $$ = node2(CMNS, $1, $4); }
     |  CMPXVAR //d 
     |  CFC //d

// ---- NUMERIC TERM (very strict system!)
NTERM:	INTERM | FNTERM //d
/*
NTERM:	NTERM '-' NUMERIC { $$ = node2('-', $1, $3); }
     |	'-' NUMERIC { ast_t *z = node0(integer); z->val1.integer = 0; $$ = node2('-', z, $2); }
     |	'+' NUMERIC { ast_t *z = node0(integer); z->val1.integer = 0; $$ = node2('+', z, $2); }
     |	NTERM '+' NUMERIC { $$ = node2('+', $1, $3); }
     |	NTERM '*' NUMERIC { $$ = node2('*', $1, $3); }
     |	NTERM '/' NUMERIC { $$ = node2('/', $1, $3); }
     |	NTERM eq NUMERIC {}
     |  NTERM neq NUMERIC {}
     |  NTERM '>' NUMERIC {}
     |  NTERM '<' NUMERIC {}
     |  NTERM mine NUMERIC {}
     |  NTERM grte NUMERIC {}
     | 	NTERM '^' NUMERIC { $$ = node2('^', $1, $3); }
     |	'(' NTERM ')' { $$ = $2; }
     |  NUMERIC //d

NUMERIC:	INT | FLOAT | INTVAR | FLOATVAR | IFC | FFC
*/

INTERM: INTERM '-' INUM { $$=node2(NMNS, $1, $3); }
      |	'-' INUM { $$=node1(NMNS, $2); }
      |	'+' INUM { $$=node1(NADD, $2); }
      | INTERM '+' INUM { $$=node2(NADD, $1, $3); }
      |	INTERM '*' INUM { $$=node2(NMUL, $1, $3); }
      | INTERM '/' INUM { $$=node2(NDIV, $1, $3); }
      | INTERM eq INUM { $$=node2(NEQ, $1, $3); }
      | INTERM neq INUM { $$=node2(NNEQ, $1, $3); }
      | INTERM '>' INUM { $$=node2(NGRT, $1, $3); }
      | INTERM '<' INUM { $$=node2(NMIN, $1, $3); }
      | INTERM mine INUM { $$=node2(NMINE, $1, $3); }
      | INTERM grte INUM { $$=node2(NGRTE, $1, $3); }
      |	INTERM '^' INUM { $$=node2(NPOW, $1, $3); }
      |	'(' INTERM ')' { $$ = $2; }
      | INUM 
INUM: INT | INTVAR | IFC //d

FNTERM: FNTERM '-' FNUM { $$=node2(FMNS, $1, $3); }
      |	'-' FNUM { ast_t *t = node0(real); t->val1.real=0; $$ = node2(FMNS, t, $2); }
      |	'+' FNUM { $$ = $2; }
      | FNTERM '+' FNUM { $$=node2(FADD, $1, $3); }
      |	FNTERM '*' FNUM { $$=node2(FMUL, $1, $3); }
      | FNTERM '/' FNUM { $$=node2(FDIV, $1, $3); }
      | FNTERM eq FNUM { $$=node2(NEQ, $1, $3); }
      | FNTERM neq FNUM { $$=node2(NNEQ, $1, $3); }
      | FNTERM '>' FNUM { $$=node2(NGRT, $1, $3); }
      | FNTERM '<' FNUM { $$=node2(NMIN, $1, $3); }
      | FNTERM mine FNUM { $$=node2(NMINE, $1, $3); }
      | FNTERM grte FNUM { $$=node2(NGRTE, $1, $3); }
      |	FNTERM '^' FNUM { $$=node2(FPOW, $1, $3); } 
      |	'(' FNTERM ')' { $$ = $2; }
      | FNUM
FNUM: FLOAT | FLOATVAR | FFC //d

/* VALUE TOKENS */
INT:		integer { $$ = node0(integer); $$->val1.integer = $1; }
FLOAT:		real { $$ = node0(real); $$->val1.real = $1; }
STR: 		str { $$ = node0(str); $$->val1.str = $1; }

INTVAR:		int_var { $$ = node0(int_var); $$->val1.id = $1; }
FLOATVAR:	float_var { $$ = node0(float_var); $$->val1.id = $1; }
STRVAR:		str_var { $$ = node0(str_var); $$->val1.id = $1; }
CMPXVAR:	cmpx_var { $$ = node0(cmpx_var); $$->val1.id = $1; }
ILISTVAR:	intlist_var { $$ = node0(intlist_var); $$->val1.id = $1; }
FLISTVAR:	floatlist_var { $$ = node0(floatlist_var); $$->val1.id = $1; }
SLISTVAR:	strlist_var { $$ = node0(strlist_var); $$->val1.id = $1; }
CLISTVAR:	cmpxlist_var { $$ = node0(cmpxlist_var); $$->val1.id = $1; }

INTFUN:		int_fun { $$ = node0(int_fun); $$->val1.id = $1; }
FLOATFUN:	float_fun { $$ = node0(float_fun); $$->val1.id = $1; }
STRFUN:		str_fun { $$ = node0(str_fun); $$->val1.id = $1; }
CMPXFUN:	cmpx_fun  { $$ = node0(cmpx_fun); $$->val1.id = $1; }
ILISTFUN:	intlist_fun { $$ = node0(intlist_fun); $$->val1.id = $1; }
FLISTFUN:	floatlist_fun { $$ = node0(floatlist_fun); $$->val1.id = $1; }
SLISTFUN:	strlist_fun { $$ = node0(strlist_fun); $$->val1.id = $1; }
CLISTFUN:	cmpxlist_fun { $$ = node0(cmpxlist_fun); $$->val1.id = $1; }

NEWID:		new_id { $$ = node0(new_id); $$->val1.id = $1; }

//SET:		set { $$ = $1; }

%%

int main(int argc, char **argv) {
	_staticstack = create_static_scope_stack(10);
	yyin = fopen(argv[1], "r");
	yyparse();
}
