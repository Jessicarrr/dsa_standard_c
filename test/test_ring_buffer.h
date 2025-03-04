#ifndef TEST_RING_BUFFER_H
#define TEST_RING_BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "data_structures/linear/ring_buffer.h"
#include "test_helpers.h"

static char* int_item_to_string(void* item) {
    int* as_int = (int*) item;
    char* buffer = (char*)malloc(1024);

    snprintf(buffer, 1024,
        "%d", *as_int);

    return buffer;
}

// Test: Create a small ring buffer and insert a few ints
void test_create_small_ring_buffer(void) {
    char name[] = "Generate small ring buffer";
    printf("\n\033[1;35m=== Testing Ring Buffer: %s ===\033[0m\n", name);
    
    RingBuffer buffer;
    CU_ASSERT_EQUAL(create_ring_buffer(sizeof(int), &buffer), DSC_OK);
    
    for (int i = 0; i < 5; i++) {
        CU_ASSERT_EQUAL(ring_buffer_insert(&buffer, &i), DSC_OK);
    }
    
    CU_ASSERT_EQUAL(buffer.length, 5);
    print_ring_buffer(&buffer, int_item_to_string);
    ring_buffer_destroy(&buffer);
}

// Test: Insert elements then remove them using get_and_remove_first
void test_ring_buffer_insert_and_remove_first(void) {
    char name[] = "Insert and remove first in ring buffer";
    printf("\n\033[1;35m=== Testing Ring Buffer: %s ===\033[0m\n", name);
    
    RingBuffer buffer;
    CU_ASSERT_EQUAL(create_ring_buffer(sizeof(int), &buffer), DSC_OK);
    
    for (int i = 0; i < 10; i++) {
        CU_ASSERT_EQUAL(ring_buffer_insert(&buffer, &i), DSC_OK);
    }
    
    int value = -1;
    for (int i = 0; i < 10; i++) {
        CU_ASSERT_EQUAL(get_and_remove_first(&buffer, &value), DSC_OK);
        CU_ASSERT_EQUAL(value, i);
    }
    
    print_ring_buffer(&buffer, int_item_to_string);
    ring_buffer_destroy(&buffer);
}

// Test: Insert elements then remove them using get_and_remove_last
void test_ring_buffer_insert_and_remove_last(void) {
    char name[] = "Insert and remove last in ring buffer";
    printf("\n\033[1;35m=== Testing Ring Buffer: %s ===\033[0m\n", name);
    
    RingBuffer buffer;
    CU_ASSERT_EQUAL(create_ring_buffer(sizeof(int), &buffer), DSC_OK);
    
    for (int i = 0; i < 10; i++) {
        CU_ASSERT_EQUAL(ring_buffer_insert(&buffer, &i), DSC_OK);
    }
    
    int value = -1;
    // Since get_and_remove_last removes the most recently inserted element,
    // we expect the values in reverse order.
    for (int i = 9; i >= 0; i--) {
        CU_ASSERT_EQUAL(get_and_remove_last(&buffer, &value), DSC_OK);
        CU_ASSERT_EQUAL(value, i);
    }
    
    print_ring_buffer(&buffer, int_item_to_string);
    ring_buffer_destroy(&buffer);
}

// Test: Inserting enough elements triggers auto resize (increase capacity)
void test_ring_buffer_auto_resize_increase(void) {
    char name[] = "Auto resize increase in ring buffer";
    printf("\n\033[1;35m=== Testing Ring Buffer: %s ===\033[0m\n", name);
    
    RingBuffer buffer;
    CU_ASSERT_EQUAL(create_ring_buffer(sizeof(int), &buffer), DSC_OK);
    
    // Initial capacity is CAPACITY_START (8); insert 9 elements to force a resize.
    for (int i = 0; i < 9; i++) {
        CU_ASSERT_EQUAL(ring_buffer_insert(&buffer, &i), DSC_OK);
    }
    
    CU_ASSERT_TRUE(buffer.capacity > 8);
    CU_ASSERT_EQUAL(buffer.length, 9);
    
    print_ring_buffer(&buffer, int_item_to_string);
    ring_buffer_destroy(&buffer);
}

