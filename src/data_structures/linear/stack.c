#include "data_structures/linear/stack.h"
#include <stdio.h>

enum DscReturnCode create_stack(size_t item_size, Stack* out) {
    if (out == NULL) {
        fprintf(stderr, "Could not allocate enough memory to create a Stack. "
            "Not enough system memory available?");
        return DSC_ERROR_MEM_ALLOC;
    }

    out->list = create_list(item_size);
    return DSC_OK;
}

enum DscReturnCode stack_destroy(Stack* stack) {
    list_destroy(stack->list);
    return DSC_OK;
}

enum DscReturnCode stack_push(Stack* stack, void* out) {
    if (stack == NULL) {
        fprintf(stderr, "Stack: the 'stack' parameter was null, "
            "cannot continue with push operation.");
        return DSC_ERROR_INVALID_PARAM;
    }
    if (out == NULL) {
        fprintf(stderr, "Stack: the 'out' param was null, cannot "
            "continue with push operation.");
        return DSC_ERROR_INVALID_PARAM;

    }

    list_insert(stack->list, out);
    stack->length = stack->list->length;
    return DSC_OK;
}

enum DscReturnCode stack_pop(Stack* stack, void* out) {
    if (stack == NULL) {
        fprintf(stderr, "Stack: the 'stack' parameter was null, "
            "cannot continue with pop operation.");
        return DSC_ERROR_INVALID_PARAM;
    }
    if (out == NULL) {
        fprintf(stderr, "Stack: the 'out' param was null, cannot "
            "continue with pop operation.");
        return DSC_ERROR_INVALID_PARAM;
    }

    size_t position = stack->list->length - 1;
    list_get_value_at(stack->list, position, out);
    list_remove(stack->list, position);
    stack->length = stack->list->length;

    return DSC_OK;
}

enum DscReturnCode stack_peek(Stack* stack, void* out) {
    if (stack == NULL) {
        fprintf(stderr, "Stack: the 'stack' parameter was null, "
            "cannot continue with peek operation.");
        return DSC_ERROR_INVALID_PARAM;
    }
    if (out == NULL) {
        fprintf(stderr, "Stack: the 'out' param was null, cannot "
            "continue with peek operation.");
        return DSC_ERROR_INVALID_PARAM;
    }

    size_t position = stack->list->length - 1;
    list_get_value_at(stack->list, position, out);

    return DSC_OK;
}

bool stack_is_empty(Stack* stack) {
    if (stack == NULL) {
        fprintf(stderr, "Stack: the 'stack' parameter was null, "
            "cannot continue with 'is empty' operation.");
        return DSC_ERROR_INVALID_PARAM;
    }
    return stack->length == 0;
}

bool stack_is_full(Stack* stack) {
    if (stack == NULL) {
        fprintf(stderr, "Stack: the 'stack' parameter was null, "
            "cannot continue with 'is full' operation.");
        return DSC_ERROR_INVALID_PARAM;
    }
    return stack->length != 0;
}