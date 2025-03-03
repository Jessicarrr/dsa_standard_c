#include "data_structures/linear/ring_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define CAPACITY_MULTIPLY 2
#define CAPACITY_START 8
#define BUFFER_GROW_THRESHOLD 1

enum DsaReturnCode create_ring_buffer(size_t item_size, RingBuffer* out) {
    if (out == NULL) {
        fprintf(stderr, "Could not create ring buffer because 'out' param "
                "was null.\n");
        return DSA_ERROR_INVALID_PARAM;
    }

    if (item_size == 0) {
        fprintf(stderr, "Cannot create ring buffer with item_size of zero.\n");
        return DSA_ERROR_INVALID_PARAM;
    }

    out->item_size = item_size;
    out->head = 0;
    out->next_position = 0;
    out->capacity = CAPACITY_START;
    out->length = 0;
    out->data = malloc(CAPACITY_START * item_size);

    if (out->data == NULL) {
        fprintf(stderr, "Could not create ring buffer, allocating memory to the data block failed.\n");
        return DSA_ERROR_MEM_ALLOC;
    }

    return DSA_OK;
}

static bool increase_ring_capacity(RingBuffer* buffer) {
    void* new_data = malloc(
        buffer->item_size * 
        (buffer->capacity * CAPACITY_MULTIPLY)
    );

    if (new_data == NULL) {
        fprintf(stderr, "Could not create new ring buffer capacity, "
                "potentially out of memory on the system.\n");
        return false;
    }

    if (buffer->length > 0) {
        for(size_t i = 0; i < buffer->length; i++) {
            size_t index = buffer->head + i;

            if(index >= buffer->capacity) {
                index -= buffer->capacity;
            }

            uint8_t* fetch_address = ((uint8_t*)buffer->data) + (index * buffer->item_size);

            uint8_t* place_address = ((uint8_t*)new_data) + (i * buffer->item_size);
            
            memcpy(place_address, fetch_address, buffer->item_size);
        }
    }

    free(buffer->data);
    buffer->data = new_data;
    buffer->capacity = buffer->capacity * CAPACITY_MULTIPLY;
    buffer->head = 0;
    buffer->next_position = buffer->length;
    return true;
}

static bool decrease_ring_capacity(RingBuffer* buffer) {
    if (buffer->capacity <= CAPACITY_START) {
        return false;
    }

    size_t new_capacity = buffer->capacity / CAPACITY_MULTIPLY;

    if (new_capacity == 0) {
        return false;
    }

    if (buffer->length > new_capacity) {
        fprintf(stderr, "decrease_ring_capacity called when the length of "
                        "the RingBuffer would end up being greater than the "
                        "capacity. The operation has been stopped because "
                        "of the potential for data loss.\n");
        return false;
    }
    
    void* new_data = malloc(
        buffer->item_size *
        new_capacity
    );

    if (new_data == NULL) {
        fprintf(stderr, "Could not create new RingBuffer capacity, "
                "potentially out of memory on the system.\n");
        return false;
    }

    size_t index = buffer->head;

    for(size_t i = 0; i < buffer->length; i++) {
        uint8_t* target = ((uint8_t*)new_data) + (i * buffer->item_size);
        uint8_t* read = ((uint8_t*)buffer->data) + (index * buffer->item_size);

        memcpy(target, read, buffer->item_size);

        index++;

        if(index >= buffer->capacity) {
            index = 0;
        }
    }

    free(buffer->data);
    buffer->data = new_data;
    buffer->capacity = new_capacity;
    buffer->head = 0;
    buffer->next_position = buffer->length;
    return true;
}

static size_t get_num_free_positions(RingBuffer* buffer) {
    return buffer->capacity - buffer->length;
}

bool is_full(RingBuffer* buffer) {
    return get_num_free_positions(buffer) == 0;
}

bool is_empty(RingBuffer* buffer) {
    return get_num_free_positions(buffer) == buffer->capacity;
}

static size_t get_next_position(RingBuffer* buffer) {
    return (buffer->next_position + 1) % buffer->capacity;
}

static bool should_increase_capacity(RingBuffer* buffer) {
    int free_positions = get_num_free_positions(buffer);

    if (free_positions <= BUFFER_GROW_THRESHOLD) {
        return true;
    }

    return false;
}

static bool should_decrease_capacity(RingBuffer* buffer) {
    if (buffer->capacity <= CAPACITY_START) {
        return false;
    }

    if (buffer->length <= (buffer->capacity / 6)) {
        return true;
    }

    return false;
}

enum DsaReturnCode ring_buffer_insert(RingBuffer* buffer, void* data) {
    if (buffer == NULL) {
        fprintf(stderr, "ring_buffer_insert buffer param is null, cannot insert.\n");
        return DSA_ERROR_INVALID_PARAM;
    }

    if (data == NULL) {
        fprintf(stderr, "Cannot put null value into RingBuffer.\n");
        return DSA_ERROR_INVALID_PARAM;
    }

    if (should_increase_capacity(buffer) == true) {
        bool success = increase_ring_capacity(buffer);

        if (success == false) {
            fprintf(stderr, "Could not allocate memory for ring buffer.\n");
            return DSA_ERROR_MEM_ALLOC;
        }
    }

    uint8_t* target_address = ((uint8_t*)buffer->data) + 
            (buffer->next_position * buffer->item_size);

