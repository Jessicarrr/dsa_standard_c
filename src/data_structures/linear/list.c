#include "data_structures/linear/list.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define CAPACITY_INCREMENT 8
#define NEEDS_ROOM_THRESHOLD 1

List* create_list(size_t item_size) {
    List* new_list = (List*) malloc(sizeof(List));

    if (new_list == NULL) {
        fprintf(stderr, "Could not create new list, "
                "potentially out of memory on the system.\n");
        return NULL;
    }

    new_list->length = 0;
    new_list->item_size = item_size;
    new_list->capacity = 0;
    new_list->data = NULL;
    return new_list;

}
static void decrease_list_capacity(List* list) {
    if (list->capacity <= CAPACITY_INCREMENT) {
        return;
    }

    size_t new_capacity = list->capacity - CAPACITY_INCREMENT;

    if (list->length > new_capacity) {
        fprintf(stderr, "decrease_list_capacity called when the length of "
                        "the list would end up being greater than the "
                        "capacity. The operation has been stopped because "
                        "of the potential for data loss.");
        return;
    }
    
    
    void* new_data = malloc(
        list->item_size *
        new_capacity
    );

    if (new_data == NULL) {
        fprintf(stderr, "Could not create new list capacity, "
                "potentially out of memory on the system.");
        return;
    }

    if (list->length > 0) {
        memmove(new_data, list->data, list->item_size * list->length);
    }

    free(list->data);
    list->data = new_data;
    list->capacity = new_capacity;
}
static void increase_list_capacity(List* list) {
    void* new_data = malloc(
        list->item_size * 
        (list->capacity + CAPACITY_INCREMENT)
    );

    if (new_data == NULL) {
        fprintf(stderr, "Could not create new list capacity, "
                "potentially out of memory on the system.\n");
        return;
    }

    if (list->length > 0) {
        memmove(new_data, list->data, list->item_size * list->length);
    }

    free(list->data);
    list->data = new_data;
    list->capacity = list->capacity + CAPACITY_INCREMENT;
}

static bool needs_capacity_decrease(List* list) {
    size_t room_left = list->capacity - list->length;

    if (room_left >= CAPACITY_INCREMENT + NEEDS_ROOM_THRESHOLD) {
        return true;
    }

    return false;
}

static bool needs_capacity(List* list) {
    size_t room_left = list->capacity - list->length;

    if (room_left <= NEEDS_ROOM_THRESHOLD) {
        return true;
    }

    return false;
}

void list_insert(List* list, void* item) {
   if (needs_capacity(list)) {
        increase_list_capacity(list);
    }

    size_t position = list->length;
    uint8_t* address = (uint8_t *)list->data + (position * list->item_size);
    memcpy(address, item, list->item_size);
    list->length += 1;
}

void list_insert_at(List* list, void* item, size_t position) {
    if (needs_capacity(list)) {
        increase_list_capacity(list);
    }

    if (position > list->length || position < 0) {
        fprintf(stderr, "Index out of bounds in List, using index %zd", position);
        return;
    }

    uint8_t* address = (uint8_t*) list->data + (position * list->item_size);

    if (position < list->length) {
        size_t num_elements_to_move = (list->length) - position;
        size_t size_to_move = num_elements_to_move * list->item_size;
        uint8_t* next = address + list->item_size;

        /*printf("Inserting at address %p - moving next elements forward "
                "starting at %p and "
                "moving %zd elements with size of %zd\n",
                address, next, num_elements_to_move, size_to_move);*/
               
        memmove(next, address, size_to_move);
    }
    /*else {
        printf("Inserting at address %p - no elements to move.\n", address);
    }*/

    memcpy(address, item, list->item_size);
    list->length += 1;
}

void list_remove(List* list, size_t index) {
    if (list == NULL || list->length <= 0) {
        return;
    }

    if (index < 0) {
        fprintf(stderr, "Cannot remove item from a list using a "
                         "negative index (%zd)\n",
                index);
        return;
    }

    if (index >= list->length) {
        fprintf(stderr, "Cannot delete element at index %zd because the "
                "index is beyond the scope of the list. (length %zd)\n",
                index, list->length);
        return;
    }

    uint8_t* index_address =((uint8_t*)list->data) + (list->item_size * index);

    uint8_t* next_address = ((uint8_t*)
                            list->data) + (list->item_size * index) 
                            + list->item_size;

    size_t size_to_move = (list->item_size * (list->length - index - 1));

    //printf("Moving. Destination = %p, source = %p, size = %zd\n",
    //       index_address, next_address, size_to_move);
    //
    memmove(index_address, next_address, size_to_move);
    list->length -= 1;

    if (needs_capacity_decrease(list)) {
        decrease_list_capacity(list);
    }
}

void* list_get_pointer_to(List* list, size_t index) {
    if(list == NULL || index < 0 || index > list->length) {
        return NULL;
    }

    uint8_t* first_pointer = (uint8_t*) list->data;
    uint8_t* relevant_pointer = (uint8_t*)
            (first_pointer + (list->item_size * index));
    void* void_pointer = (void*) relevant_pointer;
    return void_pointer;
}

int list_get_value_at(List* list, size_t index, void* out) {
    if(list == NULL || index < 0 
        || index > list->length || list->length < 1) {
        return -1;
    }

    uint8_t* relevant_pointer = ((uint8_t*) 
            list->data) + list->item_size * index;
    memcpy(out, relevant_pointer, list->item_size);
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
        printf("List is null.\n");
        return;
    }

    printf("List @ %p - length: %zd, data size: %zd, "
           "capacity: %zd, contents:\n",
           (void*)list, list->length, list->item_size, list->capacity);

    if (list->length <= 0) {
        printf("Empty list.\n");
        return;
    }

    int* array_as_int = (int*)list->data;

    printf("[ ");

    for(size_t i = 0; i < list->length; i++) {
        int data_as_int = array_as_int[i];
        printf("%d, ", data_as_int);
    }
    printf("]\n");
}

void print_list_custom(List* list, char* (*to_string)(void* item)) {
    if (list == NULL) {
        printf("List is null.\n");
        return;
    }

    if (to_string == NULL) {
        fprintf(stderr, "print_list_custom function parameter should not be null.");
        return;
    }

    printf("List @ %p - length: %zd, data size: %zd, "
        "capacity: %zd, contents:\n",
        (void*)list, list->length, list->item_size, list->capacity);

    if (list->length <= 0) {
        printf("Empty list.\n");
        return;
    }

    printf("[ ");
    
    for(size_t i = 0; i < list->length; i++) {
        void* item = ((uint8_t*)list->data) + (i * list->item_size);
        char* description = to_string(item);
        printf("%s", description);
        free(description);
        printf(", ");
    }

    printf("]\n");
}
