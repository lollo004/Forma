#ifndef VARIABLE_STACK_H
#define VARIABLE_STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <abstract_tree.h>

#define HASHMAP_SIZE 128

typedef struct ast ast_t;

// Function map entry structure
typedef struct FunctionEntry {
    char *name;
    ast_t *ast_node;
    struct FunctionEntry *next;
} FunctionEntry;

// Variable map entry structure
typedef struct VariableEntry {
    char *name;
    void *value;
    struct VariableEntry *next;
} VariableEntry;

// Hashmap for functions
typedef struct FunctionMap {
    FunctionEntry *buckets[HASHMAP_SIZE];
} FunctionMap;

// Hashmap for variables
typedef struct VariableMap {
    VariableEntry *buckets[HASHMAP_SIZE];
} VariableMap;

// Scope for variables
typedef struct VariableScope {
    VariableMap *variables;
    struct VariableScope *next; 
} VariableScope;

// Stack for variable scopes
typedef struct VariableStack {
    VariableScope *top;
    VariableMap *global_variables; // Global variables accessible everywhere
} VariableStack;

// Stack for function call arguments
typedef struct FCallStack {
    void **data;
    int top;
    int capacity;
} FCallStack;

// Function prototypes

// Hash function
unsigned int hash(const char *key);

// FunctionMap management
FunctionMap *create_function_map();
void function_map_insert(FunctionMap *map, const char *name, ast_t *node);
ast_t *function_map_get(FunctionMap *map, const char *name);

// VariableMap management
VariableMap *create_variable_map();
void variable_map_insert(VariableMap *map, const char *name, void *value);
void *variable_map_get(VariableMap *map, const char *name);

// VariableStack management
VariableStack *create_variable_stack();
void push_scope(VariableStack *stack);
void pop_scope(VariableStack *stack);
void variable_stack_insert(VariableStack *stack, const char *name, void *value, bool is_global);
void *variable_stack_get(VariableStack *stack, const char *name);

// FCallStack management
FCallStack *create_fcall_stack(int capacity);
void fcall_stack_push(FCallStack *stack, void *value);
void *fcall_stack_pop(FCallStack *stack);
void free_fcall_stack(FCallStack *stack);

typedef struct ReturnState {
    bool is_returning;
    void *return_value;
} ReturnState;

typedef struct ExecutionContext {
    VariableStack *variable_stack;
    FunctionMap *function_map;
    FCallStack *call_stack;
    ReturnState *return_state;
} ExecutionContext;

void set_return_state(ExecutionContext *context, void *return_value);
bool is_returning(ExecutionContext *context);
void *get_return_value(ExecutionContext *context);
void reset_return_state(ExecutionContext *context);

ExecutionContext *create_execution_context();

void free_execution_context(ExecutionContext *context); 

#endif // VARIABLE_STACK_H

