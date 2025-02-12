
#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"
#include <stdbool.h>

typedef enum {
    SUCCESS = 0,
    ERROR_SIZE_MISMATCH = 1,
    ERROR_NULL_ARRAY = 2
} CopyStatus;

bool is_in_array(int* arr, int size, int value) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) {
            return true;
        }
    }
    return false;
}

void print_array_with_bold_elements(int* arr, int size, int* bold_indexes,
                                    int bold_indexes_size) {
    printf("[ ");

    for (int i = 0; i < size; i++) {
        if (is_in_array(bold_indexes, bold_indexes_size, i)) {
            printf(COLOR_BOLD "%d " COLOR_OFF, arr[i]);
        }
        else {
            printf("%d ", arr[i]);
        }
    }

    printf("]\n");
}

void swap_elements(int* arr, int left, int right) {
    int left_value = arr[left];

    arr[left] = arr[right];
    arr[right] = left_value;
}

int* copy_array(int* array, int size) {
    int* new_array = malloc(sizeof(int) * size);

    for(int pointer = 0; pointer < size; pointer++) {
        new_array[pointer] = array[pointer];

    }

    return new_array;

}

int copy_array_to(int* source_array, int source_size,
                   int* destination_array, int destination_size) {

    if (source_array == NULL) {
        fprintf(stderr, "copy_array_to could not complete because "
                        "the source array was null.");
        return ERROR_NULL_ARRAY;
    }

    if (destination_array == NULL) {
        fprintf(stderr, "copy_array_to could not complete because "
                        "the destination array was null.");
        return ERROR_NULL_ARRAY;
    }

    if(source_size != destination_size) {
        fprintf(stderr, "copy_array_to requires both arrays to be the "
                        "same size.");
        return ERROR_SIZE_MISMATCH;
    }

    for(int i = 0; i < source_size; i++) {
        destination_array[i] = source_array[i];
    }

    return SUCCESS;

}
