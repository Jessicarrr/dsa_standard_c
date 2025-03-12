#include "data_structures/linear/queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

bool enqueue(Queue* queue, void* data, DscError** error) {
    if (queue == NULL || queue->buffer == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "enqueue: Queue or its buffer is NULL.");
        }
        return false;
    }
    bool result = ring_buffer_insert(queue->buffer, data, error);
    return result;
}

bool dequeue(Queue* queue, void* out, DscError** error) {
    if (queue == NULL || queue->buffer == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "dequeue: Queue or its buffer is NULL.");
        }
        return false;
    }
    bool result = ring_buffer_pop_first(queue->buffer, out, error);
    return result;
}

bool peek(Queue* queue, void* out, DscError** error) {
    if (queue == NULL || queue->buffer == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "peek: Queue or its buffer is NULL.");
        }
        return false;
    }
    bool result = ring_buffer_peek_first(queue->buffer, out, error);
    return result;
}

Queue* create_queue(size_t item_size, DscError** error) {
    if (item_size == 0) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "create_queue: item_size is zero.");
        }
        return NULL;
    }
    
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (!queue) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_MEM_ALLOC, "create_queue: Failed to allocate memory for Queue.");
        }
        return NULL;
    }
    
    queue->buffer = create_ring_buffer(item_size, error);
    if (queue->buffer == NULL) {
        free(queue);
        return NULL;
    }
    
    return queue;
}

void queue_destroy(Queue* queue) {
    if (queue) {
        if (queue->buffer) {
            ring_buffer_destroy(queue->buffer);
        }
        free(queue);
    }
}

void print_queue(Queue* queue, char* (*to_string)(void* data)) {
    if (queue == NULL || queue->buffer == NULL) {
        printf("Queue or its buffer is NULL.\n");
        return;
    }
    if (to_string == NULL) {
        fprintf(stderr, "print_queue: to_string function pointer is NULL.\n");
        return;
    }
    
    printf("Queue @ %p - length: %zd, item size: %zd, capacity: %zd, contents:\n",
           (void*)queue, queue->buffer->length, queue->buffer->item_size, queue->buffer->capacity);
    
    if (queue->buffer->length == 0) {
        printf("Empty Queue.\n");
        return;
    }
    
    printf("[ ");
    // Iterate in logical order starting from the ring buffer's head.
    for (size_t i = 0; i < queue->buffer->length; i++) {
        size_t index = (queue->buffer->head + i) % queue->buffer->capacity;
        void* item = ((uint8_t*)queue->buffer->data) + (index * queue->buffer->item_size);
        char* description = to_string(item);
        printf("%s", description);
        free(description);
        if (i < queue->buffer->length - 1) {
            printf(", ");
        }
    }
    printf(" ]\n");
}

void* queue_get_pointer_to(Queue* queue, int index) {
    if (queue == NULL || queue->buffer == NULL) {
        return NULL;
    }
    if (index < 0 || (size_t)index >= queue->buffer->length) {
        return NULL;
    }
    size_t real_index = (queue->buffer->head + index) % queue->buffer->capacity;
    return ((uint8_t*)queue->buffer->data) + (real_index * queue->buffer->item_size);
}

bool queue_get_value_at(Queue* queue, int index, void* out, DscError** error) {
    if (queue == NULL || queue->buffer == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "queue_get_value_at: Queue or its buffer is NULL.");
        }
        return false;
    }
    if (index < 0 || (size_t)index >= queue->buffer->length) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "queue_get_value_at: index out of bounds.");
        }
        return false;
    }
    
    void* ptr = queue_get_pointer_to(queue, index);
    if (ptr == NULL) {
        if (error) {
            *error = create_dsc_error(DSC_ERROR_INVALID_PARAM, "queue_get_value_at: Failed to get pointer to element.");
        }
        return false;
    }
    
    memcpy(out, ptr, queue->buffer->item_size);
    return true;
}
