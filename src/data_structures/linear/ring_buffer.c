#include "data_structures/linear/ring_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define CAPACITY_MULTIPLY 2
#define CAPACITY_START 8
#define BUFFER_GROW_THRESHOLD 1

// Helper functions that now also take an error parameter.
static bool increase_ring_capacity(RingBuffer* buffer, DscError** error) {
    void* new_data = malloc(buffer->item_size * (buffer->capacity * CAPACITY_MULTIPLY));
    if (new_data == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_MEM_ALLOC, "Could not allocate memory for ring buffer expansion.");
        }
        return false;
    }

    // Copy the data from the old buffer into the new one in order.
    for (size_t i = 0; i < buffer->length; i++) {
        size_t index = buffer->head + i;
        if (index >= buffer->capacity) {
            index -= buffer->capacity;
        }
        uint8_t* fetch_address = ((uint8_t*)buffer->data) + (index * buffer->item_size);
        uint8_t* place_address = ((uint8_t*)new_data) + (i * buffer->item_size);
        memcpy(place_address, fetch_address, buffer->item_size);
    }

    free(buffer->data);
    buffer->data = new_data;
    buffer->capacity *= CAPACITY_MULTIPLY;
    buffer->head = 0;
    buffer->next_position = buffer->length;
    return true;
}

static bool decrease_ring_capacity(RingBuffer* buffer, DscError** error) {
    if (buffer->capacity <= CAPACITY_START) {
        // Not an error – capacity is already minimal.
        return true;
    }

    size_t new_capacity = buffer->capacity / CAPACITY_MULTIPLY;
    if (new_capacity == 0 || buffer->length > new_capacity) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_RESIZE_FAIL,
                "Cannot decrease ring buffer capacity without risking data loss.");
        }
        return false;
    }
    
    void* new_data = malloc(buffer->item_size * new_capacity);
    if (new_data == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_MEM_ALLOC, "Could not allocate memory for ring buffer contraction.");
        }
        return false;
    }

    size_t index = buffer->head;
    for (size_t i = 0; i < buffer->length; i++) {
        uint8_t* target = ((uint8_t*)new_data) + (i * buffer->item_size);
        uint8_t* read = ((uint8_t*)buffer->data) + (index * buffer->item_size);
        memcpy(target, read, buffer->item_size);
        index = (index + 1) % buffer->capacity;
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
    return buffer->length == 0;
}

static size_t get_next_position(RingBuffer* buffer) {
    return (buffer->next_position + 1) % buffer->capacity;
}

static bool should_increase_capacity(RingBuffer* buffer) {
    return get_num_free_positions(buffer) <= BUFFER_GROW_THRESHOLD;
}

static bool should_decrease_capacity(RingBuffer* buffer) {
    if (buffer->capacity <= CAPACITY_START) {
        return false;
    }
    return (buffer->length <= (buffer->capacity / 6));
}

//
// PUBLIC FUNCTIONS
//

RingBuffer* create_ring_buffer(size_t item_size, DscError** error) {
    if (item_size == 0) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "Cannot create ring buffer with item_size of zero.");
        }
        return NULL;
    }

    RingBuffer* buffer = (RingBuffer*)malloc(sizeof(RingBuffer));
    if (buffer == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_MEM_ALLOC, "Could not create ring buffer, out of memory.");
        }
        return NULL;
    }

    buffer->item_size = item_size;
    buffer->head = 0;
    buffer->next_position = 0;
    buffer->capacity = CAPACITY_START;
    buffer->length = 0;
    buffer->data = malloc(CAPACITY_START * item_size);

    if (buffer->data == NULL) {
        free(buffer);
        if (error) {
            *error = create_dsc_error(DSC_ERROR_MEM_ALLOC, "Could not allocate memory for ring buffer data block.");
        }
        return NULL;
    }

    return buffer;
}

bool ring_buffer_insert(RingBuffer* buffer, void* data, DscError** error) {
    if (buffer == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "ring_buffer_insert: 'buffer' parameter is NULL.");
        }
        return false;
    }

    if (data == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "ring_buffer_insert: 'data' parameter is NULL.");
        }
        return false;
    }

    if (should_increase_capacity(buffer)) {
        if (!increase_ring_capacity(buffer, error)) {
            return false;
        }
    }

    uint8_t* target_address = ((uint8_t*)buffer->data) + (buffer->next_position * buffer->item_size);
    memcpy(target_address, data, buffer->item_size);
    buffer->length += 1;
    buffer->next_position = get_next_position(buffer);
    return true;
}

