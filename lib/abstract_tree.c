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

int optimize(ast_t *t) { return 0; }

/* Variables don't need to be stored with type, type is controlled by parser */

int exec_env(ast_t *t) {
	ExecutionContext *context = create_execution_context();
	optimize(t);
	ex(t, context);	
	free_execution_context(context);
	return 0;
}

void *alloc_t(void *src, size_t size){
	void *ret = malloc(size);
	memcpy(ret, src, size);
	return ret;
}

ex_t ex(ast_t *t, ExecutionContext *e) { 
	ex_t ret = {.val.integer = 0};
	if(!t) return ret;

	switch (t->type) {
		case STMTS:
			return ex(t->c[0],e), ex(t->c[1],e);

		case INEQ:
			if(ex(t->c[0],e).val.integer==ex(t->c[1],e).val.integer) ret.val.integer++; 
			return ret; 
		case INNEQ:
			if(ex(t->c[0],e).val.integer!=ex(t->c[1],e).val.integer) ret.val.integer++; 
			return ret;
		case INGRT:
			if(ex(t->c[0],e).val.integer>ex(t->c[1],e).val.integer) ret.val.integer++; 
			return ret;
		case INMIN:
			if(ex(t->c[0],e).val.integer<ex(t->c[1],e).val.integer) ret.val.integer++; 
			return ret;
		case INGRTE:
			if(ex(t->c[0],e).val.integer>=ex(t->c[1],e).val.integer) ret.val.integer++; 
			return ret;
		case INMINE:
			if(ex(t->c[0],e).val.integer<=ex(t->c[1],e).val.integer) ret.val.integer++; 
			return ret;	
		case FNEQ:
			if(ex(t->c[0],e).val.real==ex(t->c[1],e).val.real) ret.val.integer++; 
			return ret; 
		case FNNEQ:
			if(ex(t->c[0],e).val.real!=ex(t->c[1],e).val.real) ret.val.integer++; 
			return ret;
		case FNGRT:
			if(ex(t->c[0],e).val.real>ex(t->c[1],e).val.real) ret.val.integer++; 
			return ret;
		case FNMIN:
			if(ex(t->c[0],e).val.real<ex(t->c[1],e).val.real) ret.val.integer++; 
			return ret;
		case FNGRTE:
			if(ex(t->c[0],e).val.real>=ex(t->c[1],e).val.real) ret.val.integer++; 
			return ret;
		case FNMINE:
			if(ex(t->c[0],e).val.real<=ex(t->c[1],e).val.real) ret.val.integer++; 
			return ret;
		case SEQ:
			if(!strcmp(ex(t->c[0],e).val.str,ex(t->c[1],e).val.str)) ret.val.integer++; 
			return ret;
		case SNEQ:
			if(strcmp(ex(t->c[0],e).val.str,ex(t->c[1],e).val.str)) ret.val.integer++; 
			return ret;
		case ILEQ:
			if(compare_int_lists(ex(t->c[0],e).val.ilist,ex(t->c[1],e).val.ilist)) ret.val.integer++; 
			return ret;
		case ILNEQ:
			if(!compare_int_lists(ex(t->c[0],e).val.ilist,ex(t->c[1],e).val.ilist)) ret.val.integer++; 
			return ret;
		case FLEQ:
			if(compare_double_lists(ex(t->c[0],e).val.flist,ex(t->c[1],e).val.flist)) ret.val.integer++; 
			return ret;
		case FLNEQ:
			if(!compare_double_lists(ex(t->c[0],e).val.flist,ex(t->c[1],e).val.flist)) ret.val.integer++; 
			return ret;
		case CLEQ:
			if(compare_complex_lists(ex(t->c[0],e).val.clist,ex(t->c[1],e).val.clist)) ret.val.integer++; 
			return ret;
		case CLNEQ:
			if(!compare_complex_lists(ex(t->c[0],e).val.clist,ex(t->c[1],e).val.clist)) ret.val.integer++; 
			return ret;
		case SLEQ:
			if(compare_string_lists(ex(t->c[0],e).val.slist,ex(t->c[1],e).val.slist)) ret.val.integer++; 
			return ret;
		case SLNEQ:
			if(!compare_string_lists(ex(t->c[0],e).val.slist,ex(t->c[1],e).val.slist)) ret.val.integer++; 
			return ret;

		case IWRITE:
			printf("%d", ex(t->c[0],e).val.integer); return ret;
		case FWRITE:
			printf("%lf", ex(t->c[0],e).val.real); return ret;
		case CWRITE:
			ret.val.cmpx = ex(t->c[0],e).val.cmpx;
			printf("%.2lf %.2lfi", creal(ret.val.cmpx), cimag(ret.val.cmpx)); return ret;
		case SWRITE:
			printf("%s", ex(t->c[0],e).val.str); return ret;
		case IREAD:
			scanf("%d", &ret.val.integer); return ret;	
		case FREAD:
			scanf("%lf", &ret.val.real); return ret;
		case SREAD:
			scanf("%255[^\n]", ret.val.str); return ret;
		case IF:
			if(ex(t->c[1],e).val.integer) ex(t->c[0],e);
			else ex(t->c[2],e); 
			return ret;
		case ELSE:
			ex(t->c[0],e); return ret;
		case FUNDEC:
			function_map_insert(e->function_map, t->val.id, t->c[0]); return ret;
		case FUN:
			ex(t->c[0],e), ex(t->c[1],e); return ret;
		case FPARAMS:
			variable_stack_insert(e->variable_stack, t->val.id, fcall_stack_pop(e->call_stack), false); 
			ex(t->c[1],e); return ret;
		case FPARAM:
			variable_stack_insert(e->variable_stack, t->val.id, fcall_stack_pop(e->call_stack), false); return ret;
		
		/* Solve argument type situation*/
		case SLFC:
    			fcall_stack_push(e->call_stack, NULL);
			push_scope(e->variable_stack);
			ret.val.slist=ex(function_map_get(e->function_map, t->val.id),e).val.slist;
			pop_scope(e->variable_stack); return ret;		
		case CLFC:
    			fcall_stack_push(e->call_stack, NULL);
			push_scope(e->variable_stack);
			ret.val.clist=ex(function_map_get(e->function_map, t->val.id),e).val.clist;
			pop_scope(e->variable_stack); return ret;
		case FLFC:
    			fcall_stack_push(e->call_stack, NULL);
			push_scope(e->variable_stack);
			ret.val.flist=ex(function_map_get(e->function_map, t->val.id),e).val.flist;
			pop_scope(e->variable_stack); return ret;
		case ILFC:
    			fcall_stack_push(e->call_stack, NULL);
			push_scope(e->variable_stack);
			ret.val.ilist=ex(function_map_get(e->function_map, t->val.id),e).val.ilist;
			pop_scope(e->variable_stack); return ret;
		case SFC:
    			fcall_stack_push(e->call_stack, NULL);
			push_scope(e->variable_stack);
			ret.val.str=ex(function_map_get(e->function_map, t->val.id),e).val.str;
			pop_scope(e->variable_stack); return ret;
		case CFC:
    			fcall_stack_push(e->call_stack, NULL);
			push_scope(e->variable_stack);
			ret.val.cmpx=ex(function_map_get(e->function_map, t->val.id),e).val.cmpx;
			pop_scope(e->variable_stack); return ret;
		case FFC:
    			fcall_stack_push(e->call_stack, NULL);
			push_scope(e->variable_stack);
			ret.val.real=ex(function_map_get(e->function_map, t->val.id),e).val.real;
			pop_scope(e->variable_stack); return ret;
		case IFC:
			push_scope(e->variable_stack);
			ret.val.integer=ex(function_map_get(e->function_map, t->val.id),e).val.integer;
			pop_scope(e->variable_stack); return ret;
		case FARGS:
			ex(t->c[0],e), ex(t->c[1],e);
		case IFARG:
			ret.val.integer = ex(t->c[0],e).val.integer;
			fcall_stack_push(e->call_stack, alloc_t(&ret.val.integer, sizeof(int)));	
		case FFARG:
			ret.val.real = ex(t->c[0],e).val.real;
			fcall_stack_push(e->call_stack, alloc_t(&ret.val.real, sizeof(double)));
		case CFARG:
			ret.val.cmpx = ex(t->c[0],e).val.cmpx;
			fcall_stack_push(e->call_stack, alloc_t(&ret.val.cmpx, sizeof(_Complex)));
		case SFARG:
			ret.val.str = ex(t->c[0],e).val.str;
			fcall_stack_push(e->call_stack, ret.val.str);
		case ILFARG:
			ret.val.ilist = ex(t->c[0],e).val.ilist;
			fcall_stack_push(e->call_stack, ret.val.ilist);
		case FLFARG:
			ret.val.flist = ex(t->c[0],e).val.flist;
			fcall_stack_push(e->call_stack, ret.val.flist);
		case CLFARG:
			ret.val.clist = ex(t->c[0],e).val.clist;
			fcall_stack_push(e->call_stack, ret.val.clist);
		case SLFARG:
			ret.val.slist = ex(t->c[0],e).val.slist;
			fcall_stack_push(e->call_stack, ret.val.slist);

		// no globals yet. keyword? automatic?
		case SLVDEC:
			variable_stack_insert(e->variable_stack, t->val.id, ex(t->c[0],e).val.slist, false); return ret;
		case CLVDEC:
			variable_stack_insert(e->variable_stack, t->val.id, ex(t->c[0],e).val.clist, false); return ret;
		case FLVDEC:
			variable_stack_insert(e->variable_stack, t->val.id, ex(t->c[0],e).val.flist, false); return ret;
		case ILVDEC:
			variable_stack_insert(e->variable_stack, t->val.id, ex(t->c[0],e).val.ilist, false); return ret;	
		case SVDEC:
			variable_stack_insert(e->variable_stack, t->val.id, ex(t->c[0],e).val.str, false); return ret;
		case CVDEC:
			ret.val.cmpx = ex(t->c[0],e).val.cmpx;
			_Complex *_c = malloc(sizeof(_Complex)); memcpy(_c, &ret.val.cmpx, sizeof(_Complex));
			variable_stack_insert(e->variable_stack, t->val.id, _c, false); return ret;
		case FVDEC:
			ret.val.real = ex(t->c[0],e).val.real;
			double *_d = malloc(sizeof(double)); memcpy(_d, &ret.val.real, sizeof(double));
			variable_stack_insert(e->variable_stack, t->val.id, _d, false); return ret;
		case IVDEC:
			ret.val.integer = ex(t->c[0],e).val.integer;
			int *_i = malloc(sizeof(int)); memcpy(_i, &ret.val.integer, sizeof(int));
			variable_stack_insert(e->variable_stack, t->val.id, _i, false); return ret;

		case ELIST:
			// AFTER left RECURSION SOLUTION	
		

		// ret.val.ilist = int_list_cloneadd(ex(t->c[0],e).val.ilist, ex(t->c[1],e).val.ilist);
		case ILADD: // this is append in reality
			ret.val.ilist = int_list_cloneappend(ex(t->c[0],e).val.ilist, ex(t->c[1],e).val.ilist); return ret;	
		case ILSLICE:
			ret.val.ilist = int_list_cloneslice(ex(t->c[0],e).val.ilist, ex(t->c[1],e).val.slice.a, ex(t->c[1],e).val.slice.b); return ret;
		case FLADD:
			ret.val.flist = double_list_cloneappend(ex(t->c[0],e).val.flist, ex(t->c[1],e).val.flist); return ret;	
		case FLSLICE:
			ret.val.flist = double_list_cloneslice(ex(t->c[0],e).val.flist, ex(t->c[1],e).val.slice.a, ex(t->c[1],e).val.slice.b); return ret;
 		case CLADD:
			ret.val.clist = complex_list_cloneappend(ex(t->c[0],e).val.clist, ex(t->c[1],e).val.clist); return ret;	
		case CLSLICE:
			ret.val.clist = complex_list_cloneslice(ex(t->c[0],e).val.clist, ex(t->c[1],e).val.slice.a, ex(t->c[1],e).val.slice.b); return ret;
		case SLADD:
			ret.val.slist = string_list_cloneappend(ex(t->c[0],e).val.slist, ex(t->c[1],e).val.slist); return ret;	
		case SLSLICE:
			ret.val.slist = string_list_cloneslice(ex(t->c[0],e).val.slist, ex(t->c[1],e).val.slice.a, ex(t->c[1],e).val.slice.b); return ret;

		case SADD:
			char *p1=ex(t->c[0],e).val.str,*p2=ex(t->c[1],e).val.str;
			ret.val.str = calloc(strlen(p1)+strlen(p2)+1, sizeof(char));
			strcat(ret.val.str, p1);	
			strcat(ret.val.str+strlen(p1), p2);
			return ret;
		case SSLICE:
			char *p=ex(t->c[0],e).val.str; slice_t s=ex(t->c[1],e).val.slice;
			if(strlen(p) < s.b) { printf("String index outbound: %d\n", t->type);
			break; }
			ret.val.str = calloc(s.b-s.a+1, sizeof(char));
			memcpy(ret.val.str, p+s.a, s.b-s.a);
		case SLICEI:
			ret.val.slice.a=ret.val.slice.b=ex(t->c[0],e).val.integer; return ret;
		case SLICE:
			ret.val.slice.a=ex(t->c[0],e).val.integer; ret.val.slice.b=ex(t->c[1],e).val.integer; return ret;
		case SLICER:
			ret.val.slice.b=0;ret.val.slice.b=ex(t->c[0],e).val.integer; return ret;

		case SLICEL:
			ret.val.slice.a=0;ret.val.slice.b=ex(t->c[0],e).val.integer; return ret;
			
		case ILISTE:
			/* AFTER RIGHT RECURSION IS REVISED.. */
			ret.val.ilist = create_int_list();
			t->c[0]->val.ilist = ret.val.ilist; ex(t->c[0],e); return ret;
		case ILISTS:
			/* AFTER RIGHT RECURSION IS REVISED.. */	
			
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
			ret.val.cmpx = ex(t->c[0],e).val.cmpx+ex(t->c[1],e).val.cmpx; return ret;
		case CMNS: 
			ret.val.cmpx = ex(t->c[0],e).val.cmpx-ex(t->c[1],e).val.cmpx; return ret;
		case CMUL: 
			ret.val.cmpx = ex(t->c[0],e).val.cmpx*ex(t->c[1],e).val.cmpx; return ret;
		case CDIV: 
			ret.val.cmpx = ex(t->c[0],e).val.cmpx/ex(t->c[1],e).val.cmpx; return ret; 
		case CNUMADD:
			ret.val.cmpx = ex(t->c[1],e).val.real+ex(t->c[0],e).val.real*I; return ret;
		case CNUMMNS:
			ret.val.cmpx = -ex(t->c[1],e).val.real+ex(t->c[0],e).val.real*I; return ret;
		case IMNS:
			ret.val.integer = ex(t->c[0],e).val.integer-ex(t->c[1],e).val.integer; return ret;
		case IADD:
			ret.val.integer = ex(t->c[0],e).val.integer+ex(t->c[1],e).val.integer; return ret;
		case IMUL:
			ret.val.integer = ex(t->c[0],e).val.integer*ex(t->c[1],e).val.integer; return ret;
		case IDIV:
			ret.val.integer = ex(t->c[0],e).val.integer/ex(t->c[1],e).val.integer; return ret;
		case IPOW:
			ret.val.integer = pow(ex(t->c[0],e).val.integer,ex(t->c[1],e).val.integer); return ret;
		case FMNS:
			ret.val.real = ex(t->c[0],e).val.real-ex(t->c[1],e).val.real; return ret;
		case FADD:
			ret.val.real = ex(t->c[0],e).val.real+ex(t->c[1],e).val.real; return ret;
		case FMUL:
			ret.val.real = ex(t->c[0],e).val.real*ex(t->c[1],e).val.real; return ret;
		case FDIV:
			ret.val.real = ex(t->c[0],e).val.real/ex(t->c[1],e).val.real; return ret;
		case FPOW:
			ret.val.real = pow(ex(t->c[0],e).val.real,ex(t->c[1],e).val.real); return ret;
		 return ret;
		case Str:
			ret.val.str = t->val.str; return ret;
		case Real:
			ret.val.real = t->val.real; return ret;
		case Integer:
			ret.val.integer = t->val.integer; return ret;
		
		case New_id:
		case Cmpxlist_fun:
		case Strlist_fun:
		case Floatlist_fun:
		case Intlist_fun:
		case Cmpx_fun:
		case Str_fun:
		case Float_fun:
		case Int_fun:
			ret.val.id = t->val.id; return ret;
		case Cmpxlist_var:
			ret.val.clist = (ComplexLinkedList *)variable_stack_get(e->variable_stack, t->val.id); return ret;
		case Strlist_var:
			ret.val.slist = (StringLinkedList *)variable_stack_get(e->variable_stack, t->val.id); return ret;
		case Floatlist_var:
			ret.val.flist = (DoubleLinkedList *)variable_stack_get(e->variable_stack, t->val.id); return ret;
		case Intlist_var:
			ret.val.ilist = (IntLinkedList *)variable_stack_get(e->variable_stack, t->val.id); return ret;
		case Cmpx_var:
			ret.val.cmpx = *( (_Complex *)variable_stack_get(e->variable_stack, t->val.id) ); return ret;
		case Str_var:
			ret.val.str = (char *)variable_stack_get(e->variable_stack, t->val.id); return ret;
		case Float_var:
			ret.val.real = *( (double *)variable_stack_get(e->variable_stack, t->val.id) ); return ret;
		case Int_var:
			ret.val.integer = *( (int *)variable_stack_get(e->variable_stack, t->val.id) ); return ret;
		default:
			printf("Unsupported node type %d\n", t->type);
			break;
	}
	return ret;
}
