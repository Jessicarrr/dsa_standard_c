#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdio.h>
#include <stdbool.h>
#include "data_structures/linear/ring_buffer_return_codes.h"

typedef struct RingBuffer {
    void* data;
    size_t length;
    size_t capacity;
    size_t item_size;
    size_t head;
    size_t next_position;

} RingBuffer;

enum RingBufferReturnCode create_ring_buffer(size_t item_size, RingBuffer* out);

enum RingBufferReturnCode ring_buffer_insert(RingBuffer* buffer, void* data);

enum RingBufferReturnCode get_and_remove_first(RingBuffer* buffer, void* out);
enum RingBufferReturnCode get_and_remove_last(RingBuffer* buffer, void* out);
enum RingBufferReturnCode get_first(RingBuffer* buffer, void* out);
enum RingBufferReturnCode get_last(RingBuffer* buffer, void* out);

void print_ring_buffer(RingBuffer* buffer, char* (*to_string)(void* item));

bool is_full(RingBuffer* buffer);
bool is_empty(RingBuffer* buffer);

void ring_buffer_destroy(RingBuffer* buffer);

#endif