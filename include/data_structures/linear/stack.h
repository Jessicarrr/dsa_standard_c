#ifndef STACK_H
#define QUEUE_H

#include <stdbool.h>
#include "data_structures/linear/list.h"
#include "dsc_return_codes.h"

typedef struct Stack {
    List* list;
    size_t length;

} Stack;

enum DscReturnCode create_stack(size_t item_size, Stack* out);
enum DscReturnCode stack_destroy(Stack* stack);

enum DscReturnCode stack_push(Stack* stack, void* out);
enum DscReturnCode stack_pop(Stack* stack, void* out);

enum DscReturnCode stack_peek(Stack* stack, void* out);

bool stack_is_empty(Stack* stack);
bool stack_is_full(Stack* stack);

#endif