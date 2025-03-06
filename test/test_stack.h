#include <stdio.h>
#include <stdlib.h>
#include "data_structures/linear/stack.h"
#include <CUnit/Basic.h>

// Test push and pop operations.
void test_stack_push_pop(void) {
    // Allocate and create a stack.
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    CU_ASSERT_PTR_NOT_NULL(stack);
    enum DscReturnCode result = create_stack(sizeof(int), stack);
    CU_ASSERT_EQUAL(result, DSC_OK);

    // Push several integers.
    int a = 10, b = 20, c = 30;
    result = stack_push(stack, &a);
    CU_ASSERT_EQUAL(result, DSC_OK);
    result = stack_push(stack, &b);
    CU_ASSERT_EQUAL(result, DSC_OK);
    result = stack_push(stack, &c);
    CU_ASSERT_EQUAL(result, DSC_OK);
    CU_ASSERT_EQUAL(stack->length, 3);

    // Peek should return the last pushed value.
    int top = 0;
    result = stack_peek(stack, &top);
    CU_ASSERT_EQUAL(result, DSC_OK);
    CU_ASSERT_EQUAL(top, c);
    
    // Popping should return values in reverse order.
    int popped = 0;
    result = stack_pop(stack, &popped);
    CU_ASSERT_EQUAL(result, DSC_OK);
    CU_ASSERT_EQUAL(popped, c);

    result = stack_pop(stack, &popped);
    CU_ASSERT_EQUAL(result, DSC_OK);
    CU_ASSERT_EQUAL(popped, b);

    result = stack_pop(stack, &popped);
    CU_ASSERT_EQUAL(result, DSC_OK);
    CU_ASSERT_EQUAL(popped, a);
    CU_ASSERT_EQUAL(stack->length, 0);

    stack_destroy(stack);
    free(stack);
}

// Test error conditions for invalid parameters.
void test_stack_invalid_params(void) {
    // Allocate and create a valid stack.
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    CU_ASSERT_PTR_NOT_NULL(stack);
    enum DscReturnCode result = create_stack(sizeof(int), stack);
    CU_ASSERT_EQUAL(result, DSC_OK);

    int value = 5;
    // Passing NULL for the stack parameter.
    result = stack_push(NULL, &value);
    CU_ASSERT_EQUAL(result, DSC_ERROR_INVALID_PARAM);
    result = stack_pop(NULL, &value);
    CU_ASSERT_EQUAL(result, DSC_ERROR_INVALID_PARAM);
    result = stack_peek(NULL, &value);
    CU_ASSERT_EQUAL(result, DSC_ERROR_INVALID_PARAM);

    // Passing NULL for the data parameter.
    result = stack_push(stack, NULL);
    CU_ASSERT_EQUAL(result, DSC_ERROR_INVALID_PARAM);
    result = stack_pop(stack, NULL);
    CU_ASSERT_EQUAL(result, DSC_ERROR_INVALID_PARAM);
    result = stack_peek(stack, NULL);
    CU_ASSERT_EQUAL(result, DSC_ERROR_INVALID_PARAM);

    stack_destroy(stack);
    free(stack);
}

// Test checking if the stack is empty or full.
// Note: In this implementation, stack_is_empty returns true if there are no elements,
// and stack_is_full returns true when there is at least one element.
void test_stack_empty_full(void) {
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    CU_ASSERT_PTR_NOT_NULL(stack);
    enum DscReturnCode result = create_stack(sizeof(int), stack);
    CU_ASSERT_EQUAL(result, DSC_OK);

    // Initially, the stack should be empty.
    CU_ASSERT_TRUE(stack_is_empty(stack));
    CU_ASSERT_FALSE(stack_is_full(stack));

    int value = 42;
    result = stack_push(stack, &value);
    CU_ASSERT_EQUAL(result, DSC_OK);

    // After pushing, stack should not be empty and is considered "full" per the implementation.
    CU_ASSERT_FALSE(stack_is_empty(stack));
    CU_ASSERT_TRUE(stack_is_full(stack));

    stack_destroy(stack);
    free(stack);
}
