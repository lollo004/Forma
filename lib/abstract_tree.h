#include "list_t.h"
#include <stdlib.h>
#include <stdio.h>

#define MC 3

struct ast {
    int type;
    union {
        int integer;
        double real;
        double _Complex compx;
        char *str;
        char *id;
	IntLinkedList *ilist;
        DoubleLinkedList *flist;
        ComplexLinkedList *clist;
        StringLinkedList *slist;
    } val;
    struct ast *c[MC];
};
typedef struct ast ast_t;

ast_t *node0(int type); 
ast_t *node1(int type, ast_t *c1); 
ast_t *node2(int type, ast_t *c1, ast_t *c2); 
ast_t *node3(int type, ast_t *c1, ast_t *c2, ast_t *c3);

void print_ast(ast_t *t, int deep); 

int validate(ast_t *t);
int optimize(ast_t *t);
int execute(ast_t *t);
