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

void print_ast(ast_t *t, int deep, const char *prefix, int is_last) {
    if (!t) return;

    // Stampa il nodo corrente con il prefisso corretto
    printf("%s%s-- Node Type: %d\n", prefix, is_last ? "└" : "├", t->type);

    // Costruisce il nuovo prefisso
    char new_prefix[256];
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_last ? "    " : "│   ");

    // Conta il numero effettivo di figli
    int count = 0;
    for (int i = 0; i < MC; i++) {
        if (t->c[i]) count++;
    }

    // Stampa i figli con il prefisso corretto
    int child_index = 0;
    for (int i = 0; i < MC; i++) {
        if (t->c[i]) {
            print_ast(t->c[i], deep + 1, new_prefix, ++child_index == count);
        }
    }
}
/*
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
*/
int optimize(ast_t *t) {
	if(!t) return 0;
	switch(t->type){
		case IF:
			if(t->c[1]->type != Integer) break;
			ast_t *node;
			if(t->c[1]->val.integer) {
				node = t->c[0];
				// clear all t->c[2] tree
			}
			else {
				node = t->c[2];
				// clear all t->c[0] tree
 			}
 			free(t->c[1]);
			t->type = STMTS;
			t->c[0] = node;
			t->c[1] = NULL;
			t->c[2] = NULL;
			return 1;
		
		case SEQ:
			if (t->c[0]->type == Str_var && t->c[1]->type == Str_var) {
				if(strcmp(t->c[0]->val.id, t->c[1]->val.id)) break;
				t->type = Integer;
				t->val.integer = 1;
				free(t->c[0]->val.id), free(t->c[1]->val.id);
				free(t->c[0]), free(t->c[1]);
				t->c[0] = t->c[1] = NULL;
				return 1;
			}
			else if(t->c[0]->type != Str || t->c[1]->type != Str) break;
			t->type = Integer;
			t->val.integer = !strcmp(t->c[0]->val.str, t->c[1]->val.str);
			free(t->c[0]->val.str), free(t->c[1]->val.str);
			free(t->c[0]), free(t->c[1]);
			t->c[0] = t->c[1] = NULL;
			return 1; 
		case SNEQ: 
			if (t->c[0]->type == Str_var && t->c[1]->type == Str_var) {
				if(strcmp(t->c[0]->val.id, t->c[1]->val.id)) break;
				t->type = Integer;
				t->val.integer = 0;
				free(t->c[0]->val.id), free(t->c[1]->val.id);
				free(t->c[0]), free(t->c[1]);
				t->c[0] = t->c[1] = NULL;
				return 1;
			}
			else if(t->c[0]->type != Str || t->c[1]->type != Str) break;
			t->type = Integer;
			t->val.integer = strcmp(t->c[0]->val.str, t->c[1]->val.str);
			free(t->c[0]->val.str), free(t->c[1]->val.str);
			free(t->c[0]), free(t->c[1]);
			t->c[0] = t->c[1] = NULL;
			return 1; 

		case CEQ:
			if (t->c[0]->type == Cmpx_var && t->c[1]->type == Cmpx_var) {
				if(strcmp(t->c[0]->val.id, t->c[1]->val.id)) break;
				t->type = Integer;
				t->val.integer = 1;
				free(t->c[0]->val.id), free(t->c[1]->val.id);
				free(t->c[0]), free(t->c[1]);
				t->c[0] = t->c[1] = NULL;
				return 1;
			}			
		case CNEQ:
			if (t->c[0]->type == Cmpx_var && t->c[1]->type == Cmpx_var) {
				if(strcmp(t->c[0]->val.id, t->c[1]->val.id)) break;
				t->type = Integer;
				t->val.integer = 0;
				free(t->c[0]->val.id), free(t->c[1]->val.id);
				free(t->c[0]), free(t->c[1]);
				t->c[0] = t->c[1] = NULL;
				return 1;
			}
			
		case FNEQ:
			if (t->c[0]->type == Float_var && t->c[1]->type == Float_var) {
				if(strcmp(t->c[0]->val.id, t->c[1]->val.id)) break;
				t->type = Integer;
				t->val.integer = 1;
				free(t->c[0]->val.id), free(t->c[1]->val.id);
				free(t->c[0]), free(t->c[1]);
				t->c[0] = t->c[1] = NULL;
				return 1;
			}
			else if(t->c[0]->type != Real || t->c[1]->type != Real) break;
			t->type = Integer;
			t->val.integer = t->c[0]->val.integer == t->c[1]->val.integer;
			free(t->c[0]), free(t->c[1]);
			t->c[0] = t->c[1] = NULL;
			return 1; 
		case FNNEQ:
			if (t->c[0]->type == Float_var && t->c[1]->type == Float_var) {
				if(strcmp(t->c[0]->val.id, t->c[1]->val.id)) break;
				t->type = Integer;
				t->val.integer = 0;
				free(t->c[0]->val.id), free(t->c[1]->val.id);
				free(t->c[0]), free(t->c[1]);
				t->c[0] = t->c[1] = NULL;
				return 1;
			}
			else if(t->c[0]->type != Real || t->c[1]->type != Real) break;
			t->type = Integer;
			t->val.integer = t->c[0]->val.integer != t->c[1]->val.integer;
			free(t->c[0]), free(t->c[1]);
			t->c[0] = t->c[1] = NULL;
			return 1; 

		case INEQ:
			if (t->c[0]->type == Int_var && t->c[1]->type == Int_var) {
				if(strcmp(t->c[0]->val.id, t->c[1]->val.id)) break;
				t->type = Integer;
				t->val.integer = 1;
				free(t->c[0]->val.id), free(t->c[1]->val.id);
				free(t->c[0]), free(t->c[1]);
				t->c[0] = t->c[1] = NULL;
				return 1;
			}
			else if(t->c[0]->type != Integer || t->c[1]->type != Integer) break;
			t->type = Integer;
			t->val.integer = t->c[0]->val.integer == t->c[1]->val.integer;
			free(t->c[0]), free(t->c[1]);
			t->c[0] = t->c[1] = NULL;
			return 1; 
		case INNEQ:
			if (t->c[0]->type == Int_var && t->c[1]->type == Int_var) {
				if(strcmp(t->c[0]->val.id, t->c[1]->val.id)) break;
				t->type = Integer;
				t->val.integer = 0;
				free(t->c[0]->val.id), free(t->c[1]->val.id);
				free(t->c[0]), free(t->c[1]);
				t->c[0] = t->c[1] = NULL;
				return 1;
			}
			else if(t->c[0]->type != Integer || t->c[1]->type != Integer) break;
			t->type = Integer;
			t->val.integer = t->c[0]->val.integer != t->c[1]->val.integer;
			free(t->c[0]), free(t->c[1]);
			t->c[0] = t->c[1] = NULL;
			return 1; 


		case SADD:
			if(t->c[0]->type != Str || t->c[1]->type != Str) break;
			t->type = Str;
			t->val.str = calloc(strlen(t->c[0]->val.str)+strlen(t->c[1]->val.str)+1, sizeof(char));
			strcat(t->val.str, t->c[0]->val.str);	
			strcat(t->val.str+strlen(t->c[0]->val.str), t->c[1]->val.str);
			free(t->c[0]->val.str); free(t->c[1]->val.str); 
			free(t->c[0]); free(t->c[1]);
			t->c[0] = NULL, t->c[1] = NULL;
			return 1;		
		case FDIV:
			if(t->c[0]->type != Real || t->c[1]->type != Real) break;
			t->type = Real;
			t->val.real = t->c[0]->val.real / t->c[1]->val.real;
			free(t->c[0]), free(t->c[1]);
			t->c[0] = t->c[1] = NULL;
			return 1;		
		case FMUL:
			if(t->c[0]->type != Real || t->c[1]->type != Real) break;
			t->type = Real;
			t->val.real = t->c[0]->val.real * t->c[1]->val.real;
			free(t->c[0]), free(t->c[1]);
			t->c[0] = t->c[1] = NULL;
			return 1;		
		case FMNS:
			if(t->c[0]->type != Real || t->c[1]->type != Real) break;
			t->type = Real;
			t->val.real = t->c[0]->val.real - t->c[1]->val.real;
			free(t->c[0]), free(t->c[1]);
			t->c[0] = t->c[1] = NULL;
			return 1;	
		case FADD:
			if(t->c[0]->type != Real || t->c[1]->type != Real) break;
			t->type = Real;
			t->val.real = t->c[0]->val.real + t->c[1]->val.real;
			free(t->c[0]), free(t->c[1]);
			t->c[0] = t->c[1] = NULL;
			return 1;	
		case IDIV:
			if(t->c[0]->type != Integer || t->c[1]->type != Integer) break;
			t->type = Integer;
			t->val.integer = t->c[0]->val.integer / t->c[1]->val.integer;
			free(t->c[0]), free(t->c[1]);
			t->c[0] = t->c[1] = NULL;
			return 1;		
		case IMUL:
			if(t->c[0]->type != Integer || t->c[1]->type != Integer) break;
			t->type = Integer;
			t->val.integer = t->c[0]->val.integer * t->c[1]->val.integer;
			free(t->c[0]), free(t->c[1]);
			t->c[0] = t->c[1] = NULL;
			return 1;		
		case IMNS:
			if(t->c[0]->type != Integer || t->c[1]->type != Integer) break;
			t->type = Integer;
			t->val.integer = t->c[0]->val.integer - t->c[1]->val.integer;
			free(t->c[0]), free(t->c[1]);
			t->c[0] = t->c[1] = NULL;
			return 1;	
		case IADD:
			if(t->c[0]->type != Integer || t->c[1]->type != Integer) break;
			t->type = Integer;
			t->val.integer = t->c[0]->val.integer + t->c[1]->val.integer;
			free(t->c[0]), free(t->c[1]);
			t->c[0] = t->c[1] = NULL;
			return 1;
	}
	return optimize(t->c[0]) + optimize(t->c[1]) + optimize(t->c[2]); 
}

