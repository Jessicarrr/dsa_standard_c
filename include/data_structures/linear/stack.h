#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include "data_structures/linear/list.h"
#include "dsc_result.h"

typedef struct Stack {
    List* list;
    size_t length;
} Stack;

/**
 * @brief Creates a new stack.
 *
 * @param item_size The size of each item.
 * @param error Optional pointer to a DscError pointer; on error, a new error is allocated.
 * @return Pointer to a new Stack, or NULL if an error occurs.
 */
Stack* create_stack(size_t item_size, DscError **error);

/**
 * @brief Destroys the stack and frees its resources.
 *
 * @param stack The stack to destroy.
 */
void stack_destroy(Stack* stack);

/**
 * @brief Pushes an item onto the stack.
 *
 * @param stack The stack.
 * @param item Pointer to the item to push.
 * @param error Optional pointer to a DscError pointer.
 * @return true on success, false on error.
 */
bool stack_push(Stack* stack, void* item, DscError **error);

/**
 * @brief Pops the top item from the stack.
 *
 * The item is copied into the memory pointed to by out.
 *
 * @param stack The stack.
 * @param out Pointer where the popped item will be copied.
 * @param error Optional pointer to a DscError pointer.
 * @return true on success, false on error.
 */
bool stack_pop(Stack* stack, void* out, DscError **error);

/**
 * @brief Peeks at the top item on the stack without removing it.
 *
 * The item is copied into the memory pointed to by out.
 *
 * @param stack The stack.
 * @param out Pointer where the item will be copied.
 * @param error Optional pointer to a DscError pointer.
 * @return true on success, false on error.
 */
bool stack_peek(Stack* stack, void* out, DscError **error);

/**
 * @brief Checks whether the stack is empty.
 *
 * @param stack The stack.
 * @param error Optional pointer to a DscError pointer.
 * @return true if empty, false if not or if an error occurs.
 */
bool stack_is_empty(Stack* stack, DscError **error);

/**
 * @brief Checks whether the stack is full.
 *
 * For a dynamic stack this is always false if the stack is valid.
 *
 * @param stack The stack.
 * @param error Optional pointer to a DscError pointer.
 * @return false if not full; if stack is NULL, an error is reported and false is returned.
 */
bool stack_is_full(Stack* stack, DscError **error);

#endif
