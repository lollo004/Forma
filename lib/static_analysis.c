// static_analysis_stack.c
#include "static_analysis.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Hash function for symbol names
static unsigned int hash(const char *name) {
    unsigned int hash = 0;
    while (*name) {
        hash = (hash << 5) + *name++;
    }
    return hash % HASH_TABLE_SIZE;
}

// Create a symbol
static Symbol *create_symbol(const char *name, SymbolType type, SymbolNature nature, int scope_level) {
    Symbol *symbol = malloc(sizeof(Symbol));
    symbol->name = strdup(name);
    symbol->type = type;
    symbol->nature = nature;
    symbol->scope_level = scope_level;
    symbol->next = NULL;
    return symbol;
}

// Destroy a symbol
static void destroy_symbol(Symbol *symbol) {
    if (symbol) {
        free(symbol->name);
        free(symbol);
    }
}

// Create a symbol table
static SymbolTable *create_symbol_table() {
    SymbolTable *table = malloc(sizeof(SymbolTable));
    table->table = calloc(HASH_TABLE_SIZE, sizeof(Symbol *));
    return table;
}

// Destroy a symbol table
static void destroy_symbol_table(SymbolTable *table) {
    if (table) {
        for (int i = 0; i < HASH_TABLE_SIZE; i++) {
            Symbol *symbol = table->table[i];
            while (symbol) {
                Symbol *next = symbol->next;
                destroy_symbol(symbol);
                symbol = next;
            }
        }
        free(table->table);
        free(table);
    }
}

// Create the static scope stack
StaticScopeStack *create_static_scope_stack(int capacity) {
    StaticScopeStack *stack = malloc(sizeof(StaticScopeStack));
    stack->scopes = malloc(capacity * sizeof(SymbolTable *));
    stack->top = -1;
    stack->capacity = capacity;
    stack->global = create_symbol_table();  // Create the global symbol table
    return stack;
}

// Destroy the static scope stack
void destroy_static_scope_stack(StaticScopeStack *stack) {
    if (stack->global) destroy_symbol_table(stack->global);
    for (int i = 0; i <= stack->top; i++) {
        destroy_symbol_table(stack->scopes[i]);
    }
    free(stack->scopes);
    free(stack);
}

// Enter a new static scope
void enter_static_scope(StaticScopeStack *stack) {
    if (stack->top + 1 >= stack->capacity) {
        fprintf(stderr, "Static scope stack overflow\n");
        exit(EXIT_FAILURE);
    }
    stack->scopes[++stack->top] = create_symbol_table();
}

// Exit the current static scope
void exit_static_scope(StaticScopeStack *stack) {
    if (stack->top < 0) {
        fprintf(stderr, "Static scope stack underflow\n");
        exit(EXIT_FAILURE);
    }
    destroy_symbol_table(stack->scopes[stack->top--]);
}

// Add a symbol to the current or global scope
bool add_static_symbol(StaticScopeStack *stack, const char *name, SymbolType type, SymbolNature nature, bool to_global) {
    SymbolTable *table = to_global ? stack->global : (stack->top >= 0 ? stack->scopes[stack->top] : stack->global);
    unsigned int index = hash(name);
    Symbol *symbol = table->table[index];

    // Check for duplicates
    while (symbol) {
        if (strcmp(symbol->name, name) == 0) {
            fprintf(stderr, "Error: Symbol '%s' already defined in the current scope\n", name);
            return false;
        }
        symbol = symbol->next;
    }

    // Add the new symbol
    symbol = create_symbol(name, type, nature, stack->top);
    symbol->next = table->table[index];
    table->table[index] = symbol;
    return true;
}

// Find a symbol in the current or parent scopes, or global
Symbol *find_static_symbol(StaticScopeStack *stack, const char *name) {
    for (int i = stack->top; i >= 0; i--) {
        SymbolTable *table = stack->scopes[i];
        unsigned int index = hash(name);
        Symbol *symbol = table->table[index];
        while (symbol) {
            if (strcmp(symbol->name, name) == 0) {
                return symbol;
            }
            symbol = symbol->next;
        }
    }
    // Check global scope
    unsigned int index = hash(name);
    Symbol *symbol = stack->global->table[index];
    while (symbol) {
        if (strcmp(symbol->name, name) == 0) {
            return symbol;
        }
        symbol = symbol->next;
    }
    return NULL;
}

// Print the current state of the static scope stack
void print_static_scope_stack(StaticScopeStack *stack) {
    printf("--- Static Scope Stack ---\n");
    for (int i = stack->top; i >= 0; i--) {
        printf("Scope %d:\n", i);
        for (int j = 0; j < HASH_TABLE_SIZE; j++) {
            Symbol *symbol = stack->scopes[i]->table[j];
            while (symbol) {
                printf("  %s (Type: %d, Nature: %d)\n", symbol->name, symbol->type, symbol->nature);
                symbol = symbol->next;
            }
        }
    }
    printf("Global Scope:\n");
    for (int j = 0; j < HASH_TABLE_SIZE; j++) {
        Symbol *symbol = stack->global->table[j];
        while (symbol) {
            printf("  %s (Type: %d, Nature: %d)\n", symbol->name, symbol->type, symbol->nature);
            symbol = symbol->next;
        }
    }
    printf("--------------------------\n");
}