    memcpy(target_address, data, buffer->item_size);
    buffer->length += 1;
    buffer->next_position = get_next_position(buffer);
    return DSA_OK;
}

enum DsaReturnCode get_and_remove_first(RingBuffer* buffer, void* out) {
    if (buffer == NULL) {
        fprintf(stderr, "get_first in RingBuffer, 'buffer' param is null\n");
        return DSA_ERROR_INVALID_PARAM;
    }

    if (out == NULL) {
        fprintf(stderr, "get_and_remove_first in RingBuffer, 'out' param is null.\n");
        return DSA_ERROR_INVALID_PARAM;
    }

    if (is_empty(buffer)) {
        fprintf(stderr, "Cannot remove from an empty ring buffer\n");
        return DSA_ERROR_IS_EMPTY;
    }

    uint8_t* head_address = ((uint8_t*)buffer->data) + (buffer->head * buffer->item_size);
    memcpy(out, head_address, buffer->item_size);

    buffer->head += 1;
    buffer->length -= 1;

    if (buffer->head >= buffer->capacity) {
        buffer->head = 0;
    }

    if (should_decrease_capacity(buffer)) {
        decrease_ring_capacity(buffer);
    }

    if(is_empty(buffer)) {
        buffer->head = 0;
        buffer->next_position = 0;
    }

    return DSA_OK;
}

enum DsaReturnCode get_and_remove_last(RingBuffer* buffer, void* out) {
    if (buffer == NULL) {
        fprintf(stderr, "get_first in RingBuffer, 'buffer' param is null\n");
        return DSA_ERROR_INVALID_PARAM;
    }

    if (out == NULL) {
        fprintf(stderr, "get_and_remove_last in RingBuffer, 'out' param is null\n");
        return DSA_ERROR_INVALID_PARAM;
    }
    
    if (is_empty(buffer)) {
        fprintf(stderr, "Cannot remove from an empty ring buffer\n");
        return DSA_ERROR_IS_EMPTY;
    }

    size_t last_index = (buffer->next_position + buffer->capacity - 1) % buffer->capacity;
    uint8_t* tail_address = ((uint8_t*)buffer->data) + (last_index * buffer->item_size);

    memcpy(out, tail_address, buffer->item_size);

    buffer->length -= 1;
    buffer->next_position = last_index;
    
    if (should_decrease_capacity(buffer)) {
        decrease_ring_capacity(buffer);
    }

    if(is_empty(buffer)) {
        buffer->head = 0;
        buffer->next_position = 0;
    }

    return DSA_OK;
}

enum DsaReturnCode get_first(RingBuffer* buffer, void* out) {
    if (buffer == NULL) {
        fprintf(stderr, "get_first in RingBuffer, 'buffer' param is null\n");
        return DSA_ERROR_INVALID_PARAM;
    }

    if (out == NULL) {
        fprintf(stderr, "get_first in RingBuffer, 'out' param is null\n");
        return DSA_ERROR_INVALID_PARAM;
    }
    
    if (is_empty(buffer)) {
        fprintf(stderr, "Cannot get from an empty ring buffer\n");
        return DSA_ERROR_IS_EMPTY;
    }

    uint8_t* head_address = ((uint8_t*)buffer->data) + (buffer->head * buffer->item_size);
    memcpy(out, head_address, buffer->item_size);

    return DSA_OK;
}

enum DsaReturnCode get_last(RingBuffer* buffer, void* out) {
    if (buffer == NULL) {
        fprintf(stderr, "get_last in RingBuffer, 'buffer' param is null\n");
        return DSA_ERROR_INVALID_PARAM;
    }

    if (out == NULL) {
        fprintf(stderr, "get_last in RingBuffer, 'out' param is null\n");
        return DSA_ERROR_INVALID_PARAM;
    }
    
    if (is_empty(buffer)) {
        fprintf(stderr, "get_last from an empty ring buffer\n");
        return DSA_ERROR_IS_EMPTY;
    }

    size_t last_index = (buffer->next_position + buffer->capacity - 1) % buffer->capacity;
    uint8_t* tail_address = ((uint8_t*)buffer->data) + (last_index * buffer->item_size);

    memcpy(out, tail_address, buffer->item_size);
    return DSA_OK;
}

void ring_buffer_destroy(RingBuffer* buffer) {
    free(buffer->data);
    buffer->data = NULL;

}

void print_ring_buffer(RingBuffer* buffer, char* (*to_string)(void* item)){
    if (buffer == NULL) {
        printf("RingBuffer is null.\n");
        return;
    }

    if (to_string == NULL) {
        fprintf(stderr, "print_ring_buffer function parameter should not be null.\n");
        return;
    }

    printf("RingBuffer @ %p - length: %zd, data size: %zd, "
        "capacity: %zd, contents:\n",
        (void*)buffer, buffer->length, buffer->item_size, buffer->capacity);

    if (buffer->length <= 0) {
        printf("Empty RingBuffer.\n");
        return;
    }

    printf("[ ");
    
    for(size_t i = 0; i < buffer->length; i++) {
        void* item = ((uint8_t*)buffer->data) + (i * buffer->item_size);
        char* description = to_string(item);
        printf("%s", description);
        free(description);
        printf(", ");
    }

    printf("]\n");
}
