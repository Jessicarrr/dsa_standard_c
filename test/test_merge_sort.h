#include <stdio.h>
#include "sorting/merge_sort.h"

#include "test_helpers.h"
#include <CUnit/Basic.h>

void test_merge_general(char* name, int size, int array_type) {
    printf("\n\033[1;36m=== Testing Merge Sort: %s ===\033[0m\n",
            name);

    int* arr = generate_array(array_type, size);
    print_array(arr, size < 20 ? size : 20,
                "Before sorting (first 20 elements)");

    clock_t start = clock();
    merge_sort(arr, size);
    clock_t end = clock();

    print_array(arr, size < 20 ? size : 20,
                "After sorting (first 20 elements)");

    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_taken);

    bool is_ordered = is_array_ordered(arr, size);
    CU_ASSERT(is_ordered == true);

}

void test_merge_sort_random(void) {
    test_merge_general("Randomized 50,000 Long Array",
                        50000,
                        RANDOM_ARRAY);
}

void test_merge_sort_small(void) {
    test_merge_general("Small 8 long array",
                        8,
                        RANDOM_ARRAY);
}
