#include "list_t.h"
#include <stdlib.h>
#include <stdio.h>

enum {	STMTS = 1000, 
	FUN=1100, FUNSIG=1099, FPARAMS=1101, FPARAM=1102, FCALL=1110, FARGS=1111, FARGV=1112, FARGF=1113, 
	VDEC=1200,
	LADD=1300, LEQ=1310, LNEQ=1311, LSLICE=1320, ELIST=1330, ILIST=1331, FLIST=1332, CLIST=1333, SLIST=1334,
	CADD=1400, CMNS=1410,
	SADD=1500, SEQ=1510, SNEQ=1511, SSLICE=1520,
	SLICE=1600, SLICER=1601, SLICEL=1602, SLICEI=1603,
	NADD=1700, NMNS=1701, NMUL=1702, NDIV=1703, NPOW=1704, 
	NEQ=1710, NNEQ=1711, NGRT=1712, NMIN=1713, NGRTE=1714, NMINE=1715,
	IF=1800, ELSE=1801,
	FADD=1900, FMNS=1901, FMUL=1902, FDIV=1903, FPOW=1904,
	integer=2000,real=2001,str=2002,
	int_var=2100,float_var=2101,str_var=2102,cmpx_var=2103,
	intlist_var=2110, floatlist_var=2111, strlist_var=2112, cmpxlist_var=2113,
	int_fun=2200,float_fun=2201,str_fun=2202,cmpx_fun=2203,
	intlist_fun=2210, floatlist_fun=2211, strlist_fun=2212, cmpxlist_fun=2213,
	new_id=2300
};

#define MC 3

struct ast {
    int type;
    union {
        int integer;
	SymbolType set;
	double real;
        double _Complex compx;
        char *str;
        char *id;
	IntLinkedList *ilist;
        DoubleLinkedList *flist;
        ComplexLinkedList *clist;
        StringLinkedList *slist;
    } val1;
    union {
        int integer;
	SymbolType set;
	double real;
        double _Complex compx;
        char *str;
        char *id;
	IntLinkedList *ilist;
        DoubleLinkedList *flist;
        ComplexLinkedList *clist;
        StringLinkedList *slist;
    } val2;
    struct ast *c[MC];
};
typedef struct ast ast_t;

// future execution
struct ex {
    union {
        int integer;
	SymbolType set;
	double real;
        double _Complex compx;
        char *str;
        char *id;
	IntLinkedList *ilist;
        DoubleLinkedList *flist;
        ComplexLinkedList *clist;
        StringLinkedList *slist;
    } val;
};
typedef struct ex ex_t; 

ast_t *node0(int type); 
ast_t *node1(int type, ast_t *c1); 
ast_t *node2(int type, ast_t *c1, ast_t *c2); 
ast_t *node3(int type, ast_t *c1, ast_t *c2, ast_t *c3);

void print_ast(ast_t *t, int deep, const char *prefix); 

int validate(ast_t *t);
int optimize(ast_t *t);
ex_t ex(ast_t *t);
