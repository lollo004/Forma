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

// Compare lists
int compare_int_lists(IntLinkedList *list1, IntLinkedList *list2);
int compare_double_lists(DoubleLinkedList *list1, DoubleLinkedList *list2);
int compare_string_lists(StringLinkedList *list1, StringLinkedList *list2);
int compare_complex_lists(ComplexLinkedList *list1, ComplexLinkedList *list2);

// linked list clone functions
IntLinkedList *int_list_cloneadd(IntLinkedList *list, int value);
IntLinkedList *int_list_cloneappend(IntLinkedList *list1, IntLinkedList *list2);
DoubleLinkedList *double_list_cloneadd(DoubleLinkedList *list, double value);
DoubleLinkedList *double_list_cloneappend(DoubleLinkedList *list1, DoubleLinkedList *list2);
StringLinkedList *string_list_cloneadd(StringLinkedList *list, const char *value);
StringLinkedList *string_list_cloneappend(StringLinkedList *list1, StringLinkedList *list2);
ComplexLinkedList *complex_list_cloneadd(ComplexLinkedList *list, double _Complex value);
ComplexLinkedList *complex_list_cloneappend(ComplexLinkedList *list1, ComplexLinkedList *list2);

// linked list clone slice function
IntLinkedList *int_list_cloneslice(IntLinkedList *list, int a, int b);
DoubleLinkedList *double_list_cloneslice(DoubleLinkedList *list, int a, int b);
StringLinkedList *string_list_cloneslice(StringLinkedList *list, int a, int b);
ComplexLinkedList *complex_list_cloneslice(ComplexLinkedList *list, int a, int b);

int *int_list_get_reference(IntLinkedList *list, size_t index);
double *double_list_get_reference(DoubleLinkedList *list, size_t index);
char *string_list_get_copy(StringLinkedList *list, size_t index);
double _Complex *complex_list_get_reference(ComplexLinkedList *list, size_t index);

#endif // LINKED_LIST_LIBRARY_H

