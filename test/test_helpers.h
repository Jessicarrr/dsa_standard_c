// test/test_helpers.h
#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

// Enum to specify array type
typedef enum {
    RANDOM_ARRAY,
    SORTED_ARRAY,
    REVERSE_SORTED_ARRAY
} array_type;

// Helper function to print arrays
void print_array(int arr[], int size, const char* message) {
    printf("\n%s: [", message);
    for (int i = 0; i < size; i++) {
        printf("%d%s", arr[i], i < size - 1 ? ", " : "");
    }
    printf("]\n");
}

bool is_array_ordered(int arr[], int size) {
    if (size <= 1) return true;

    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[i - 1]) {
            return false;
        }
    }

    return true;
}

// Function to generate array of specified type and size
int* generate_array(array_type type, int size) {
    int* arr = malloc(size * sizeof(int));
    
    switch(type) {
        case RANDOM_ARRAY:
            srand(time(NULL));  // Seed random number generator
            for(int i = 0; i < size; i++) {
                arr[i] = rand() % (size * 10);
            }
            break;
            
        case SORTED_ARRAY:
            for(int i = 0; i < size; i++) {
                arr[i] = i;
            }
            break;
            
        case REVERSE_SORTED_ARRAY:
            for(int i = 0; i < size; i++) {
                arr[i] = size - i - 1;
            }
            break;
    }
    
    return arr;
}

// Don't forget to free the array when done!
void free_test_array(int* arr) {
    free(arr);
}

#endif
