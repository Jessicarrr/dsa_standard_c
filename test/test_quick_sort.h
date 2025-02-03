#include <CUnit/Basic.h>
#include <stdbool.h>

#include "sorting/quick_sort.h"
#include "test_helpers.h"

void test_quick_sort_random(void) {
    printf("\n\033[1;36m=== Testing Quick Sort with "
            "Random Array ===\033[0m\n");

    int size = 20000;
    int* arr = generate_array(RANDOM_ARRAY, size);

    print_array(arr, size < 20 ? size : 20,
                "Before sorting (first 20 elements)");

    clock_t start = clock();
    quick_sort(arr, size);
    clock_t end = clock();

    print_array(arr, size < 20 ? size : 20,
                "After sorting (first 20 elements)");

    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_taken);

    bool is_ordered = is_array_ordered(arr, size);
    CU_ASSERT(is_ordered == true);
}


