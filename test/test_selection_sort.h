#include <CUnit/Basic.h>
#include "sorting/selection_sort.h"
#include "test_helpers.h"

void test_selection_sort_random(void) {
    printf("\n\033[1;36m=== Testing Selection Sort with "
            "Random Array ===\033[0m\n");

    int size = 10000;
    int* arr = generate_array(RANDOM_ARRAY, size);

    print_array(arr, size < 20 ? size : 20,
                "Before sorting (first 20 elements)");

    clock_t start = clock();
    selection_sort(arr, size);
    clock_t end = clock();

    print_array(arr, size < 20 ? size : 20,
                "After sorting (first 20 elements)");

    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_taken);

    for (int i = 1; i < size; i++) {
        CU_ASSERT(arr[i-1] <= arr[i]);
    }
}

void test_selection_sort_reverse(void) {
    printf("\n\033[1;35m=== Testing Selection Sort with "
            "Reverse Sorted Array ===\033[0m\n");
    
    int size = 10000;
    int* arr = generate_array(REVERSE_SORTED_ARRAY, size);

    print_array(arr, size < 20 ? size : 20,
            "Before sorting (first 20 elements)");

    clock_t start = clock();
    selection_sort(arr, size);
    clock_t end = clock();

    print_array(arr, size < 20 ? size : 20,
            "After sorting (first 20 elements)");

    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_taken);

    for (int i = 1; i < size; i++) {
        CU_ASSERT(arr[i-1] <= arr[i]);
    }
}
