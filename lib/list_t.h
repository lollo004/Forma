#ifndef LINKED_LIST_LIBRARY_H
#define LINKED_LIST_LIBRARY_H

#include <complex.h>
#include <stdlib.h>
#include "static_analysis.h"

// Integer linked list structures
typedef struct IntListNode {
    int value;
    struct IntListNode *next;
} IntListNode;

typedef struct IntLinkedList {
    IntListNode *head;
    IntListNode *tail;
    size_t size;
} IntLinkedList;

// Double linked list structures
typedef struct DoubleListNode {
    double value;
    struct DoubleListNode *next;
} DoubleListNode;

typedef struct DoubleLinkedList {
    DoubleListNode *head;
    DoubleListNode *tail;
    size_t size;
} DoubleLinkedList;

// String linked list structures
typedef struct StringListNode {
    char *value;
    struct StringListNode *next;
} StringListNode;

typedef struct StringLinkedList {
    StringListNode *head;
    StringListNode *tail;
    size_t size;
} StringLinkedList;

// Complex linked list structures
typedef struct ComplexListNode {
    double _Complex value;
    struct ComplexListNode *next;
} ComplexListNode;

typedef struct ComplexLinkedList {
    ComplexListNode *head;
    ComplexListNode *tail;
    size_t size;
} ComplexLinkedList;

// List type enumeration
SymbolType get_list_type(void *list);

// Integer linked list functions
IntLinkedList *create_int_list();
void int_list_add(IntLinkedList *list, int value);
void int_list_append(IntLinkedList *list1, IntLinkedList *list2);
void free_int_list(IntLinkedList *list);

// Double linked list functions
DoubleLinkedList *create_double_list();
void double_list_add(DoubleLinkedList *list, double value);
void double_list_append(DoubleLinkedList *list1, DoubleLinkedList *list2);
void free_double_list(DoubleLinkedList *list);

// String linked list functions
StringLinkedList *create_string_list();
void string_list_add(StringLinkedList *list, const char *value);
void string_list_append(StringLinkedList *list1, StringLinkedList *list2);
void free_string_list(StringLinkedList *list);

// Complex linked list functions
ComplexLinkedList *create_complex_list();
void complex_list_add(ComplexLinkedList *list, double _Complex value);
void complex_list_append(ComplexLinkedList *list1, ComplexLinkedList *list2);
void free_complex_list(ComplexLinkedList *list);

#endif // LINKED_LIST_LIBRARY_H

