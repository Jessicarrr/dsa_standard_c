#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdio.h>
#include <stdbool.h>
#include "dsc_return_codes.h"

typedef struct RingBuffer {
    void* data;
    size_t length;
    size_t capacity;
    size_t item_size;
    size_t head;
    size_t next_position;

} RingBuffer;

enum DscReturnCode create_ring_buffer(size_t item_size, RingBuffer* out);

enum DscReturnCode ring_buffer_insert(RingBuffer* buffer, void* data);

enum DscReturnCode ring_buffer_pop_first(RingBuffer* buffer, void* out);
enum DscReturnCode ring_buffer_pop_last(RingBuffer* buffer, void* out);
enum DscReturnCode ring_buffer_peek_first(RingBuffer* buffer, void* out);
enum DscReturnCode ring_buffer_peek_last(RingBuffer* buffer, void* out);

void print_ring_buffer(RingBuffer* buffer, char* (*to_string)(void* item));

bool is_full(RingBuffer* buffer);
bool is_empty(RingBuffer* buffer);

void ring_buffer_destroy(RingBuffer* buffer);

#endif