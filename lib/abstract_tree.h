#define MC 3

struct ast {
        int type;
        unsigned long natural;
        long int integer;
        long double real;
        double _Complex compx;
        char *str;
        char *id;
        struct ast*c[MC];
};

typedef struct ast ast_t;

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

int validate(ast_t *t);
int optimize(ast_t *t);
int execute(ast_t *t);

int validate(ast_t *t) { return 0; }
int optimize(ast_t *t) { return 0; }
int execute(ast_t *t) { return 0; }