int exec_env(ast_t *t) {
	// print_ast(t, 0, "", 0);
	while (optimize(t));
	printf("\r");
	ExecutionContext *context = create_execution_context();
	push_scope(context->variable_stack);	
	ex(t, context);	
	pop_scope(context->variable_stack);
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
	if(!t || is_returning(e)) return ret;
	switch (t->type) {
		case STMTS:
			return ex(t->c[0],e), ex(t->c[1],e);

		case RANDOM:
			srand(time(NULL));
			ret.val.integer = ex(t->c[0],e).val.integer + rand() % ex(t->c[1],e).val.integer;
			return ret;
	
		case IRET:
			ret.val.integer = ex(t->c[0],e).val.integer;
    			set_return_state(e, alloc_t(&ret.val.integer, sizeof(int)));	
			return ret;
		case FRET:
			ret.val.real = ex(t->c[0],e).val.real;
    			set_return_state(e, alloc_t(&ret.val.real, sizeof(double)));	
			return ret;
		case CRET:
			ret.val.cmpx = ex(t->c[0],e).val.cmpx;
    			set_return_state(e, alloc_t(&ret.val.cmpx, sizeof(_Complex)));	
			return ret;
		case SRET:
    			set_return_state(e, ex(t->c[0],e).val.str);	
			return ret;
		case ILRET:
    			set_return_state(e, ex(t->c[0],e).val.ilist);	
			return ret;
		case FLRET:
    			set_return_state(e, ex(t->c[0],e).val.flist);	
			return ret;
		case CLRET:
    			set_return_state(e, ex(t->c[0],e).val.clist);	
			return ret;
		case SLRET:
    			set_return_state(e, ex(t->c[0],e).val.slist);	
			return ret;
	
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
			printf("%.2lf%+.2lfi", creal(ret.val.cmpx), cimag(ret.val.cmpx)); return ret;
		case SWRITE:
			printf("%s", ex(t->c[0],e).val.str); return ret;

		case IREAD:
			scanf("%d", &ret.val.integer); 
			ret.val.any = alloc_t(&ret.val.integer, sizeof(int));
			return ret;
		case FREAD:
			scanf("%lf", &ret.val.real);
			ret.val.any = alloc_t(&ret.val.real, sizeof(double));
			return ret;
		case SREAD:
			char _s[255] = {0};
			if (fgets(_s, sizeof(_s), stdin) != NULL) {
				_s[strcspn(_s, "\n")] = '\0';
				ret.val.any = strdup(_s);
			} else ret.val.any = strdup("");
			return ret;
		case IF:
			if(ex(t->c[1],e).val.integer) ex(t->c[0],e);
			else ex(t->c[2],e); 
			return ret;
		
		case FUNDEC:
			function_map_insert(e->function_map, t->val.id, t->c[0]); 
			return ret;
		case FUN:
			ex(t->c[0],e), ex(t->c[1],e); 
			return ret;
		case FPARAM:
			variable_stack_insert(e->variable_stack, t->val.id, fcall_stack_pop(e->call_stack), false); 
			ex(t->c[0],e); 
			return ret;
		
		/* Solve argument type situation*/
		case SLFC:
			ex(t->c[0],e);
			push_scope(e->variable_stack);
			ex(function_map_get(e->function_map, t->val.id),e);
			pop_scope(e->variable_stack); 
			ret.val.slist = get_return_value(e);
			reset_return_state(e);
			return ret;		
		case CLFC:
			ex(t->c[0],e);
			push_scope(e->variable_stack);
			ex(function_map_get(e->function_map, t->val.id),e);
			pop_scope(e->variable_stack); 
			ret.val.clist = get_return_value(e);
			reset_return_state(e);
			return ret;
		case FLFC:
    			ex(t->c[0],e);
			push_scope(e->variable_stack);
			ex(function_map_get(e->function_map, t->val.id),e);
			pop_scope(e->variable_stack); 
			ret.val.flist = get_return_value(e);
			reset_return_state(e);
			return ret;
		case ILFC:
    			ex(t->c[0],e);
			push_scope(e->variable_stack);
			ex(function_map_get(e->function_map, t->val.id),e);
			pop_scope(e->variable_stack); 
			ret.val.ilist = get_return_value(e);
			reset_return_state(e);
			return ret;
		case SFC:
			ex(t->c[0],e);
			push_scope(e->variable_stack);
			ex(function_map_get(e->function_map, t->val.id),e);
			pop_scope(e->variable_stack);
			ret.val.str = (char *)get_return_value(e);
			reset_return_state(e);
			return ret;
		case CFC:
    			ex(t->c[0],e);
			push_scope(e->variable_stack);
			ex(function_map_get(e->function_map, t->val.id),e);
			pop_scope(e->variable_stack); 
			ret.val.cmpx = *(_Complex *)get_return_value(e);
			reset_return_state(e);
			return ret;
		case FFC:
			ex(t->c[0],e);
			push_scope(e->variable_stack);
			ex(function_map_get(e->function_map, t->val.id),e);
			pop_scope(e->variable_stack); 
			ret.val.real = *(double *)get_return_value(e);
			reset_return_state(e);
			return ret;
		case IFC:
			ex(t->c[0],e);
			push_scope(e->variable_stack);
			ex(function_map_get(e->function_map, t->val.id),e);
			pop_scope(e->variable_stack);
			ret.val.integer = *(int *)get_return_value(e);
			reset_return_state(e);
			return ret;
		case FARGS:
			ex(t->c[0],e), ex(t->c[1],e);
			return ret;
		case IFARG:
			ret.val.integer = ex(t->c[0],e).val.integer;
			fcall_stack_push(e->call_stack, alloc_t(&ret.val.integer, sizeof(int)));	
			return ret;
		case FFARG:
			ret.val.real = ex(t->c[0],e).val.real;
			fcall_stack_push(e->call_stack, alloc_t(&ret.val.real, sizeof(double)));
			return ret;
		case CFARG:
			ret.val.cmpx = ex(t->c[0],e).val.cmpx;
			fcall_stack_push(e->call_stack, alloc_t(&ret.val.cmpx, sizeof(_Complex)));
			return ret;
		case SFARG:
			ret.val.str = ex(t->c[0],e).val.str;
			fcall_stack_push(e->call_stack, ret.val.str);
			return ret;
		case ILFARG:
			ret.val.ilist = ex(t->c[0],e).val.ilist;
			fcall_stack_push(e->call_stack, ret.val.ilist);
			return ret;
		case FLFARG:
			ret.val.flist = ex(t->c[0],e).val.flist;
			fcall_stack_push(e->call_stack, ret.val.flist);
			return ret;
		case CLFARG:
			ret.val.clist = ex(t->c[0],e).val.clist;
			fcall_stack_push(e->call_stack, ret.val.clist);
			return ret;
		case SLFARG:
			ret.val.slist = ex(t->c[0],e).val.slist;
			fcall_stack_push(e->call_stack, ret.val.slist);
			return ret;

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
			variable_stack_insert(e->variable_stack, t->val.id, alloc_t(&ret.val.cmpx, sizeof(_Complex)), false); return ret;
		case FVDEC:
			ret.val.real = ex(t->c[0],e).val.real;
			variable_stack_insert(e->variable_stack, t->val.id, alloc_t(&ret.val.real, sizeof(double)), false);
			return ret;	
		case IVDEC:
			ret.val.integer = ex(t->c[0],e).val.integer;
			variable_stack_insert(e->variable_stack, t->val.id, alloc_t(&ret.val.integer, sizeof(int)), false); return ret;
		case RVDEC:
			variable_stack_insert(e->variable_stack, t->val.id, ex(t->c[0],e).val.any, false); return ret;

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
		
		case ILISTE:
			ret.val.ilist = create_int_list();
			if(!t->c[0]) return ret;
			t->c[0]->val.any = ret.val.ilist; 
			ex(t->c[0],e); 
			return ret;
		case FLISTE:
			ret.val.flist = create_double_list();
			if(!t->c[0]) return ret;
			t->c[0]->val.flist = ret.val.flist; ex(t->c[0],e); 
			return ret;
		case CLISTE:
			ret.val.clist = create_complex_list();
			if(!t->c[0]) return ret;
			t->c[0]->val.clist = ret.val.clist; ex(t->c[0],e); 
			return ret;
		case SLISTE:
			ret.val.any = create_string_list();
			if(!t->c[0]) return ret;
			t->c[0]->val.any = ret.val.any; 
			ex(t->c[0],e);
			void *_t = ret.val.any;
			ret.val.slist = _t;
			return ret;
		case LISTS:
			t->c[0]->val.any = t->val.any; t->c[1]->val.any = t->val.any;
			ex(t->c[0],e); 
			ex(t->c[1],e);
			return ret;
		case ILIST:
			int_list_add(t->val.any, ex(t->c[0],e).val.integer);
			return ret;
		case FLIST:
			double_list_add(t->val.any, ex(t->c[0],e).val.real);
			return ret;
		case CLIST:
			complex_list_add(t->val.any, ex(t->c[0],e).val.cmpx);
			return ret;
		case SLIST:
			string_list_add(t->val.any, ex(t->c[0],e).val.str);
			return ret;

		case SADD:
			char *p1=ex(t->c[0],e).val.str,*p2=ex(t->c[1],e).val.str;
			ret.val.str = calloc(strlen(p1)+strlen(p2)+1, sizeof(char));
			strcat(ret.val.str, p1);	
			strcat(ret.val.str+strlen(p1), p2);
			return ret;
		/*case SSLICE:
			char *p=ex(t->c[0],e).val.str; slice_t s=ex(t->c[1],e).val.slice;
			if(s.b == -1) s.b = strlen(p)-1;	
			else if(strlen(p) <= s.b) { printf("String index outbound: %d\n", t->type); break; }
			ret.val.str = calloc(s.b-s.a+2, sizeof(char));
			memcpy(ret.val.str, p+(s.a), (s.b-s.a) * sizeof(char));
			return ret;
		*/
case SSLICE: {
    char *p = ex(t->c[0], e).val.str;
    if (!p) {
        printf("Null string pointer encountered.\n");
        break;
    }

    slice_t s = ex(t->c[1], e).val.slice;
    size_t len = strlen(p);

    // Se s.b è -1, assegnalo all'ultimo indice della stringa
    if (s.b == -1) s.b = len - 1;

    // Se s.a è fuori dai limiti, impostalo al limite massimo valido
    if (s.a < 0) s.a = 0;
    if (s.a > len) s.a = len;

    // Se s.b è fuori dai limiti, impostalo al limite massimo valido
    if (s.b < 0) s.b = 0;
    if (s.b > len) s.b = len;

    // Se s.b < s.a, restituisci una stringa vuota
    if (s.b < s.a) {
        ret.val.str = calloc(1, sizeof(char)); // Stringa vuota terminata
        return ret;
    }

    // Calcola la lunghezza dello slice e alloca memoria
    size_t slice_len = s.b - s.a + 1;
    ret.val.str = calloc(slice_len + 1, sizeof(char)); // +1 per il terminatore nullo
    if (!ret.val.str) {
        printf("Memory allocation failed.\n");
        break;
    }

    // Copia i caratteri dello slice nella nuova stringa
    memcpy(ret.val.str, p + s.a, slice_len);
    ret.val.str[slice_len] = '\0'; // Aggiungi il terminatore nullo
    return ret;
}
		case SLICE:
			ret.val.slice.a=ex(t->c[0],e).val.integer; ret.val.slice.b=ex(t->c[1],e).val.integer; 
			return ret;
		case SLICER:
			ret.val.slice.b=-1;ret.val.slice.a=ex(t->c[0],e).val.integer; 
			return ret;
		case SLICEL:
			ret.val.slice.a=0;ret.val.slice.b=ex(t->c[0],e).val.integer; 
			return ret;
		case SLICEI:
			ret.val.slice.a=ret.val.slice.b=ex(t->c[0],e).val.integer;
			return ret;
	
		case ISLICEI:
			ret.val.integer = *(int *)int_list_get_reference(ex(t->c[0],e).val.ilist, ex(t->c[1],e).val.integer);
			return ret;
		case FSLICEI:
			ret.val.real = *(double *)double_list_get_reference(ex(t->c[0],e).val.flist, ex(t->c[1],e).val.integer);
			return ret;
		case CSLICEI:
			ret.val.cmpx = *(_Complex *)complex_list_get_reference(ex(t->c[0],e).val.clist, ex(t->c[1],e).val.integer);
			return ret;
		case SSLICEI:
			ret.val.str = (char *)string_list_get_copy(ex(t->c[0],e).val.slist, ex(t->c[1],e).val.integer);
			return ret;		
					
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
