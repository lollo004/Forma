#include <stack_t.h>

#define HASHMAP_SIZE 128
#define FCALL_STACK_SIZE 256

// Hash functions
unsigned int hash(const char *key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash * 31) + *key++;
    }
    return hash % HASHMAP_SIZE;
}

// FunctionMap functions
FunctionMap *create_function_map() {
    FunctionMap *map = malloc(sizeof(FunctionMap));
    memset(map->buckets, 0, sizeof(map->buckets));
    return map;
}

void function_map_insert(FunctionMap *map, const char *name, ast_t *node) {
    unsigned int index = hash(name);
    FunctionEntry *entry = malloc(sizeof(FunctionEntry));
    entry->name = strdup(name);
    entry->ast_node = node;
    entry->next = map->buckets[index];
    map->buckets[index] = entry;
}

ast_t *function_map_get(FunctionMap *map, const char *name) {
    unsigned int index = hash(name);
    FunctionEntry *entry = map->buckets[index];
    while (entry) {
        if (strcmp(entry->name, name) == 0) {
            return entry->ast_node;
        }
        entry = entry->next;
    }
    return NULL;
}

// VariableMap functions
VariableMap *create_variable_map() {
    VariableMap *map = malloc(sizeof(VariableMap));
    memset(map->buckets, 0, sizeof(map->buckets));
    return map;
}

void variable_map_insert(VariableMap *map, const char *name, void *value) {
    unsigned int index = hash(name);
    VariableEntry *entry = malloc(sizeof(VariableEntry));
    entry->name = strdup(name);
    entry->value = value;
    entry->next = map->buckets[index];
    map->buckets[index] = entry;
}

void *variable_map_get(VariableMap *map, const char *name) {
    unsigned int index = hash(name);
    VariableEntry *entry = map->buckets[index];
    while (entry) {
        if (strcmp(entry->name, name) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

// VariableStack functions
VariableStack *create_variable_stack() {
    VariableStack *stack = malloc(sizeof(VariableStack));
    stack->top = NULL;
    stack->global_variables = create_variable_map();
    return stack;
}

void push_scope(VariableStack *stack) {
    VariableScope *scope = malloc(sizeof(VariableScope));
    scope->variables = create_variable_map();
    scope->next = stack->top;
    stack->top = scope;
}

void pop_scope(VariableStack *stack) {
    if (stack->top) {
        VariableScope *top = stack->top;
        stack->top = top->next;

        // Free the variable map and entries
        for (int i = 0; i < HASHMAP_SIZE; i++) {
            VariableEntry *entry = top->variables->buckets[i];
            while (entry) {
                VariableEntry *next = entry->next;
                free(entry->name);
                free(entry);
                entry = next;
            }
        }
        free(top->variables);
        free(top);
    }
}

void variable_stack_insert(VariableStack *stack, const char *name, void *value, bool is_global) {
    if (is_global) {
        variable_map_insert(stack->global_variables, name, value); // Inserisce nei globali
    } else if (stack->top) {
        variable_map_insert(stack->top->variables, name, value);   // Inserisce nello scope locale
    }
}

void *variable_stack_get(VariableStack *stack, const char *name) {
    VariableScope *scope = stack->top;
    while (scope) {
        void *value = variable_map_get(scope->variables, name);
        if (value) {
            return value;
        }
        scope = scope->next;
    }
    // Se non trovato, controlla nei globali
    return variable_map_get(stack->global_variables, name);
}

// FCallStack functions
FCallStack *create_fcall_stack(int capacity) {
    FCallStack *stack = malloc(sizeof(FCallStack));
    stack->data = malloc(sizeof(void *) * capacity);
    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

void fcall_stack_push(FCallStack *stack, void *value) {
    if (stack->top + 1 >= stack->capacity) {
        fprintf(stderr, "FCallStack overflow\n");
        exit(EXIT_FAILURE);
    }
    stack->data[++stack->top] = value;
}

void *fcall_stack_pop(FCallStack *stack) {
    if (stack->top < 0) {
        fprintf(stderr, "FCallStack underflow\n");
        exit(EXIT_FAILURE);
    }
    return stack->data[stack->top--];
}

void free_fcall_stack(FCallStack *stack) {
    free(stack->data);
    free(stack);
}

ExecutionContext *create_execution_context() {
    ExecutionContext *context = malloc(sizeof(ExecutionContext));
    context->variable_stack = create_variable_stack();
    context->function_map = create_function_map();
    context->call_stack = create_fcall_stack(FCALL_STACK_SIZE);
    return context;
}

void free_execution_context(ExecutionContext *context) {
    // Free stacks and maps
    free(context->variable_stack);
    free(context->function_map);
    free(context);
}


/*
int main() {
    VariableStack *variable_stack = create_variable_stack();

    // Variabile globale
    int *global_value = malloc(sizeof(int));
    *global_value = 999;
    variable_stack_insert(variable_stack, "global_var", global_value, true);

    // Scope locale
    push_scope(variable_stack);
    int *local_value = malloc(sizeof(int));
    *local_value = 42;
    variable_stack_insert(variable_stack, "local_var", local_value, false);

    // Accesso a variabili
    int *retrieved_local = (int *)variable_stack_get(variable_stack, "local_var");
    int *retrieved_global = (int *)variable_stack_get(variable_stack, "global_var");

    printf("Local variable: %d\\n", retrieved_local ? *retrieved_local : -1);
    printf("Global variable: %d\\n", retrieved_global ? *retrieved_global : -1);

    pop_scope(variable_stack);

    // Accesso dopo aver chiuso lo scope
    retrieved_local = (int *)variable_stack_get(variable_stack, "local_var");
    retrieved_global = (int *)variable_stack_get(variable_stack, "global_var");

    printf("Local variable after scope pop: %s\\n", retrieved_local ? "Found" : "Not Found");
    printf("Global variable remains: %d\\n", retrieved_global ? *retrieved_global : -1);

    // Cleanup
    free(global_value);
    free(local_value);
    free(variable_stack);
    return 0;
}
*/
