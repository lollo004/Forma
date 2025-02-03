#ifndef TYPES_T_H
#define TYPES_T_H

#include "list_t.h"
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

#endif
