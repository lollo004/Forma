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
