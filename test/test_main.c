#include <CUnit/Basic.h>
#include <stdio.h>
#include <time.h>
#include "test_helpers.h"
#include "test_selection_sort.h"
#include "test_binary_search.h"
#include "test_merge_sort.h"

/* Test Suite setup and cleanup functions */
int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

int main() {
    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    /* Add a suite to the registry */
    CU_pSuite suite = CU_add_suite("Selection Sort Test Suite",
                                   init_suite, clean_suite);
    if (NULL == suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Register all tests */
    CU_Test* test;
    test = CU_add_test(suite, "Random Array Sort",
                       test_selection_sort_random);
    if (test == NULL) goto cleanup;
    
    test = CU_add_test(suite, "Reverse Array Sort",
                       test_selection_sort_reverse);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Sorted Array Binary Search",
                       test_binary_search_ordered);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Binary Search Find First",
                       test_binary_search_find_first);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Binary Search Find Random Sorted",
                       test_binary_search_random_but_ordered);
    if (test == NULL) goto cleanup;
    
    test = CU_add_test(suite, "binary search find one element",
                       test_binary_search_one_element);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "binary search find zero element",
                       test_binary_search_zero_element);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "binary search find negative element",
                       test_binary_search_negative_elements);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "binary search find large gap element",
                       test_binary_search_large_gaps);
    if (test == NULL) goto cleanup;


    test = CU_add_test(suite, "binary search find one of two elements",
                       test_binary_search_two_elements);
    if (test == NULL) goto cleanup;
    
    //test = CU_add_test(suite, "Merge sort random large array",
    //                   test_merge_sort_random);
    //if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Merge sort random small array",
                       test_merge_sort_small);
    if (test == NULL) goto cleanup;

    // Easy to add more tests:
    //
    // test = CU_add_test(suite, "Mostly Sorted Array",
    //                    test_selection_sort_mostly_sorted);
    //
    // if (test == NULL) goto cleanup;

    /* Run all tests using CUnit Basic interface */
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();

cleanup:
    CU_cleanup_registry();
    return CU_get_error();
}
