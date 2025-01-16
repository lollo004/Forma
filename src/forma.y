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

#define STATIC_STACK_SIZE 128

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
%type <ast> INTFUN FLOATFUN STRFUN CMPXFUN ILISTFUN FLISTFUN SLISTFUN CLISTFUN
%type <ast> INT FLOAT STR S STMTS STMT TERM FDEF FBODY LISTVAR STERM CTERM STRING FUNC IFC FCARGS FFC SFC CFC ILFC FLFC SLFC CLFC 
%type <ast> ILTERM FLTERM CLTERM SLTERM ILIST FLIST CLIST SLIST SLICE INARGS FNARGS CNARGS SARGS INTERM FNTERM INUM FNUM CNUM COND FBODYS
%type <fparams> FDECARGS
%type <fargs> FBARGS

%right '=' eq neq '<' '>' mine grte ','
%left '-' '+'
%left '*' '/'
%left '^'
%left '['
%%

S:	STMTS { ASTD printf("\n"); ASTD print_ast($1, 0, ""); ASTD printf("\n"); /*ASTD print_static_scope_stack(_staticstack);*/ }
 ;
STMTS:	STMTS STMT ';' { $$ = node2(STMTS, $1, $2);}
     |  STMTS SASTMT ';' {} // no action on SA
     |	%empty { $$ = NULL; }
     ;

SASTMT:	FDEF {}

STMT:	TERM //d
    |	_write INTERM { $$ = node1(IWRITE, $2); }
    |	_write FNTERM { $$ = node1(FWRITE, $2); }
    |	_write CTERM { $$ = node1(CWRITE, $2); }
    |	_write STERM { $$ = node1(SWRITE, $2); }
    |	_read set { 
	if(set == 'N' || set == 'I') $$ = node0(IREAD); 
	if(set == 'R') $$ = node0(FREAD);
	else $$ = node0(SREAD); }
    |	'?' COND '?' { $$ = $2; }
    |   VDEC //d
    |   FBODY //d
    |	_return TERM { $$ = node1(_return, $2); }

// problem, can exist _else without _if .
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

FBODY:	FBODYS '{' STMTS '}' { 	ast_t *tmp = node2(FUN, $1, $3);
				$$ = node1(FUNDEC, tmp); $$->val.id = $1->val.id;}

FBODYS:	def FUNC '(' FBARGS ')' { 
	Symbol *symbol = find_static_symbol(_staticstack, $2->val.id);
	if($4 == NULL) {
		if(symbol->nparams != 0) { 
			printf("FuncError: %s signature not matching declaration!\n", $2->val.id);return -1;}
		// no params
		$$ = node1(FUNSIG, NULL);
		$$->val.id = strdup($2->val.id); // is strdup necessary?
	}
	else if(symbol->nparams != $4->nargs) { 
		printf("FuncError: %s signature not matching declaration!\n", $2->val.id);return -1;
	}
	else {
		// signature is matching
		ast_t **pnodes = malloc(sizeof(ast_t *) * $4->nargs);	
		add_static_symbol(_staticstack, $4->args[0], symbol->params[0], symbol->natparams[0], 0);
		ast_t *tmp = node0(set); tmp->val.integer = symbol->params[0]; 
		pnodes[0] = node1(FPARAM, tmp);
		pnodes[0]->val.id = $4->args[0];
		for(int i=1;i<$4->nargs;i++){
			add_static_symbol(_staticstack, $4->args[i], symbol->params[i], symbol->natparams[i], 0);
			ast_t *tmp = node0(set); tmp->val.integer = symbol->params[i];
			pnodes[i] = node2(FPARAMS, tmp, pnodes[i-1]);
			pnodes[i]->val.id = $4->args[i];
		} 
		$$ = node1(FUNSIG, pnodes[$4->nargs-1]);
		$$->val.id = strdup($2->val.id); //is strdup necessary?
		free(pnodes);
	}
}

FBARGS:	FBARGS ',' ANYID { $$ = $1; $$->args[($$->nargs)++] = $3->val.id;}
      | ANYID { $$ = calloc(1, sizeof(struct fargs)); $$->args[($$->nargs)++] = $1->val.id; }
      | %empty { $$ = NULL; }

