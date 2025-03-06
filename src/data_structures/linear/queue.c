#include "data_structures/linear/queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

enum DscReturnCode enqueue(Queue* queue, void* data) {
    enum DscReturnCode result = ring_buffer_insert(queue->buffer, data);
    queue->length = queue->buffer->length;
    return result;

}
enum DscReturnCode dequeue(Queue* queue, void* out) {
    enum DscReturnCode result = ring_buffer_pop_first(queue->buffer, out);
    queue->length = queue->buffer->length;
    return result;
}

enum DscReturnCode peek(Queue* queue, void* out) {
    return ring_buffer_peek_first(queue->buffer, out);
}

enum DscReturnCode create_queue(size_t item_size, Queue* out) {
    out->buffer = (RingBuffer*)malloc(sizeof(RingBuffer));
    enum DscReturnCode result = create_ring_buffer(item_size, out->buffer);
    out->length = 0;
    
    return result;
}

void queue_destroy(Queue* queue) {
    ring_buffer_destroy(queue->buffer);
}

void print_queue(Queue* queue, char* (*to_string)(void* data)) {
    if (queue == NULL || queue->buffer == NULL) {
        printf("Queue or queue->buffer is null.\n");
        return;
    }

    if (to_string == NULL) {
        fprintf(stderr, "print_queue function parameter should not be null.\n");
        return;
    }

    printf("Queue @ %p - length: %zd, data size: %zd, "
        "capacity: %zd, contents:\n",
        (void*)queue->buffer, queue->length, queue->buffer->item_size, queue->buffer->capacity);

    if (queue->length <= 0) {
        printf("Empty Queue.\n");
        return;
    }

    printf("[ ");
    
    for(size_t i = 0; i < queue->buffer->length; i++) {
        void* item = ((uint8_t*)queue->buffer->data) + (i * queue->buffer->item_size);
        char* description = to_string(item);
        printf("%s", description);
        free(description);
        printf(", ");
    }

    printf("]\n");

}