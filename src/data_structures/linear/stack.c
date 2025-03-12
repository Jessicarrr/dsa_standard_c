#include "data_structures/linear/stack.h"
#include <stdio.h>
#include <stdlib.h>

Stack* create_stack(size_t item_size, DscError **error) {
    if (item_size == 0) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "Item size must be greater than zero.");
        }
        return NULL;
    }

    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_MEM_ALLOC, "Failed to allocate memory for stack.");
        }
        return NULL;
    }

    stack->list = create_list(item_size, error);
    if (stack->list == NULL) {
        free(stack);
        return NULL;
    }
    stack->length = stack->list->length;
    return stack;
}

void stack_destroy(Stack* stack) {
    if (stack == NULL) {
        return;
    }
    list_destroy(stack->list);
    free(stack);
}

bool stack_push(Stack* stack, void* item, DscError **error) {
    if (stack == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "Stack is NULL in stack_push.");
        }
        return false;
    }
    if (item == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "Item pointer is NULL in stack_push.");
        }
        return false;
    }
    list_insert(stack->list, item, error);
    if (*error != NULL) {
        return false;
    }
    stack->length = stack->list->length;
    return true;
}

bool stack_pop(Stack* stack, void* out, DscError **error) {
    if (stack == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "Stack is NULL in stack_pop.");
        }
        return false;
    }
    if (out == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "Output pointer is NULL in stack_pop.");
        }
        return false;
    }
    if (stack->list->length == 0) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_IS_EMPTY, "Stack is empty in stack_pop.");
        }
        return false;
    }
    size_t position = stack->list->length - 1;
    if (list_get_value_at(stack->list, position, out, error) != 0) {
        return false;
    }
    list_remove(stack->list, position, error);
    if (*error != NULL) {
        return false;
    }
    stack->length = stack->list->length;
    return true;
}

bool stack_peek(Stack* stack, void* out, DscError **error) {
    if (stack == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "Stack is NULL in stack_peek.");
        }
        return false;
    }
    if (out == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "Output pointer is NULL in stack_peek.");
        }
        return false;
    }
    if (stack->list->length == 0) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_IS_EMPTY, "Stack is empty in stack_peek.");
        }
        return false;
    }
    size_t position = stack->list->length - 1;
    if (list_get_value_at(stack->list, position, out, error) != 0) {
        return false;
    }
    return true;
}

bool stack_is_empty(Stack* stack, DscError **error) {
    if (stack == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "Stack is NULL in stack_is_empty.");
        }
        return true;
    }
    return (stack->length == 0);
}

bool stack_is_full(Stack* stack, DscError **error) {
    if (stack == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "Stack is NULL in stack_is_full.");
        }
        return false;
    }
    
    if (stack->list->capacity == 0) {
        return false;
    }

    size_t slots_left = stack->list->capacity - stack->list->length;

    //printf("Stack debug: slots_left = %zd\n", slots_left);
    if (slots_left == 0) {
        //printf("Returning true - it is full.\n");
        return true;
    }

    //printf("Returning false - it is not full.\n");
    return false;
   
}
