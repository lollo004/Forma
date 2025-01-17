#ifndef ABSTRACT_TREE_H
#define ABSTRACT_TREE_H

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <math.h>

#include <list_t.h>
#include <stack_t.h>

typedef struct ExecutionContext ExecutionContext;

enum {	STMTS = 1000, 
	FUN=1100, FUNDEC=1098, FUNSIG=1099, FPARAMS=1101, FPARAM=1102, 
	FCALL=1110, FARGS=1111, IFARG=1120, FFARG=1121, CFARG=1122, SFARG=1123, ILFARG=1124, FLFARG=1125, CLFARG=1126, SLFARG=1127,
	IVDEC=1200, FVDEC=1201, CVDEC=1202, SVDEC=1203, ILVDEC=1210, FLVDEC=1211, CLVDEC=1212, SLVDEC=1213, RVDEC=1220,
	ILADD=1300, FLADD=1301, CLADD=1302, SLADD=1303, LEQ=1310, LNEQ=1311, ILSLICE=1320, FLSLICE=1321, CLSLICE=1322, SLSLICE=1323, ELIST=1330, 
	ILIST=1331, ILISTS=1335, ILISTE=1336, FLIST=1332, FLISTS=1337, FLISTE=1338, CLIST=1333, CLISTS=1339, CLISTE=1340, SLIST=1334, LISTS=1341, SLISTE=1342,
	SADD=1500, SSLICE=1520,
	SLICE=1600, SLICER=1601, SLICEL=1602, SLICEI=1603,

	IF=1800, ELSE=1801,
	FADD=1900, FMNS=1901, FMUL=1902, FDIV=1903, FPOW=1904,
	IADD=2400, IMNS=2401, IMUL=2402, IDIV=2403, IPOW= 2404,
	CNUMADD=2500, CNUMMNS=2501, CADD=2510, CMNS=2511, CMUL=2512, CDIV=2513,
	IWRITE=2600, FWRITE=2601, CWRITE=2602, SWRITE=2603,
	IREAD=2610, FREAD=2611, SREAD=2612,
	SLFC=2700, CLFC=2701, FLFC=2702, ILFC=2703,
	SFC=2710, CFC=2711, FFC=2712, IFC=2713,
	INEQ=1700, FNEQ=1701, SEQ=1702, CEQ=1703,
	INNEQ=1710, FNNEQ=1711, SNEQ=1712, CNEQ=1713,
	INGRT=1720, FNGRT=1721, CNGRT=1722,
	INMIN=1730, FNMIN=1731, CNMIN=1732,
	INMINE=1740, FNMINE=1741, CNMINE=1742,
	INGRTE=1750, FNGRTE=1751, CNGRTE=1752,
	ILEQ=3000, ILNEQ=3001, FLEQ=3002, FLNEQ=3003, CLEQ=3004, CLNEQ=3005, SLEQ=3006, SLNEQ=3007,
	
 	Integer=2000,Real=2001,Str=2002,
	Int_var=2100,Float_var=2101,Str_var=2102,Cmpx_var=2103,
	Intlist_var=2110, Floatlist_var=2111,Strlist_var=2112,Cmpxlist_var=2113,
	Int_fun=2200,Float_fun=2201,Str_fun=2202,Cmpx_fun=2203,
	Intlist_fun=2210, Floatlist_fun=2211,Strlist_fun=2212,Cmpxlist_fun=2213,
	New_id=2300
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
	void *any;
    } val;
    struct ast *c[MC];
};
typedef struct ast ast_t;

struct slice {
	int a;
	int b;
};
typedef struct slice slice_t;

struct exval {
    union {
        int integer;
	SymbolType set;
	double real;
        double _Complex cmpx;
        char *str;
        char *id;
	IntLinkedList *ilist;
        DoubleLinkedList *flist;
        ComplexLinkedList *clist;
        StringLinkedList *slist;
    	slice_t slice;
	void *any;
    } val;
};
typedef struct exval ex_t; 

ast_t *node0(int type); 
ast_t *node1(int type, ast_t *c1); 
ast_t *node2(int type, ast_t *c1, ast_t *c2); 
ast_t *node3(int type, ast_t *c1, ast_t *c2, ast_t *c3);

void print_ast(ast_t *t, int deep, const char *prefix); 

int optimize(ast_t *t);
int exec_env(ast_t *t);
ex_t ex(ast_t *t, ExecutionContext *e);

#endif
