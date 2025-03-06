#include <CUnit/Basic.h>
#include <stdio.h>
#include <time.h>
#include "test_helpers.h"
#include "test_selection_sort.h"
#include "test_binary_search.h"
#include "test_quick_sort.h"
#include "test_merge_sort.h"
#include "test_list.h"
#include "test_ring_buffer.h"
#include "test_queue.h"
#include "test_stack.h"

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

    test = CU_add_test(suite, "Quick sort sorted array",
                       test_quick_sort_random);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Simple create list",
                       test_create_small_list);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Simple create larger list",
                       test_create_larger_list);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Create list and remove elements",
                       test_create_list_and_remove);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Create list and remove many elements",
                       test_create_list_and_remove_many);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Remove from nulll list",
                       test_remove_from_null_list);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Remove from empty list",
                       test_remove_from_empty_list);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Remove last element",
                       test_list_remove_last_element);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Remove duplicate values in list",
                       test_list_remove_duplicate_values);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Remove negative index from list",
                       test_list_remove_negative_index);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "List add remove over time",
                       test_list_add_remove_over_time);
    if (test == NULL) goto cleanup;    
    
    test = CU_add_test(suite, "List place at index",
        test_list_place_at_index);
    if (test == NULL) goto cleanup; 

    test = CU_add_test(suite, "List of structs",
        test_list_of_structs);
    if (test == NULL) goto cleanup;   

    test = CU_add_test(suite, "Generate small ring buffer", test_create_small_ring_buffer);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Insert and remove first in ring buffer", test_ring_buffer_insert_and_remove_first);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Insert and remove last in ring buffer", test_ring_buffer_insert_and_remove_last);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Auto resize increase in ring buffer", test_ring_buffer_auto_resize_increase);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Auto resize decrease in ring buffer", test_ring_buffer_auto_resize_decrease);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Remove from empty ring buffer", test_remove_from_empty_ring_buffer);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Invalid parameters in ring buffer operations", test_invalid_params_ring_buffer);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Ring buffer wrap-around test", test_ring_buffer_wrap_around);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "enqueue and dequeue", test_enqueue_dequeue);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Test peek", test_peek);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Test dequeue on empty queue", test_dequeue_empty);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Test enqueue with null pointer", test_enqueue_null);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Test dynamic resize", test_dynamic_resize);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Test alternating operations", test_alternating_operations);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Test create queue with invalid item size", test_create_queue_invalid_item_size);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Stack push and pop", test_stack_push_pop);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Stack invalid parameters", test_stack_invalid_params);
    if (test == NULL) goto cleanup;

    test = CU_add_test(suite, "Stack empty/full state", test_stack_empty_full);
    if (test == NULL) goto cleanup;




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
