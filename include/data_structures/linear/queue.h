#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include "data_structures/linear/ring_buffer.h"
#include "dsc_result.h"

typedef struct Queue {
    struct RingBuffer* buffer;
    size_t length;
} Queue;

enum DscReturnCode enqueue(Queue* queue, void* data);
enum DscReturnCode dequeue(Queue* queue, void* out);

enum DscReturnCode peek(Queue* queue, void* out);

enum DscReturnCode create_queue(size_t item_size, Queue* out);
void queue_destroy(Queue* queue);

void print_queue(Queue* queue, char* (*to_string)(void* data));

void* queue_get_pointer_to(Queue* queue, int index);
int queue_get_value_at(Queue* queue, int index, void* out);

#endif