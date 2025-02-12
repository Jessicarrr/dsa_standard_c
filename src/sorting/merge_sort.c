#include "sorting/merge_sort.h"
#include "stdlib.h"
#include <stdio.h>

#include "helpers.h"

static void bottom_up_merge(int* source_array, int start_index,
                            int middle_index, int end_index,
                            int* work_array, int array_size) {
    int left_pointer = start_index;
    int right_pointer = middle_index;
    int work_pointer = start_index;

    while(left_pointer < middle_index && right_pointer < end_index) {
        if(source_array[left_pointer] < source_array[right_pointer]) {
            work_array[work_pointer] = source_array[left_pointer];
            left_pointer++;
        }
        else {
            work_array[work_pointer] = source_array[right_pointer];
            right_pointer++;
        }
        work_pointer++;
    }

    while (left_pointer < middle_index) {
        work_array[work_pointer] = source_array[left_pointer];
        left_pointer++;
        work_pointer++;
    }

    while (right_pointer < end_index) {
        work_array[work_pointer] = source_array[right_pointer];
        right_pointer++;
        work_pointer++;
    }

}

/*static void bottom_up_merge(int* source_array, int start_index,
                            int middle_index, int end_index,
                            int* work_array, int array_size) {

    int left_pointer = start_index;
    int right_pointer = middle_index;
    
    for(int work_pointer = start_index;
            work_pointer < end_index;
            work_pointer++)
    {
        
        if(left_pointer < middle_index &&
           (right_pointer >= end_index ||
            source_array[left_pointer] <= source_array[right_pointer])) {

            work_array[work_pointer] = source_array[left_pointer];
            left_pointer++;

        }
        else {
            work_array[work_pointer] = source_array[right_pointer];
            right_pointer++;
        }
    }

    int bold_indexes[2] = {start_index, end_index};
            printf("Final work array -> ");
            print_array_with_bold_elements(work_array, array_size,
                                    bold_indexes,
                                    3); 




}*/

static void bottom_up_split_sort(int* source_array, int array_size,
                                 int* work_array) {

    for (int run_size = 1; run_size < array_size;
             run_size = run_size * 2) {

        for(int start_index = 0; start_index < array_size;
                start_index = start_index + 2 * run_size) {

            int end_index = (start_index + 2 * run_size < array_size) ?
                                start_index + 2 * run_size :
                                array_size;

            int middle_index = (start_index + run_size < array_size) ?
                                start_index + run_size :
                                array_size;
        


            int bold_indexes[3] = {start_index, middle_index, end_index};
            printf("Now working with indexes %d,%d,%d, subarray -> ",
                   start_index, middle_index, end_index);
            print_array_with_bold_elements(source_array, array_size,
                                    bold_indexes,
                                    3); 



            bottom_up_merge(source_array, start_index, middle_index,
                            end_index, work_array, array_size);
        }
        copy_array_to(work_array, array_size, source_array, array_size);
    }
}

void merge_sort(int* arr, int size) {
    if (size <= 1) return;

    int* work_array = calloc(size, sizeof(int));

    bottom_up_split_sort(arr, size, work_array);

    copy_array_to(work_array, size, arr, size);

    free(work_array);
}



