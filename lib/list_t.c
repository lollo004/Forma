#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include "list_t.h"

// Functions for integer linked list
IntLinkedList *create_int_list() {
    IntLinkedList *list = (IntLinkedList *)malloc(sizeof(IntLinkedList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void int_list_add(IntLinkedList *list, int value) {
    IntListNode *node = (IntListNode *)malloc(sizeof(IntListNode));
    node->value = value;
    node->next = NULL;
    if (list->tail) {
        list->tail->next = node;
    } else {
        list->head = node;
    }
    list->tail = node;
    list->size++;
}

void int_list_append(IntLinkedList *list1, IntLinkedList *list2) {
    if (!list2->head) return;
    if (list1->tail) {
        list1->tail->next = list2->head;
    } else {
        list1->head = list2->head;
    }
    list1->tail = list2->tail;
    list1->size += list2->size;
    free(list2);
}

void free_int_list(IntLinkedList *list) {
    IntListNode *current = list->head;
    while (current) {
        IntListNode *next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

// Functions for double linked list
DoubleLinkedList *create_double_list() {
    DoubleLinkedList *list = (DoubleLinkedList *)malloc(sizeof(DoubleLinkedList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void double_list_add(DoubleLinkedList *list, double value) {
    DoubleListNode *node = (DoubleListNode *)malloc(sizeof(DoubleListNode));
    node->value = value;
    node->next = NULL;
    if (list->tail) {
        list->tail->next = node;
    } else {
        list->head = node;
    }
    list->tail = node;
    list->size++;
}

void double_list_append(DoubleLinkedList *list1, DoubleLinkedList *list2) {
    if (!list2->head) return;
    if (list1->tail) {
        list1->tail->next = list2->head;
    } else {
        list1->head = list2->head;
    }
    list1->tail = list2->tail;
    list1->size += list2->size;
    free(list2);
}

void free_double_list(DoubleLinkedList *list) {
    DoubleListNode *current = list->head;
    while (current) {
        DoubleListNode *next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

// Functions for string linked list
StringLinkedList *create_string_list() {
    StringLinkedList *list = (StringLinkedList *)malloc(sizeof(StringLinkedList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void string_list_add(StringLinkedList *list, const char *value) {
    StringListNode *node = (StringListNode *)malloc(sizeof(StringListNode));
    node->value = strdup(value);
    node->next = NULL;
    if (list->tail) {
        list->tail->next = node;
    } else {
        list->head = node;
    }
    list->tail = node;
    list->size++;
}

void string_list_append(StringLinkedList *list1, StringLinkedList *list2) {
    if (!list2->head) return;
    if (list1->tail) {
        list1->tail->next = list2->head;
    } else {
        list1->head = list2->head;
    }
    list1->tail = list2->tail;
    list1->size += list2->size;
    free(list2);
}

void free_string_list(StringLinkedList *list) {
    StringListNode *current = list->head;
    while (current) {
        StringListNode *next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
    free(list);
}

// Functions for complex linked list
ComplexLinkedList *create_complex_list() {
    ComplexLinkedList *list = (ComplexLinkedList *)malloc(sizeof(ComplexLinkedList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void complex_list_add(ComplexLinkedList *list, double _Complex value) {
    ComplexListNode *node = (ComplexListNode *)malloc(sizeof(ComplexListNode));
    node->value = value;
    node->next = NULL;
    if (list->tail) {
        list->tail->next = node;
    } else {
        list->head = node;
    }
    list->tail = node;
    list->size++;
}

void complex_list_append(ComplexLinkedList *list1, ComplexLinkedList *list2) {
    if (!list2->head) return;
    if (list1->tail) {
        list1->tail->next = list2->head;
    } else {
        list1->head = list2->head;
    }
    list1->tail = list2->tail;
    list1->size += list2->size;
    free(list2);
}

void free_complex_list(ComplexLinkedList *list) {
    ComplexListNode *current = list->head;
    while (current) {
        ComplexListNode *next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

// Compare two integer linked lists
int compare_int_lists(IntLinkedList *list1, IntLinkedList *list2) {
    if (!list1 && !list2) return 1; // Both lists are NULL, considered equal
    if (!list1 || !list2) return 0; // Return 0 if only one list is NULL

    IntListNode *node1 = list1->head;
    IntListNode *node2 = list2->head;

    while (node1 && node2) {
        if (node1->value != node2->value) {
            return 0; // Lists are different
        }
        node1 = node1->next;
        node2 = node2->next;
    }

    return (node1 == NULL && node2 == NULL); // Return 1 if both lists are fully traversed
}

// Compare two double linked lists
int compare_double_lists(DoubleLinkedList *list1, DoubleLinkedList *list2) {
    if (!list1 && !list2) return 1; // Both lists are NULL, considered equal
    if (!list1 || !list2) return 0; // Return 0 if only one list is NULL

    DoubleListNode *node1 = list1->head;
    DoubleListNode *node2 = list2->head;

    while (node1 && node2) {
        if (node1->value != node2->value) {
            return 0; // Lists are different
        }
        node1 = node1->next;
        node2 = node2->next;
    }

    return (node1 == NULL && node2 == NULL); // Return 1 if both lists are fully traversed
}

// Compare two string linked lists
int compare_string_lists(StringLinkedList *list1, StringLinkedList *list2) {
    if (!list1 && !list2) return 1; // Both lists are NULL, considered equal
    if (!list1 || !list2) return 0; // Return 0 if only one list is NULL

    StringListNode *node1 = list1->head;
    StringListNode *node2 = list2->head;

    while (node1 && node2) {
        if (strcmp(node1->value, node2->value) != 0) {
            return 0; // Lists are different
        }
        node1 = node1->next;
        node2 = node2->next;
    }

    return (node1 == NULL && node2 == NULL); // Return 1 if both lists are fully traversed
}

// Compare two complex linked lists
int compare_complex_lists(ComplexLinkedList *list1, ComplexLinkedList *list2) {
    if (!list1 && !list2) return 1; // Both lists are NULL, considered equal
    if (!list1 || !list2) return 0; // Return 0 if only one list is NULL

    ComplexListNode *node1 = list1->head;
    ComplexListNode *node2 = list2->head;

    while (node1 && node2) {
        if (creal(node1->value) != creal(node2->value) || cimag(node1->value) != cimag(node2->value)) {
            return 0; // Lists are different
        }
        node1 = node1->next;
        node2 = node2->next;
    }

    return (node1 == NULL && node2 == NULL); // Return 1 if both lists are fully traversed
}

// Generic compare function for any list type
int compare_lists(void *list1, void *list2, SymbolType type) {
    if (!list1 && !list2) return 1; // Both lists are NULL, considered equal
    if (!list1 || !list2) return 0; // Return 0 if only one list is NULL

    switch (type) {
        case TYPE_INTLIST:
            return compare_int_lists((IntLinkedList *)list1, (IntLinkedList *)list2);
        case TYPE_FLOATLIST:
            return compare_double_lists((DoubleLinkedList *)list1, (DoubleLinkedList *)list2);
        case TYPE_STRINGLIST:
            return compare_string_lists((StringLinkedList *)list1, (StringLinkedList *)list2);
        case TYPE_COMPLEXLIST:
            return compare_complex_lists((ComplexLinkedList *)list1, (ComplexLinkedList *)list2);
        default:
            return 0; // Unknown type
    }
}

// Helper function to verify the list type
SymbolType get_list_type(void *list) {
    if (((IntLinkedList *)list)->head && ((IntLinkedList *)list)->head->value) {
        return TYPE_INTLIST;
    } else if (((DoubleLinkedList *)list)->head && ((DoubleLinkedList *)list)->head->value) {
        return TYPE_FLOATLIST;
    } else if (((StringLinkedList *)list)->head && ((StringLinkedList *)list)->head->value) {
        return TYPE_STRINGLIST;
    } else if (((ComplexLinkedList *)list)->head && ((ComplexLinkedList *)list)->head->value) {
        return TYPE_COMPLEXLIST;
    }
    return 0; // Unknown type
}
