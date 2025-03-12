#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdio.h>
#include <stdbool.h>
#include "dsc_result.h"

typedef struct RingBuffer {
    void* data;
    size_t length;
    size_t capacity;
    size_t item_size;
    size_t head;
    size_t next_position;
} RingBuffer;

/**
 * @brief Creates a new RingBuffer.
 *
 * @param item_size The size of each item stored.
 * @param error Optional address to a DscError pointer for error details.
 * @return A pointer to a new RingBuffer or NULL if an error occurred.
 */
RingBuffer* create_ring_buffer(size_t item_size, DscError** error);

/**
 * @brief Inserts a new element into the ring buffer.
 *
 * @param buffer The RingBuffer to insert into.
 * @param data Pointer to the data to insert.
 * @param error Optional address to a DscError pointer for error details.
 * @return true if successful, false otherwise.
 */
bool ring_buffer_insert(RingBuffer* buffer, void* data, DscError** error);

/**
 * @brief Removes and returns the first element from the ring buffer.
 *
 * @param buffer The RingBuffer to remove the element from.
 * @param out Pointer to a buffer where the removed item will be copied.
 * @param error Optional address to a DscError pointer for error details.
 * @return true if successful, false otherwise.
 */
bool ring_buffer_pop_first(RingBuffer* buffer, void* out, DscError** error);

/**
 * @brief Removes and returns the last element from the ring buffer.
 *
 * @param buffer The RingBuffer to remove the element from.
 * @param out Pointer to a buffer where the removed item will be copied.
 * @param error Optional address to a DscError pointer for error details.
 * @return true if successful, false otherwise.
 */
bool ring_buffer_pop_last(RingBuffer* buffer, void* out, DscError** error);

/**
 * @brief Peeks at the first element in the ring buffer.
 *
 * @param buffer The RingBuffer to peek into.
 * @param out Pointer to a buffer where the item will be copied.
 * @param error Optional address to a DscError pointer for error details.
 * @return true if successful, false otherwise.
 */
bool ring_buffer_peek_first(RingBuffer* buffer, void* out, DscError** error);

/**
 * @brief Peeks at the last element in the ring buffer.
 *
 * @param buffer The RingBuffer to peek into.
 * @param out Pointer to a buffer where the item will be copied.
 * @param error Optional address to a DscError pointer for error details.
 * @return true if successful, false otherwise.
 */
bool ring_buffer_peek_last(RingBuffer* buffer, void* out, DscError** error);

/**
 * @brief Prints the contents of the ring buffer.
 *
 * @param buffer The RingBuffer to print.
 * @param to_string A function pointer to convert an item to a string.
 */
void print_ring_buffer(RingBuffer* buffer, char* (*to_string)(void* item));

/**
 * @brief Destroys the ring buffer, freeing its resources.
 *
 * @param buffer The RingBuffer to destroy.
 */
void ring_buffer_destroy(RingBuffer* buffer);

bool is_full(RingBuffer* buffer);
bool is_empty(RingBuffer* buffer);

#endif