IFC:	int_fun '(' FCARGS ')' { $$ = node1(IFC, $3); $$->val.id = $1; }
FFC:	float_fun '(' FCARGS ')' { $$ = node1(FFC, $3); $$->val.id = $1; }
SFC:	str_fun '(' FCARGS ')' { $$ = node1(SFC, $3); $$->val.id = $1; }
CFC:	cmpx_fun '(' FCARGS ')' { $$ = node1(CFC, $3); $$->val.id = $1; }
ILFC:	intlist_fun '(' FCARGS ')' { $$ = node1(ILFC, $3); $$->val.id = $1; }
FLFC:	floatlist_fun '(' FCARGS ')' { $$ = node1(FLFC, $3); $$->val.id = $1; }
SLFC: 	strlist_fun '(' FCARGS ')' { $$ = node1(SLFC, $3); $$->val.id = $1; }
CLFC:	cmpxlist_fun '(' FCARGS ')' { $$ = node1(CLFC, $3); $$->val.id = $1; }

FCARGS: FCARGS ',' FCARGS { $$ = node2(FARGS, $1, $3); }
      |	INTERM { $$ = node1(IFARG, $1); }
      |	FNTERM { $$ = node1(FFARG, $1); }
      |	CTERM { $$ = node1(CFARG, $1); }
      |	STERM { $$ = node1(SFARG, $1); }
      |	ILTERM { $$ = node1(ILFARG, $1); }
      |	FLTERM { $$ = node1(FLFARG, $1); }
      |	CLTERM { $$ = node1(CLFARG, $1); }
      |	SLTERM { $$ = node1(SLFARG, $1); }
      |	%empty { $$ = NULL; };

//| FUNC { $$ = node1(FARGF, $1); } // arg function LATER!

VDEC:	let new_id ':' set '=' INTERM { 
	add_static_symbol(_staticstack, $2, set_to_enumtype($4, 0), NATURE_VARIABLE, 0);
	$$ = node1(IVDEC, $6); $$->val.id = $2; }
    |	let new_id ':' set '=' FNTERM { 
	add_static_symbol(_staticstack, $2, set_to_enumtype($4, 0), NATURE_VARIABLE, 0);
	$$ = node1(FVDEC, $6); $$->val.id = $2; }
    |	let new_id ':' set '=' STERM { 
	add_static_symbol(_staticstack, $2, set_to_enumtype($4, 0), NATURE_VARIABLE, 0); 
	$$ = node1(SVDEC, $6); $$->val.id = $2; }
    |	let new_id ':' set '=' CTERM { 
	add_static_symbol(_staticstack, $2, set_to_enumtype($4, 0), NATURE_VARIABLE, 0); 
	$$ = node1(CVDEC, $6); $$->val.id = $2; }
    |	let new_id '[' ']' ':' set '=' ILTERM { 
	add_static_symbol(_staticstack, $2, set_to_enumtype($6, 1), NATURE_VARIABLE, 0); 
	$$ = node1(ILVDEC, $8); $$->val.id = $2; }
    |	let new_id '[' ']' ':' set '=' FLTERM { 
	add_static_symbol(_staticstack, $2, set_to_enumtype($6, 1), NATURE_VARIABLE, 0); 
	$$ = node1(FLVDEC, $8); $$->val.id = $2; }
    |	let new_id '[' ']' ':' set '=' CLTERM { 
	add_static_symbol(_staticstack, $2, set_to_enumtype($6, 1), NATURE_VARIABLE, 0); 
	$$ = node1(CLVDEC, $8); $$->val.id = $2; }
    |	let new_id '[' ']' ':' set '=' SLTERM { 
	add_static_symbol(_staticstack, $2, set_to_enumtype($6, 1), NATURE_VARIABLE, 0); 
	$$ = node1(SLVDEC, $8); $$->val.id = $2; }




ANYID:	INTVAR | FLOATVAR | STRVAR | LISTVAR | FUNC | NEWID
// VALUE:	INUM | FNUM | CTERM | STRING | LIST 
FUNC:	INTFUN | FLOATFUN | STRFUN | CMPXFUN | ILISTFUN | FLISTFUN | SLISTFUN | CLISTFUN  /*  a Func is an existing function name */

TERM:	INTERM
    |	FNTERM
    |	CTERM
    |	STERM
    |	ILTERM
    |	FLTERM
    |	CLTERM
    |	SLTERM

// ---- LIST TERM
ILTERM:	ILTERM '+' ILTERM { $$ = node2(ILADD, $1, $3); }
     |  ILTERM eq ILTERM { $$ = node2(ILEQ, $1, $3); }
     |	ILTERM neq ILTERM { $$ = node2(ILNEQ, $1, $3); }
     |	ILTERM SLICE { $$ = node2(ILSLICE, $1, $2); }
     |	ILIST //d

