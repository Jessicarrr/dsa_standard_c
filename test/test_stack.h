#include <stdio.h>
#include <stdlib.h>
#include "data_structures/linear/stack.h"
#include <CUnit/Basic.h>
#include "dsc_result.h"

// Test push and pop operations.
void test_stack_push_pop(void) {
    DscError *error = NULL;
    // Create a new stack.
    Stack *stack = create_stack(sizeof(int), &error);
    CU_ASSERT_PTR_NOT_NULL_FATAL(stack);
    if (error) {
        fprintf(stderr, "Error in create_stack: %s\n", error->message);
        free(error);
        CU_FAIL("create_stack failed");
        return;
    }

    // Push several integers.
    int a = 10, b = 20, c = 30;
    bool success = stack_push(stack, &a, &error);
    CU_ASSERT_TRUE(success);
    if (error) {
        fprintf(stderr, "Error in stack_push (a): %s\n", error->message);
        free(error);
    }
    success = stack_push(stack, &b, &error);
    CU_ASSERT_TRUE(success);
    if (error) {
        fprintf(stderr, "Error in stack_push (b): %s\n", error->message);
        free(error);
    }
    success = stack_push(stack, &c, &error);
    CU_ASSERT_TRUE(success);
    if (error) {
        fprintf(stderr, "Error in stack_push (c): %s\n", error->message);
        free(error);
    }
    CU_ASSERT_EQUAL(stack->length, 3);

    // Peek should return the last pushed value.
    int top = 0;
    success = stack_peek(stack, &top, &error);
    CU_ASSERT_TRUE(success);
    if (error) {
        fprintf(stderr, "Error in stack_peek: %s\n", error->message);
        free(error);
    }
    CU_ASSERT_EQUAL(top, c);
    
    // Popping should return values in reverse order.
    int popped = 0;
    success = stack_pop(stack, &popped, &error);
    CU_ASSERT_TRUE(success);
    if (error) {
        fprintf(stderr, "Error in stack_pop: %s\n", error->message);
        free(error);
    }
    CU_ASSERT_EQUAL(popped, c);

    success = stack_pop(stack, &popped, &error);
    CU_ASSERT_TRUE(success);
    if (error) {
        fprintf(stderr, "Error in stack_pop: %s\n", error->message);
        free(error);
    }
    CU_ASSERT_EQUAL(popped, b);

    success = stack_pop(stack, &popped, &error);
    CU_ASSERT_TRUE(success);
    if (error) {
        fprintf(stderr, "Error in stack_pop: %s\n", error->message);
        free(error);
    }
    CU_ASSERT_EQUAL(popped, a);
    CU_ASSERT_EQUAL(stack->length, 0);

    stack_destroy(stack);
}

// Test error conditions for invalid parameters.
void test_stack_invalid_params(void) {
    DscError *error = NULL;
    // Create a valid stack.
    Stack *stack = create_stack(sizeof(int), &error);
    CU_ASSERT_PTR_NOT_NULL_FATAL(stack);
    if (error) {
        fprintf(stderr, "Error in create_stack: %s\n", error->message);
        free(error);
    }

    int value = 5;
    bool success = false;
    
    // Passing NULL for the stack parameter.
    success = stack_push(NULL, &value, &error);
    CU_ASSERT_FALSE(success);
    if (error) {
        fprintf(stderr, "Expected error in stack_push(NULL,...): %s\n", error->message);
        free(error);
    }
    success = stack_pop(NULL, &value, &error);
    CU_ASSERT_FALSE(success);
    if (error) {
        fprintf(stderr, "Expected error in stack_pop(NULL,...): %s\n", error->message);
        free(error);
    }
    success = stack_peek(NULL, &value, &error);
    CU_ASSERT_FALSE(success);
    if (error) {
        fprintf(stderr, "Expected error in stack_peek(NULL,...): %s\n", error->message);
        free(error);
    }

    // Passing NULL for the data parameter.
    success = stack_push(stack, NULL, &error);
    CU_ASSERT_FALSE(success);
    if (error) {
        fprintf(stderr, "Expected error in stack_push(stack, NULL): %s\n", error->message);
        free(error);
    }
    success = stack_pop(stack, NULL, &error);
    CU_ASSERT_FALSE(success);
    if (error) {
        fprintf(stderr, "Expected error in stack_pop(stack, NULL): %s\n", error->message);
        free(error);
    }
    success = stack_peek(stack, NULL, &error);
    CU_ASSERT_FALSE(success);
    if (error) {
        fprintf(stderr, "Expected error in stack_peek(stack, NULL): %s\n", error->message);
        free(error);
    }

    stack_destroy(stack);
}

// Test checking if the stack is empty or full.
// For a dynamic stack, is_empty returns true if there are no elements,
// and is_full always returns false (since it grows dynamically).
void test_stack_empty_full(void) {
    DscError *error = NULL;
    Stack *stack = create_stack(sizeof(int), &error);
    CU_ASSERT_PTR_NOT_NULL_FATAL(stack);
    if (error) {
        fprintf(stderr, "Error in create_stack: %s\n", error->message);
        free(error);
    }

    // Initially, the stack should be empty.
    bool empty = stack_is_empty(stack, &error);
    CU_ASSERT_EQUAL(empty, 1);
    bool full = stack_is_full(stack, &error);
    CU_ASSERT_EQUAL(full, 0);

    int value = 42;
    bool success = stack_push(stack, &value, &error);
    CU_ASSERT_TRUE(success);
    if (error) {
        fprintf(stderr, "Error in stack_push: %s\n", error->message);
        free(error);
    }

    printf("Should not be empty...\n");
    empty = stack_is_empty(stack, &error);
    CU_ASSERT_EQUAL(empty, 0);

    printf("Should not be full...");
    full = stack_is_full(stack, &error);
    printf("'full' value: %d", full);
    CU_ASSERT_EQUAL(full, 0);  // For a dynamic stack, full should always be false.

    stack_destroy(stack);
}