// Test: Removing elements triggers auto resize (decrease capacity)
void test_ring_buffer_auto_resize_decrease(void) {
    char name[] = "Auto resize decrease in ring buffer";
    printf("\n\033[1;35m=== Testing Ring Buffer: %s ===\033[0m\n", name);
    
    RingBuffer buffer;
    CU_ASSERT_EQUAL(create_ring_buffer(sizeof(int), &buffer), DSC_OK);
    
    // Insert enough elements to force an increase in capacity.
    for (int i = 0; i < 12; i++) {
        CU_ASSERT_EQUAL(ring_buffer_insert(&buffer, &i), DSC_OK);
    }
    
    int value = -1;
    // Remove 10 elements, leaving 2; if conditions are met, capacity should decrease.
    for (int i = 0; i < 10; i++) {
        CU_ASSERT_EQUAL(get_and_remove_first(&buffer, &value), DSC_OK);
    }
    
    // The capacity should have been decreased if possible (from, say, 16 down to a lower value).
    CU_ASSERT_TRUE(buffer.capacity < 16);
    CU_ASSERT_EQUAL(buffer.length, 2);
    
    print_ring_buffer(&buffer, int_item_to_string);
    ring_buffer_destroy(&buffer);
}

// Test: Attempt to remove from an empty ring buffer
void test_remove_from_empty_ring_buffer(void) {
    char name[] = "Remove from empty ring buffer";
    printf("\n\033[1;35m=== Testing Ring Buffer: %s ===\033[0m\n", name);
    
    RingBuffer buffer;
    CU_ASSERT_EQUAL(create_ring_buffer(sizeof(int), &buffer), DSC_OK);
    
    int value = -1;
    CU_ASSERT_EQUAL(get_and_remove_first(&buffer, &value), DSC_ERROR_IS_EMPTY);
    CU_ASSERT_EQUAL(get_and_remove_last(&buffer, &value), DSC_ERROR_IS_EMPTY);
    
    print_ring_buffer(&buffer, int_item_to_string);
    ring_buffer_destroy(&buffer);
}

// Test: Pass invalid parameters to ring buffer functions
void test_invalid_params_ring_buffer(void) {
    char name[] = "Invalid parameters in ring buffer operations";
    printf("\n\033[1;35m=== Testing Ring Buffer: %s ===\033[0m\n", name);
    
    RingBuffer buffer;
    CU_ASSERT_EQUAL(create_ring_buffer(sizeof(int), &buffer), DSC_OK);
    
    int value = 42;
    // Passing NULL data pointer to insert.
    CU_ASSERT_EQUAL(ring_buffer_insert(&buffer, NULL), DSC_ERROR_INVALID_PARAM);
    // Passing NULL buffer to insert.
    CU_ASSERT_EQUAL(ring_buffer_insert(NULL, &value), DSC_ERROR_INVALID_PARAM);
    // Passing NULL out pointer to get_and_remove_first/last.
    CU_ASSERT_EQUAL(get_and_remove_first(&buffer, NULL), DSC_ERROR_INVALID_PARAM);
    CU_ASSERT_EQUAL(get_and_remove_last(&buffer, NULL), DSC_ERROR_INVALID_PARAM);
    
    print_ring_buffer(&buffer, int_item_to_string);
    ring_buffer_destroy(&buffer);
}

// Test: Check the circular (wrap-around) behavior of the ring buffer
void test_ring_buffer_wrap_around(void) {
    char name[] = "Ring buffer wrap-around test";
    printf("\n\033[1;35m=== Testing Ring Buffer: %s ===\033[0m\n", name);
    
    RingBuffer buffer;
    CU_ASSERT_EQUAL(create_ring_buffer(sizeof(int), &buffer), DSC_OK);
    
    // Fill the ring buffer to its initial capacity (8) with values 0..7.
    for (int i = 0; i < 8; i++) {
        CU_ASSERT_EQUAL(ring_buffer_insert(&buffer, &i), DSC_OK);
    }
    
    int value = -1;
    // Remove first 3 elements; expect 0, 1, 2.
    for (int i = 0; i < 3; i++) {
        CU_ASSERT_EQUAL(get_and_remove_first(&buffer, &value), DSC_OK);
        CU_ASSERT_EQUAL(value, i);
    }
    
    // Insert 3 new elements.
    int new_vals[3] = {100, 101, 102};
    for (int i = 0; i < 3; i++) {
        CU_ASSERT_EQUAL(ring_buffer_insert(&buffer, &new_vals[i]), DSC_OK);
    }
    
    // Now the expected removal order should be the remaining from the original (3,4,5,6,7)
    // followed by the newly inserted elements (100,101,102).
    int expected[] = {3, 4, 5, 6, 7, 100, 101, 102};
    for (int i = 0; i < 8; i++) {
        CU_ASSERT_EQUAL(get_and_remove_first(&buffer, &value), DSC_OK);
        CU_ASSERT_EQUAL(value, expected[i]);
    }
    
    print_ring_buffer(&buffer, int_item_to_string);
    ring_buffer_destroy(&buffer);
}

#endif
