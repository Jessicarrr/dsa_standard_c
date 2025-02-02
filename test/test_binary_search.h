#include <stdio.h>

#include "sorting/selection_sort.h"
#include "searching/binary_search.h"

#include "test_helpers.h"
#include <CUnit/Basic.h>

void test_binary_search_ordered(void) {
    printf("\n\033[1;35m=== Testing Binary Search with Sorted Array "
           "(Specific Number) ===\033[0m\n");
    
    int size = 10000;
    int* arr = generate_array(SORTED_ARRAY, size);
    int target = 328;

    print_array(arr, size < 20 ? size : 20,
                "Before sorting (first 20 elements)");

    clock_t start = clock();
    int result = binary_search(arr, size, target);
    clock_t end = clock();

    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_taken);
    printf("Searched for target number %d, got position %d\n", target, result);

    CU_ASSERT(target == result);
}

void test_binary_search_find_first(void) {
    printf("\n\033[1;35m=== Testing Binary Search with Sorted Array "
           "(First Number) ===\033[0m\n");
    
    int size = 10000;
    int* arr = generate_array(SORTED_ARRAY, size);
    int target = 9999;

    print_array(arr, size < 20 ? size : 20,
                "Before sorting (first 20 elements)");

    clock_t start = clock();
    int result = binary_search(arr, size, target);
    clock_t end = clock();

    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_taken);
    printf("Searched for target number %d, got position %d\n", target, result);

    CU_ASSERT(result == target);
}

void test_binary_search_random_but_ordered(void) {
    printf("\n\033[1;35m=== Testing Binary Search with Sorted Array "
           "(Random But Sorted) ===\033[0m\n");
    
    int size = 1000;
    int* arr = generate_array(RANDOM_ARRAY, size);

    printf("Sorting array of size %d\n", size);
    selection_sort(arr, size);
    printf("Sorting complete.\n");

    int target_index = rand() % size;
    int target_value = arr[target_index];

    print_array(arr, size < 20 ? size : 20,
               "Array to search (first 20 elements)");

    clock_t start = clock();
    int result = binary_search(arr, size, target_value);
    clock_t end = clock();

    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_taken);
    printf("Searched for target number %d, got position %d, "
            "should've been at position %d\n",
            target_value,
            result,
            target_index);

    CU_ASSERT(result == target_index);
}

void test_binary_search_one_element(void) {
    printf("\n\033[1;35m=== Testing Binary Search with One Element "
           "===\033[0m\n");
    
    int size = 1;
    int* arr = generate_array(RANDOM_ARRAY, size);

    int target_index = 0;
    int target_value = arr[target_index];

    print_array(arr, size < 20 ? size : 20,
               "Array to search");

    clock_t start = clock();
    int result = binary_search(arr, size, target_value);
    clock_t end = clock();

    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_taken);
    printf("Searched for target number %d, got position %d, "
            "should've been at position %d\n",
            target_value,
            result,
            target_index);

    CU_ASSERT(result == target_index);
}
