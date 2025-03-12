#include "data_structures/linear/list.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define CAPACITY_INCREMENT 8
#define NEEDS_ROOM_THRESHOLD 1

static bool increase_list_capacity(List* list, DscError **error);
static bool decrease_list_capacity(List* list, DscError **error);
static bool needs_capacity(List* list);
static bool needs_capacity_decrease(List* list);

List* create_list(size_t item_size, DscError **error) {
    List* new_list = (List*) malloc(sizeof(List));
    if (new_list == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_MEM_ALLOC, "Could not create new list, out of memory.");
        }
        return NULL;
    }
    new_list->length = 0;
    new_list->item_size = item_size;
    new_list->capacity = 0;
    new_list->data = NULL;
    return new_list;
}

static bool decrease_list_capacity(List* list, DscError **error) {
    if (list->capacity <= CAPACITY_INCREMENT) {
        return true;
    }

    size_t new_capacity = list->capacity - CAPACITY_INCREMENT;
    if (list->length > new_capacity) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_RESIZE_FAIL, "Cannot decrease capacity: list length exceeds new capacity.");
        }
        return false;
    }

    void* new_data = malloc(list->item_size * new_capacity);
    if (new_data == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_MEM_ALLOC, "Could not decrease list capacity, out of memory.");
        }
        return false;
    }

    if (list->length > 0) {
        memmove(new_data, list->data, list->item_size * list->length);
    }
    free(list->data);
    list->data = new_data;
    list->capacity = new_capacity;
    return true;
}

static bool increase_list_capacity(List* list, DscError **error) {
    void* new_data = malloc(list->item_size * (list->capacity + CAPACITY_INCREMENT));
    if (new_data == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_MEM_ALLOC, "Could not increase list capacity, out of memory.");
        }
        return false;
    }
    if (list->length > 0) {
        memmove(new_data, list->data, list->item_size * list->length);
    }
    free(list->data);
    list->data = new_data;
    list->capacity += CAPACITY_INCREMENT;
    return true;
}

static bool needs_capacity(List* list) {
    size_t room_left = list->capacity - list->length;
    return (room_left <= NEEDS_ROOM_THRESHOLD);
}

static bool needs_capacity_decrease(List* list) {
    size_t room_left = list->capacity - list->length;
    return (room_left >= CAPACITY_INCREMENT + NEEDS_ROOM_THRESHOLD);
}

void list_insert(List* list, void* item, DscError **error) {
    if (list == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "List is NULL in list_insert.");
        }
        return;
    }
    if (needs_capacity(list)) {
        if (!increase_list_capacity(list, error)) {
            return; // error set by increase_list_capacity
        }
    }
    size_t position = list->length;
    uint8_t* address = (uint8_t *)list->data + (position * list->item_size);
    memcpy(address, item, list->item_size);
    list->length += 1;
}

void list_insert_at(List* list, void* item, size_t position, DscError **error) {
    if (list == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "List is NULL in list_insert_at.");
        }
        return;
    }
    if (position > list->length) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "Index out of bounds in list_insert_at.");
        }
        return;
    }
    if (needs_capacity(list)) {
        if (!increase_list_capacity(list, error)) {
            return;
        }
    }
    uint8_t* address = (uint8_t*) list->data + (position * list->item_size);
    if (position < list->length) {
        size_t num_elements_to_move = list->length - position;
        size_t size_to_move = num_elements_to_move * list->item_size;
        uint8_t* next = address + list->item_size;
        memmove(next, address, size_to_move);
    }
    memcpy(address, item, list->item_size);
    list->length += 1;
}

void list_remove(List* list, size_t index, DscError **error) {
    if (list == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "List is NULL in list_remove.");
        }
        return;
    }
    if (list->length == 0) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_IS_EMPTY, "List is empty in list_remove.");
        }
        return;
    }
    if (index >= list->length) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "Index out of bounds in list_remove.");
        }
        return;
    }
    uint8_t* index_address = (uint8_t*)list->data + (list->item_size * index);
    uint8_t* next_address = (uint8_t*)list->data + (list->item_size * (index + 1));
    size_t size_to_move = list->item_size * (list->length - index - 1);
    memmove(index_address, next_address, size_to_move);
    list->length -= 1;
    if (needs_capacity_decrease(list)) {
        (void)decrease_list_capacity(list, error);
        // If decrease_list_capacity fails, error is set by that function.
    }
}

void* list_get_pointer_to(List* list, size_t index, DscError **error) {
    if (list == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "List is NULL in list_get_pointer_to.");
        }
        return NULL;
    }
    if (index >= list->length) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "Index out of bounds in list_get_pointer_to.");
        }
        return NULL;
    }
    uint8_t* pointer = (uint8_t*) list->data + (index * list->item_size);
    return (void*) pointer;
}

int list_get_value_at(List* list, size_t index, void* out, DscError **error) {
    if (list == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "List is NULL in list_get_value_at.");
        }
        return -1;
    }
    if (list->length == 0) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_IS_EMPTY, "List is empty in list_get_value_at.");
        }
        return -1;
    }
    if (index >= list->length) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "Index out of bounds in list_get_value_at.");
        }
        return -1;
    }
    uint8_t* pointer = (uint8_t*) list->data + (index * list->item_size);
    memcpy(out, pointer, list->item_size);
    return 0;
}

void list_destroy(List* list) {
    if (list == NULL) {
        return;
    }
    free(list->data);
    list->data = NULL;
    free(list);
}

void print_list(List* list) {
    if (list == NULL) {
        printf("List is NULL.\n");
        return;
    }
    printf("List @ %p - length: %zd, item size: %zd, capacity: %zd, contents:\n",
           (void*)list, list->length, list->item_size, list->capacity);
    if (list->length == 0) {
        printf("Empty list.\n");
        return;
    }
    int* array_as_int = (int*) list->data;
    printf("[ ");
    for (size_t i = 0; i < list->length; i++) {
        printf("%d, ", array_as_int[i]);
    }
    printf("]\n");
}

void print_list_custom(List* list, char* (*to_string)(void* item)) {
    if (list == NULL) {
        printf("List is NULL.\n");
        return;
    }
    if (to_string == NULL) {
        fprintf(stderr, "print_list_custom: to_string function pointer is NULL.\n");
        return;
    }
    printf("List @ %p - length: %zd, item size: %zd, capacity: %zd, contents:\n",
           (void*)list, list->length, list->item_size, list->capacity);
    if (list->length == 0) {
        printf("Empty list.\n");
        return;
    }
    printf("[ ");
    for (size_t i = 0; i < list->length; i++) {
        void* item = (uint8_t*) list->data + (i * list->item_size);
        char* description = to_string(item);
        printf("%s", description);
        free(description);
        printf(", ");
    }
    printf("]\n");
}
