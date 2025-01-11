// static_analysis_stack.h
#ifndef STATIC_ANALYSIS_STACK_H
#define STATIC_ANALYSIS_STACK_H

#include <stdbool.h>

#define HASH_TABLE_SIZE 101

// Enumeration for symbol types
typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_COMPLEX,
    TYPE_INTLIST,
    TYPE_FLOATLIST,
    TYPE_STRINGLIST,
    TYPE_COMPLEXLIST
} SymbolType;

// Enumeration for symbol nature
typedef enum {
    NATURE_VARIABLE,
    NATURE_FUNCTION
} SymbolNature;

struct fparams {
	SymbolType params[10];
	SymbolNature natparams[10];
	int nparams;
};
struct fargs {
	char *args[10];
	SymbolNature natargs[10];
	int nargs;
};

// Symbol structure
typedef struct Symbol {
    char *name;             // Name of the symbol
    SymbolType type;        // Type of the symbol
    SymbolNature nature;    // Nature of the symbol (variable or function)
    int scope_level;        // Level of scope where the symbol is defined
    SymbolType *params;	    // Arguments types
    SymbolNature *natparams;// Arguments natures
    int nparams;            // N Arguments
    struct Symbol *next;    // Pointer to the next symbol (for hash table chaining)
} Symbol;

// Symbol table structure
typedef struct SymbolTable {
    Symbol **table;  // Hash table
} SymbolTable;

// Scope stack structure
typedef struct StaticScopeStack {
    SymbolTable **scopes;  // Array of symbol tables for nested scopes
    SymbolTable *global;   // Global symbol table
    int top;               // Index of the top scope
    int capacity;          // Maximum capacity of the stack
} StaticScopeStack;

// Function declarations
SymbolType set_to_enumtype(char set, bool islist);
StaticScopeStack *create_static_scope_stack(int capacity);
void destroy_static_scope_stack(StaticScopeStack *stack);
void enter_static_scope(StaticScopeStack *stack);
void exit_static_scope(StaticScopeStack *stack);
bool add_static_symbol(StaticScopeStack *stack, const char *name, SymbolType type, SymbolNature nature, bool to_global);
void addparam_static_symbol(StaticScopeStack *stack, const char *name, const SymbolType *p, const SymbolNature *n, int plen);
Symbol *find_static_symbol(StaticScopeStack *stack, const char *name);
void print_static_scope_stack(StaticScopeStack *stack);

#endif // STATIC_ANALYSIS_STACK_H

