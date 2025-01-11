#include "abstract_tree.h"

ast_t *node0(int type) {
    ast_t *ret = calloc(sizeof *ret, 1);
    ret->type = type;
    return ret;
}

ast_t *node1(int type, ast_t *c1) {
        ast_t *ret = node0(type);
        ret->c[0] = c1;
        return ret;
}

ast_t *node2(int type, ast_t *c1, ast_t *c2) {
        ast_t *ret = node1(type, c1);
        ret->c[1] = c2;
        return ret;
}
ast_t *node3(int type, ast_t *c1, ast_t *c2, ast_t *c3) {
        ast_t *ret = node2(type, c1, c2);
        ret->c[2] = c3;
        return ret;
}

void print_ast(ast_t *t, int deep, const char *prefix) {
    if (!t) return;

    // Stampa il nodo corrente
    printf("%s+-- Node Type: %d\n", prefix, t->type);

    // Costruisce il prefisso per i figli
    char new_prefix[256];
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, "|   ");

    for (int i = 0; i < MC; i++) {
        if (t->c[i]) {
            if (i == MC - 1 || !t->c[i + 1]) {
                // Se è l'ultimo figlio, cambia il prefisso
                snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, "    ");
                print_ast(t->c[i], deep + 1, new_prefix);
            } else {
                print_ast(t->c[i], deep + 1, new_prefix);
            }
        }
    }
}
/*
void print_ast(ast_t *t, int deep) {
    if (!t) return;

    // Stampa la parte centrale per il nodo corrente
    for (int i = 0; i < deep; i++) {
        printf("  ");  // Indentazione per la profondità
    }
    printf("+-- Node Type: %d\n", t->type);

    // Stampa i figli
    for (int i = 0; i < MC; i++) {
        if (t->c[i]) {
            // Stampa la connessione al figlio
            for (int j = 0; j < deep + 1; j++) {
                printf("  ");  // Indentazione per la profondità
            }
            printf("|-- ");
            print_ast(t->c[i], deep + 1);  // Chiamata ricorsiva per il figlio
        }
    }
}
*/
int validate(ast_t *t) { return 0; }
int optimize(ast_t *t) { return 0; }

/* Variables don't need to be stored with type, type is controlled by parser */
/* This doesn't apply to list yet */
/* Every operational node should differenciate if I, F, C, S, IL, FL, SL, CL */

ex_t ex(ast_t *t) { 
	ex_t ret = {.val.integer = 0};
	if(!t) return ret;

	switch (t->type) {
		case STMTS:
			return ex(t->c[0]), ex(t->c[1]);
		



		// you need to update all codes after!!
		case FMNS:
			ret.val.real = ex(t->c[0]).val.real - ex(t->c[1]).val.real;
		// and so on
		case str:
			ret.val.str = t->val1.str; return ret;
		case real:
			ret.val.real = t->val1.real; return ret;
		case integer:
			ret.val.integer = t->val1.integer; return ret;
		case new_id:
			ret.val.id = t->val1.id; return ret;
		case cmpxlist_fun:
		case strlist_fun:
		case floatlist_fun:
		case intlist_fun:
		case cmpx_fun:
		case str_fun:
		case float_fun:
		case int_fun:
			// retrieve function, execute it, write result in ret.val.integer and return
		case cmpxlist_var:
		case strlist_var:
		case floatlist_var:
		case intlist_var:
		case cmpx_var:
		case str_var:
		case float_var:
		case int_var:
			// retrieve val and return it
			//ret.val.integer = *retrieve*; return ret;
		default:
			printf("Unsupported node type %d\n", t->type);
			break;
	}
	return ret;
}
