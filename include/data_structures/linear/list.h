#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct List {
    void* data;
    size_t length;
    size_t item_size;
    size_t capacity;
} List;

List* create_list(size_t item_size);

void list_insert(List* list, void* data);

void list_insert_at(List* list, void* item, size_t position);

void list_remove(List* list, size_t index);

void* list_get_pointer_to(List* list, size_t index);

int list_get_value_at(List* list, size_t index, void* out);

void list_destroy(List* list);

void print_list(List* list);

#endif