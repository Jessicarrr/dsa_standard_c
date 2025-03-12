#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include "ring_buffer.h"
#include "dsc_result.h"

typedef struct Queue {
    RingBuffer* buffer;
} Queue;

/**
 * @brief Creates a new Queue.
 *
 * @param item_size The size of each item.
 * @param error Optional pointer to a DscError* for error details.
 * @return A pointer to the new Queue, or NULL if an error occurred.
 */
Queue* create_queue(size_t item_size, DscError** error);

/**
 * @brief Enqueues a new element into the queue.
 *
 * @param queue The Queue pointer.
 * @param data Pointer to the data to enqueue.
 * @param error Optional pointer to a DscError* for error details.
 * @return true if successful, false otherwise.
 */
bool enqueue(Queue* queue, void* data, DscError** error);

/**
 * @brief Dequeues an element from the queue.
 *
 * @param queue The Queue pointer.
 * @param out Pointer to a buffer where the dequeued element is copied.
 * @param error Optional pointer to a DscError* for error details.
 * @return true if successful, false otherwise.
 */
bool dequeue(Queue* queue, void* out, DscError** error);

/**
 * @brief Peeks at the front element of the queue.
 *
 * @param queue The Queue pointer.
 * @param out Pointer to a buffer where the element is copied.
 * @param error Optional pointer to a DscError* for error details.
 * @return true if successful, false otherwise.
 */
bool peek(Queue* queue, void* out, DscError** error);

/**
 * @brief Destroys the queue and frees its resources.
 *
 * @param queue The Queue pointer.
 */
void queue_destroy(Queue* queue);

/**
 * @brief Prints the contents of the queue.
 *
 * @param queue The Queue pointer.
 * @param to_string A function pointer to convert an element to a string.
 */
void print_queue(Queue* queue, char* (*to_string)(void* data));

/**
 * @brief Returns a pointer to the element at the given logical index.
 *
 * @param queue The Queue pointer.
 * @param index The index within the queue (0 <= index < length).
 * @return Pointer to the element, or NULL if the index is out of bounds.
 */
void* queue_get_pointer_to(Queue* queue, int index);

/**
 * @brief Retrieves the value at the specified index in the queue.
 *
 * @param queue The Queue pointer.
 * @param index The index within the queue.
 * @param out Pointer where the element is copied.
 * @param error Optional pointer to a DscError* for error details.
 * @return true if successful, false otherwise.
 */
bool queue_get_value_at(Queue* queue, int index, void* out, DscError** error);

#endif
