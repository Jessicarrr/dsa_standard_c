#include "sorting/selection_sort.h"

void selection_sort(int array[], int array_size) {
    int ptr_1, ptr_2, ptr_minimum = 0;

    for (ptr_1 = 0; ptr_1 < array_size - 1; ptr_1++) {
        ptr_minimum = ptr_1;

        for (ptr_2 = ptr_1 + 1; ptr_2 < array_size; ptr_2++) {
            
            if(array[ptr_2] < array[ptr_minimum]) {
                ptr_minimum = ptr_2;
            }

        }

        if (ptr_minimum != ptr_1) {
            int old_left_value = array[ptr_1];
            array[ptr_1] = array[ptr_minimum];
            array[ptr_minimum] = old_left_value;
        }
    }
        
}