bool ring_buffer_pop_first(RingBuffer* buffer, void* out, DscError** error) {
    if (buffer == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "ring_buffer_pop_first: 'buffer' parameter is NULL.");
        }
        return false;
    }

    if (out == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "ring_buffer_pop_first: 'out' parameter is NULL.");
        }
        return false;
    }

    if (is_empty(buffer)) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_IS_EMPTY, "ring_buffer_pop_first: Cannot pop from an empty ring buffer.");
        }
        return false;
    }

    uint8_t* head_address = ((uint8_t*)buffer->data) + (buffer->head * buffer->item_size);
    memcpy(out, head_address, buffer->item_size);

    buffer->head = (buffer->head + 1) % buffer->capacity;
    buffer->length -= 1;

    if (should_decrease_capacity(buffer)) {
        // If contraction fails due to memory issues, propagate the error.
        if (!decrease_ring_capacity(buffer, error)) {
            return false;
        }
    }

    if (is_empty(buffer)) {
        buffer->head = 0;
        buffer->next_position = 0;
    }

    return true;
}

bool ring_buffer_pop_last(RingBuffer* buffer, void* out, DscError** error) {
    if (buffer == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "ring_buffer_pop_last: 'buffer' parameter is NULL.");
        }
        return false;
    }

    if (out == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "ring_buffer_pop_last: 'out' parameter is NULL.");
        }
        return false;
    }
    
    if (is_empty(buffer)) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_IS_EMPTY, "ring_buffer_pop_last: Cannot pop from an empty ring buffer.");
        }
        return false;
    }

    size_t last_index = (buffer->next_position + buffer->capacity - 1) % buffer->capacity;
    uint8_t* tail_address = ((uint8_t*)buffer->data) + (last_index * buffer->item_size);
    memcpy(out, tail_address, buffer->item_size);

    buffer->length -= 1;
    buffer->next_position = last_index;
    
    if (should_decrease_capacity(buffer)) {
        if (!decrease_ring_capacity(buffer, error)) {
            return false;
        }
    }

    if (is_empty(buffer)) {
        buffer->head = 0;
        buffer->next_position = 0;
    }

    return true;
}

bool ring_buffer_peek_first(RingBuffer* buffer, void* out, DscError** error) {
    if (buffer == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "ring_buffer_peek_first: 'buffer' parameter is NULL.");
        }
        return false;
    }

    if (out == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "ring_buffer_peek_first: 'out' parameter is NULL.");
        }
        return false;
    }
    
    if (is_empty(buffer)) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_IS_EMPTY, "ring_buffer_peek_first: Cannot peek into an empty ring buffer.");
        }
        return false;
    }

    uint8_t* head_address = ((uint8_t*)buffer->data) + (buffer->head * buffer->item_size);
    memcpy(out, head_address, buffer->item_size);
    return true;
}

bool ring_buffer_peek_last(RingBuffer* buffer, void* out, DscError** error) {
    if (buffer == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "ring_buffer_peek_last: 'buffer' parameter is NULL.");
        }
        return false;
    }

    if (out == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "ring_buffer_peek_last: 'out' parameter is NULL.");
        }
        return false;
    }
    
    if (is_empty(buffer)) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_IS_EMPTY, "ring_buffer_peek_last: Cannot peek into an empty ring buffer.");
        }
        return false;
    }

    size_t last_index = (buffer->next_position + buffer->capacity - 1) % buffer->capacity;
    uint8_t* tail_address = ((uint8_t*)buffer->data) + (last_index * buffer->item_size);
    memcpy(out, tail_address, buffer->item_size);
    return true;
}

void print_ring_buffer(RingBuffer* buffer, char* (*to_string)(void* item)) {
    if (buffer == NULL) {
        printf("RingBuffer is NULL.\n");
        return;
    }

    if (to_string == NULL) {
        fprintf(stderr, "print_ring_buffer: 'to_string' function parameter is NULL.\n");
        return;
    }

    printf("RingBuffer @ %p - length: %zd, item size: %zd, capacity: %zd\nContents: [ ", 
           (void*)buffer, buffer->length, buffer->item_size, buffer->capacity);

    if (buffer->length == 0) {
        printf("Empty RingBuffer");
    } else {
        // Note: this print loop iterates linearly over the data block,
        // which might not reflect the ring order if head > 0.
        for (size_t i = 0; i < buffer->length; i++) {
            void* item = ((uint8_t*)buffer->data) + (i * buffer->item_size);
            char* description = to_string(item);
            printf("%s", description);
            free(description);
            if (i < buffer->length - 1) {
                printf(", ");
            }
        }
    }
    printf(" ]\n");
}

void ring_buffer_destroy(RingBuffer* buffer) {
    if (buffer) {
        free(buffer->data);
        free(buffer);
    }
}