FLTERM:	FLTERM '+' FLTERM { $$ = node2(FLADD, $1, $3); }
     |  FLTERM eq FLTERM { $$ = node2(FLEQ, $1, $3); }
     |	FLTERM neq FLTERM { $$ = node2(FLNEQ, $1, $3); }
     |	FLTERM SLICE { $$ = node2(FLSLICE, $1, $2); }
     |	FLIST //d

CLTERM:	CLTERM '+' CLTERM { $$ = node2(CLADD, $1, $3); }
     |  CLTERM eq CLTERM { $$ = node2(CLEQ, $1, $3); }
     |	CLTERM neq CLTERM { $$ = node2(CLNEQ, $1, $3); }
     |	CLTERM SLICE { $$ = node2(CLSLICE, $1, $2); }
     |	CLIST //d

SLTERM:	SLTERM '+' SLTERM { $$ = node2(SLADD, $1, $3); }
     |  SLTERM eq SLTERM { $$ = node2(SLEQ, $1, $3); }
     |	SLTERM neq SLTERM { $$ = node2(SLNEQ, $1, $3); }
     |	SLTERM SLICE { $$ = node2(SLSLICE, $1, $2); }
     |	SLIST //d

ILIST:	'[' INARGS ']' { $$ = node1(ILISTE, $2); } 
    |	ILISTVAR //d
    |	ILFC //d

FLIST:	'[' FNARGS ']' { $$ = node1(FLISTE, $2); }
    |	FLISTVAR //d
    |	FLFC //d

CLIST:	'[' CNARGS ']' { $$ = node1(CLISTE, $2); }
    |	CLISTVAR //d
    |	CLFC //d
SLIST:	'[' SARGS ']' { $$ = node1(SLISTE, $2); }
    |	SLISTVAR //d
    |	SLFC //d

LISTVAR:	ILISTVAR | FLISTVAR | SLISTVAR | CLISTVAR //d

/* LIST UTILS */
INARGS:	INARGS ',' INARGS { $$ = node2(ILISTS, $1, $3); }
      |	INTERM { $$ = node1(ILIST, $1); }

FNARGS: FNARGS ',' FNARGS { $$ = node2(FLISTS, $1, $3); }
      | FNTERM { $$ = node1(FLIST, $1); }

CNARGS: CNARGS ',' CNARGS { $$ = node2(CLISTS, $1, $3); }
      | CTERM { $$ = node1(CLIST, $1); }

SARGS:	SARGS ',' SARGS { $$ = node2(SLIST, $1, $3); }
     |	STERM { $$ = node1(SLIST, $1); }

// ---- STRING TERM
STERM:	STERM '+' STERM { $$ = node2(SADD, $1, $3); }
     |  STERM eq STERM { $$ = node2(SEQ, $1, $3); }
     |  STERM neq STERM { $$ = node2(SNEQ, $1, $3); }
     |  STERM SLICE { $$ = node2(SSLICE, $1, $2); }
     |	STRING //d

STRING:		STR | STRVAR | SFC

/* UTILS */
SLICE:	'[' ':' INTERM ']' { $$ = node1(SLICEL, $3); }
     |	'[' INTERM ':' ']' { $$ = node1(SLICER, $2); }
     |	'[' INTERM ':' INTERM ']' { $$ = node2(SLICE, $2, $4); }
     |	'[' INTERM ']' { $$ = node1(SLICEI, $2); }


CTERM:	CTERM '+' CTERM { $$ = node2(CADD, $1, $3); }
     |	CTERM '-' CTERM { $$ = node2(CMNS, $1, $3); }
     |	CTERM '*' CTERM { $$ = node2(CMUL, $1, $3); }
     | 	CTERM '/' CTERM { $$ = node2(CDIV, $1, $3); }
     // coniugate :) ?
     // comparisons :) ?
     | 	CNUM

CNUM:	FNTERM imgr '+' FNTERM { $$ = node2(CNUMADD, $1, $4); }
    |	FNTERM imgr '-' FNTERM { $$ = node2(CNUMMNS, $1, $4); }
    |	CMPXVAR
    |	CFC

