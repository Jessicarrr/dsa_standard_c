#ifndef MERGE_SORT_H
#define MERGE_SORT_H

void merge_sort(int* arr, int size);
static void bottom_up_split_sort(int* source_array, int array_size,
                                 int* work_array);

static void bottom_up_merge(int* source_array, int start_index,
                            int middle_index, int end_index,
                            int* work_array, int array_size); 
#endif
