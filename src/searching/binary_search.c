#include "searching/binary_search.h"
#include <stdio.h>

int binary_search(int array[], int size, int target_number) {
    printf("Searching binarily thru array of size %d\n", size);

    int left_pointer = 0;
    int right_pointer = size - 1;

    while (left_pointer <= right_pointer) {
        int middle_pointer = left_pointer + (right_pointer - left_pointer) / 2;
        int middle_value = array[middle_pointer];

        if (middle_value > target_number) {
            right_pointer = middle_pointer - 1;
            continue;
        }

        if (middle_value < target_number) {
            left_pointer = middle_pointer + 1;
            continue;
        }

        return middle_pointer;
    }

    return -1;
}