INTERM: INTERM '-' INTERM { $$=node2(IMNS, $1, $3); }
      |	'-' INTERM { ast_t *t = node0(Integer); t->val.integer=0; $$ = node2(FMNS, t, $2); }
      |	'+' INTERM { $$ = $2; }
      | INTERM '+' INTERM { $$=node2(IADD, $1, $3); }
      |	INTERM '*' INTERM { $$=node2(IMUL, $1, $3); }
      | INTERM '/' INTERM { $$=node2(IDIV, $1, $3); }
      | INTERM eq INTERM { $$=node2(INEQ, $1, $3); }
      | INTERM neq INTERM { $$=node2(INNEQ, $1, $3); }
      | INTERM '>' INTERM { $$=node2(INGRT, $1, $3); }
      | INTERM '<' INTERM { $$=node2(INMIN, $1, $3); }
      | INTERM mine INTERM { $$=node2(INMINE, $1, $3); }
      | INTERM grte INTERM { $$=node2(INGRTE, $1, $3); }
      |	INTERM '^' INTERM { $$=node2(IPOW, $1, $3); }
      |	'(' INTERM ')' { $$ = $2; }
      | INUM 
INUM: INT | INTVAR | IFC //d

FNTERM: FNTERM '-' FNTERM { $$=node2(FMNS, $1, $3); }
      |	'-' FNTERM { ast_t *t = node0(real); t->val.real=0; $$ = node2(FMNS, t, $2); }
      |	'+' FNTERM { $$ = $2; }
      | FNTERM '+' FNTERM { $$=node2(FADD, $1, $3); }
      |	FNTERM '*' FNTERM { $$=node2(FMUL, $1, $3); }
      | FNTERM '/' FNTERM { $$=node2(FDIV, $1, $3); }
      | FNTERM eq FNTERM { $$=node2(FNEQ, $1, $3); }
      | FNTERM neq FNTERM { $$=node2(FNNEQ, $1, $3); }
      | FNTERM '>' FNTERM { $$=node2(FNGRT, $1, $3); }
      | FNTERM '<' FNTERM { $$=node2(FNMIN, $1, $3); }
      | FNTERM mine FNTERM { $$=node2(FNMINE, $1, $3); }
      | FNTERM grte FNTERM { $$=node2(FNGRTE, $1, $3); }
      |	FNTERM '^' FNTERM { $$=node2(FPOW, $1, $3); } 
      |	'(' FNTERM ')' { $$ = $2; }
      | FNUM
FNUM: FLOAT | FLOATVAR | FFC //d

/* VALUE TOKENS */
INT:		integer { $$ = node0(Integer); $$->val.integer = $1; }
FLOAT:		real { $$ = node0(Real); $$->val.real = $1; }
STR: 		str { $$ = node0(Str); $$->val.str = $1; }

INTVAR:		int_var { $$ = node0(Int_var); $$->val.id = $1; }
FLOATVAR:	float_var { $$ = node0(Float_var); $$->val.id = $1; }
STRVAR:		str_var { $$ = node0(Str_var); $$->val.id = $1; }
CMPXVAR:	cmpx_var { $$ = node0(Cmpx_var); $$->val.id = $1; }
ILISTVAR:	intlist_var { $$ = node0(Intlist_var); $$->val.id = $1; }
FLISTVAR:	floatlist_var { $$ = node0(Floatlist_var); $$->val.id = $1; }
SLISTVAR:	strlist_var { $$ = node0(Strlist_var); $$->val.id = $1; }
CLISTVAR:	cmpxlist_var { $$ = node0(Cmpxlist_var); $$->val.id = $1; }

INTFUN:		int_fun { $$ = node0(Int_fun); $$->val.id = $1; }
FLOATFUN:	float_fun { $$ = node0(Float_fun); $$->val.id = $1; }
STRFUN:		str_fun { $$ = node0(Str_fun); $$->val.id = $1; }
CMPXFUN:	cmpx_fun  { $$ = node0(Cmpx_fun); $$->val.id = $1; }
ILISTFUN:	intlist_fun { $$ = node0(Intlist_fun); $$->val.id = $1; }
FLISTFUN:	floatlist_fun { $$ = node0(Floatlist_fun); $$->val.id = $1; }
SLISTFUN:	strlist_fun { $$ = node0(Strlist_fun); $$->val.id = $1; }
CLISTFUN:	cmpxlist_fun { $$ = node0(Cmpxlist_fun); $$->val.id = $1; }

NEWID:		new_id { $$ = node0(New_id); $$->val.id = $1; }

//SET:		set { $$ = $1; }

%%

int main(int argc, char **argv) {
	_staticstack = create_static_scope_stack(STATIC_STACK_SIZE);
	yyin = fopen(argv[1], "r");
	yyparse();
}
