#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "data_structures/linear/queue.h"
#include "test_helpers.h"

/**
 * Helper function that creates a queue, enqueues `size` integers,
 * and returns the pointer to the queue.
 */
Queue* test_generate_queue(char* name, size_t size) {
    printf("\n\033[1;35m=== Testing Queues: %s ===\033[0m\n", name);
    
    DscError* error = NULL;
    Queue* queue = create_queue(sizeof(int), &error);
    CU_ASSERT_PTR_NOT_NULL(queue);
    CU_ASSERT_PTR_NULL(error);
    
    for (size_t i = 0; i < size; i++) {
        int num = (int)i;
        error = NULL;
        CU_ASSERT_TRUE(enqueue(queue, &num, &error));
        CU_ASSERT_PTR_NULL(error);
    }
    
    return queue;
}

static char* queue_int_item_to_string(void* item) {
    int* as_int = (int*) item;
    char* buffer = (char*)malloc(1024);
    snprintf(buffer, 1024, "%d", *as_int);
    return buffer;
}

void test_enqueue_dequeue(void) {
    size_t size = 10;
    char description[] = "test enqueue and dequeue";
    Queue* queue = test_generate_queue(description, size);

    printf("After queue creation:\n");
    print_queue(queue, queue_int_item_to_string);

    int original_length = (int)queue->buffer->length;
    int* out = (int*)malloc(sizeof(int));
    DscError* error = NULL;

    for (int i = 0; i < original_length; i++) {
        error = NULL;
        CU_ASSERT_TRUE(dequeue(queue, out, &error));
        CU_ASSERT_PTR_NULL(error);
    }

    free(out);
    printf("Dequeued all. Queue after removal:\n");
    print_queue(queue, queue_int_item_to_string);
    CU_ASSERT_EQUAL(queue->buffer->length, 0);
    queue_destroy(queue);
}

void test_peek(void) {
    DscError* error = NULL;
    Queue* queue = create_queue(sizeof(int), &error);
    CU_ASSERT_PTR_NOT_NULL(queue);
    CU_ASSERT_PTR_NULL(error);

    int item;
    // Enqueue integers 0 to 4.
    for (int i = 0; i < 5; i++) {
        item = i;
        error = NULL;
        CU_ASSERT_TRUE(enqueue(queue, &item, &error));
        CU_ASSERT_PTR_NULL(error);
    }

    int peeked;
    error = NULL;
    CU_ASSERT_TRUE(peek(queue, &peeked, &error));
    CU_ASSERT_PTR_NULL(error);
    CU_ASSERT_EQUAL(peeked, 0);

    int dequeued;
    error = NULL;
    CU_ASSERT_TRUE(dequeue(queue, &dequeued, &error));
    CU_ASSERT_PTR_NULL(error);
    CU_ASSERT_EQUAL(dequeued, 0);

    error = NULL;
    CU_ASSERT_TRUE(peek(queue, &peeked, &error));
    CU_ASSERT_PTR_NULL(error);
    CU_ASSERT_EQUAL(peeked, 1);

    queue_destroy(queue);
}

void test_dequeue_empty(void) {
    DscError* error = NULL;
    Queue* queue = create_queue(sizeof(int), &error);
    CU_ASSERT_PTR_NOT_NULL(queue);
    CU_ASSERT_PTR_NULL(error);

    int value;
    error = NULL;
    CU_ASSERT_FALSE(dequeue(queue, &value, &error));
    CU_ASSERT_PTR_NOT_NULL(error);
    free(error);

    queue_destroy(queue);
}

void test_enqueue_null(void) {
    DscError* error = NULL;
    Queue* queue = create_queue(sizeof(int), &error);
    CU_ASSERT_PTR_NOT_NULL(queue);
    CU_ASSERT_PTR_NULL(error);

    error = NULL;
    CU_ASSERT_FALSE(enqueue(queue, NULL, &error));
    CU_ASSERT_PTR_NOT_NULL(error);
    free(error);

    queue_destroy(queue);
}

void test_dynamic_resize(void) {
    DscError* error = NULL;
    Queue* queue = create_queue(sizeof(int), &error);
    CU_ASSERT_PTR_NOT_NULL(queue);
    CU_ASSERT_PTR_NULL(error);

    int item;
    // Enqueue 20 items to force capacity increase.
    for (int i = 0; i < 20; i++) {
        item = i;
        error = NULL;
        CU_ASSERT_TRUE(enqueue(queue, &item, &error));
        CU_ASSERT_PTR_NULL(error);
    }
    // The capacity should have grown above the initial capacity (8).
    CU_ASSERT_TRUE(queue->buffer->capacity > 8);

    // Dequeue all items to trigger potential capacity decrease.
    while(queue->buffer->length > 0) {
        int dummy;
        error = NULL;
        CU_ASSERT_TRUE(dequeue(queue, &dummy, &error));
        CU_ASSERT_PTR_NULL(error);
    }
    // When all items are removed, the ring buffer should have shrunk back to 8.
    CU_ASSERT_EQUAL(queue->buffer->capacity, 8);

    queue_destroy(queue);
}

void test_alternating_operations(void) {
    DscError* error = NULL;
    Queue* queue = create_queue(sizeof(int), &error);
    CU_ASSERT_PTR_NOT_NULL(queue);
    CU_ASSERT_PTR_NULL(error);

    int item, dequeued;
    for (int i = 0; i < 10; i++) {
        item = i;
        error = NULL;
        CU_ASSERT_TRUE(enqueue(queue, &item, &error));
        CU_ASSERT_PTR_NULL(error);

        error = NULL;
        CU_ASSERT_TRUE(dequeue(queue, &dequeued, &error));
        CU_ASSERT_PTR_NULL(error);
        CU_ASSERT_EQUAL(dequeued, i);
        CU_ASSERT_EQUAL(queue->buffer->length, 0);
    }

    queue_destroy(queue);
}

void test_create_queue_invalid_item_size(void) {
    DscError* error = NULL;
    Queue* queue = create_queue(0, &error);
    CU_ASSERT_PTR_NULL(queue);
    CU_ASSERT_PTR_NOT_NULL(error);
    free(error);
}
