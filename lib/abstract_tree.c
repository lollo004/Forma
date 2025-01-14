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
ex_t ex(ast_t *t) { 
	ex_t ret = {.val.integer = 0};
	if(!t) return ret;

	switch (t->type) {
		case STMTS:
			return ex(t->c[0]), ex(t->c[1]);
		
		case IWRITE:
			printf("%d", ex(t->c[0]).val.integer);
			return ret;
		case FWRITE:
			printf("%lf", ex(t->c[0]).val.real);
			return ret;
		case CWRITE:
			ret.val.cmpx = ex(t->c[0]).val.cmpx;
			printf("%.2lf %.2lfi", creal(ret.val.cmpx), cimag(ret.val.cmpx));	
			return ret;
		case SWRITE:
			printf("%s", ex(t->c[0]).val.str);
			return ret;
		case IREAD:
			scanf("%d", &ret.val.integer);
			return ret;	
		case FREAD:
			scanf("%lf", &ret.val.real);
			return ret;
		case SREAD:
			scanf("%255[^\n]", ret.val.str);
			return ret;
		case IF:
			if(ex(t->c[1]).val.integer) ex(t->c[0]);
			else ex(t->c[2]);
		case ELSE:
			ex(t->c[0]);
		case FUN:
			//add fun entry with address to this this block
			//fcall will have to vdec $1 and then execute $2
		case FPARAMS:
			// pop var on $1 but doing a special vdec
			// execute c[1]
		case FPARAM:
			// pop var on $1
		case SLFC:
			// do
		case CLFC:
			// do
		case FLFC:
			// do
		case ILFC:
			// do
		case SFC:
			// do
		case CFC:
			// do
		case FFC:
			// do
		case IFC:
			// enter new scope
			// push and alloc varvalues in stack (with voidpointer!)
			// add return address to HERE
			// goto function
			// ret.val.integer = ex_t($2);
		case SLVDEC:
			// call ivdec, pass name, type, value, (scope automatic)
	
		case CLVDEC:
			// call ivdec, pass name, type, and integer value
	
		case FLVDEC:
			// call ivdec, pass name, type, and integer value
	
		case ILVDEC:
			// call ivdec, pass name, type, and integer value
	
		case SVDEC:
			// call ivdec, pass name, type, and integer value
	
		case CVDEC:
			// call ivdec, pass name, type, and integer value
	
		case FVDEC:
		// call ivdec, pass name, type, and integer value
		
		case IVDEC:
			// call ivdec, pass name, type, and integer value
		case ELIST:
			// do	
		case ILADD:
			// do
		case ILSLICE:
			// do
		case FLADD:
			// do
		case FLSLICE:
			// do
 		case CLADD:
			// do
		case CLSLICE:
			// do
		case SLADD:
			// do
		case SLSLICE:
			// do		
		case SADD:
			char *p1=ex(t->c[0]).val.str,*p2=ex(t->c[1]).val.str;
			ret.val.str = calloc(strlen(p1)+strlen(p2)+1, sizeof(char));
			strcat(ret.val.str, p1);	
			strcat(ret.val.str+strlen(p1), p2);
			return ret;
		case SSLICE:
			char *p=ex(t->c[0]).val.str; slice_t s=ex(t->c[1]).val.slice;
			if(strlen(p) < s.b) { printf("String index outbound: %d\n", t->type);
			break; }
			ret.val.str = calloc(s.b-s.a+1, sizeof(char));
			memcpy(ret.val.str, p+s.a, s.b-s.a);
		case SLICEI:
			ret.val.slice.a=ret.val.slice.b=ex(t->c[0]).val.integer; return ret;
		case SLICE:
			ret.val.slice.a=ex(t->c[0]).val.integer; ret.val.slice.b=ex(t->c[1]).val.integer; return ret;
		case SLICER:
			ret.val.slice.b=0;ret.val.slice.b=ex(t->c[0]).val.integer; return ret;

		case SLICEL:
			ret.val.slice.a=0;ret.val.slice.b=ex(t->c[0]).val.integer; return ret;
		case ILISTE:
			// release_and_clear buffer and return list_pointer
		case ILISTS:
			// Append element ex(t->c[0]) 
		case ILIST:
			// Append element [0]
		case FLISTE:
			// release_and_clear buffer and return list_pointer
		case FLISTS:
			// Append element ex(t->c[0]) 
		case FLIST:
			// Append element [0]
		case CLISTE:
			// release_and_clear buffer and return list_pointer
		case CLISTS:
			// Append element ex(t->c[0]) 
		case CLIST:
			// Append element [0]
		case SLISTE:
			// release_and_clear buffer and return list_pointer
		case SLISTS:
			// Append element ex(t->c[0]) 
		case SLIST:
			// Append element [0]
		case CADD: 
			ret.val.cmpx = ex(t->c[0]).val.cmpx+ex(t->c[1]).val.cmpx; return ret;
		case CMNS: 
			ret.val.cmpx = ex(t->c[0]).val.cmpx-ex(t->c[1]).val.cmpx; return ret;
		case CMUL: 
			ret.val.cmpx = ex(t->c[0]).val.cmpx*ex(t->c[1]).val.cmpx; return ret;
		case CDIV: 
			ret.val.cmpx = ex(t->c[0]).val.cmpx/ex(t->c[1]).val.cmpx; return ret; 
		case CNUMADD:
			ret.val.cmpx = ex(t->c[1]).val.real+ex(t->c[0]).val.real*I; return ret;
		case CNUMMNS:
			ret.val.cmpx = -ex(t->c[1]).val.real+ex(t->c[0]).val.real*I; return ret;
		case IMNS:
			ret.val.integer = ex(t->c[0]).val.integer-ex(t->c[1]).val.integer; return ret;
		case IADD:
			ret.val.integer = ex(t->c[0]).val.integer+ex(t->c[1]).val.integer; return ret;
		case IMUL:
			ret.val.integer = ex(t->c[0]).val.integer*ex(t->c[1]).val.integer; return ret;
		case IDIV:
			ret.val.integer = ex(t->c[0]).val.integer/ex(t->c[1]).val.integer; return ret;
		case IPOW:
			ret.val.integer = pow(ex(t->c[0]).val.integer,ex(t->c[1]).val.integer); return ret;
		case FMNS:
			ret.val.real = ex(t->c[0]).val.real-ex(t->c[1]).val.real; return ret;
		case FADD:
			ret.val.real = ex(t->c[0]).val.real+ex(t->c[1]).val.real; return ret;
		case FMUL:
			ret.val.real = ex(t->c[0]).val.real*ex(t->c[1]).val.real; return ret;
		case FDIV:
			ret.val.real = ex(t->c[0]).val.real/ex(t->c[1]).val.real; return ret;
		case FPOW:
			ret.val.real = pow(ex(t->c[0]).val.real,ex(t->c[1]).val.real); return ret;
		case IEQ:
			// return an integer
		 return ret;
		case Str:
			ret.val.str = t->val1.str; return ret;
		case Real:
			ret.val.real = t->val1.real; return ret;
		case Integer:
			ret.val.integer = t->val1.integer; return ret;
		case New_id:
			ret.val.id = t->val1.id; return ret;
		case Cmpxlist_fun:
		case Strlist_fun:
		case Floatlist_fun:
		case Intlist_fun:
		case Cmpx_fun:
		case Str_fun:
		case Float_fun:
		case Int_fun:
			ret.val.id = t->val1.id; return ret;
			// then fcall will make use of this
		case Cmpxlist_var:
		case Strlist_var:
		case Floatlist_var:
		case Intlist_var:
		case Cmpx_var:
		case Str_var:
		case Float_var:
		case Int_var:
			// retrieve val and return it
			//ret.val.integer = *retrieve*; return ret;
		default:
			printf("Unsupported node type %d\n", t->type);
			break;
	}
	return ret;
}
