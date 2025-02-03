
#include <stdio.h>
#include "helpers.h"
#include <stdbool.h>

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
