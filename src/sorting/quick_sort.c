#include <stdio.h>
#include "sorting/quick_sort.h"
#include "helpers.h"

void quick_sort(int* arr, int size) {
    quick_sort_with_pointers(arr,
                             size,
                             0,
                             size - 1);

}

void quick_sort_with_pointers(int* arr, int size, int left, int right) {
    if (left >= right || left < 0) return;

    int pivot_position = partition(arr, size, left, right);
 

    quick_sort_with_pointers(arr,
                             size,
                             left,
                             pivot_position - 1);

    quick_sort_with_pointers(arr,
                             size,
                             pivot_position,
                             right);   
}

int partition(int* arr, int size, int left, int right) {
    int pivot_position = right;
    int pivot_value = arr[pivot_position];
    // printf("\nPivot value -> %d\n", pivot_value);
    
    int left_pointer = left - 1;
    int right_pointer = right + 1 ;

    while (1) {

        do {
            left_pointer++;
        }
        while (left_pointer < right_pointer
               && arr[left_pointer] < pivot_value);

        do {
            right_pointer--;
        }
        while (right_pointer > left_pointer
               && arr[right_pointer] > pivot_value);

        if (left_pointer >= right_pointer) {
            
            return left_pointer;
        }

        // int left_and_right[2] = {left_pointer, right_pointer};
        // printf("\n~Swapping Elements~\n");
        // printf("From -> ");
        // print_array_with_bold_elements(arr, size,
        //                                 left_and_right,
        //                               2);
        //
        // printf("\nTo ->");

        swap_elements(arr, left_pointer, right_pointer);
        // print_array_with_bold_elements(arr, size,
        //                               left_and_right,
        //                               2);
        //printf("\n");
    }
}
