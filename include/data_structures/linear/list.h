#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include "dsc_result.h"

typedef struct List {
    void* data;
    size_t length;
    size_t item_size;
    size_t capacity;
} List;

/**
 * @brief Creates a new list.
 *
 * @param item_size Size of each element.
 * @param error Optional double pointer for error reporting.
 * @return Pointer to the new List, or NULL on failure.
 */
List* create_list(size_t item_size, DscError **error);

/**
 * @brief Inserts an item at the end of the list.
 *
 * @param list The list.
 * @param item The item to insert.
 * @param error Optional double pointer for error reporting.
 */
void list_insert(List* list, void* item, DscError **error);

/**
 * @brief Inserts an item at the specified position in the list.
 *
 * @param list The list.
 * @param item The item to insert.
 * @param position The index position.
 * @param error Optional double pointer for error reporting.
 */
void list_insert_at(List* list, void* item, size_t position, DscError **error);

/**
 * @brief Removes an item at the specified index.
 *
 * @param list The list.
 * @param index The index of the item to remove.
 * @param error Optional double pointer for error reporting.
 */
void list_remove(List* list, size_t index, DscError **error);

/**
 * @brief Gets a pointer to the item at the specified index.
 *
 * @param list The list.
 * @param index The index.
 * @param error Optional double pointer for error reporting.
 * @return Pointer to the item, or NULL if an error occurs.
 */
void* list_get_pointer_to(List* list, size_t index, DscError **error);

/**
 * @brief Copies the value at the specified index to the out parameter.
 *
 * @param list The list.
 * @param index The index.
 * @param out Pointer where the value is copied.
 * @param error Optional double pointer for error reporting.
 * @return 0 on success, -1 on failure.
 */
int list_get_value_at(List* list, size_t index, void* out, DscError **error);

void list_destroy(List* list);

void print_list(List* list);

void print_list_custom(List* list, char* (*to_string)(void* item));

#endif
