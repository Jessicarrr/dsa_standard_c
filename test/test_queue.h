#include <stdio.h>

#include "data_structures/linear/queue.h"

#include "test_helpers.h"
#include <CUnit/Basic.h>

Queue* test_generate_queue(char* name, size_t size) {
    printf("\n\033[1;35m=== Testing Queues: %s ===\033[0m\n",
            name);

    Queue* queue = (Queue*) malloc(sizeof(Queue));
    enum DscReturnCode result = create_queue(sizeof(int), queue);
    CU_ASSERT_EQUAL(result, DSC_OK);

    for(size_t i = 0; i < size; i++) {
        int num = i;
        int* ptr = &num;
        enqueue(queue, ptr);
    }

    return queue;
}

static char* queue_int_item_to_string(void* item) {
    int* as_int = (int*) item;
    char* buffer = (char*)malloc(1024);

    snprintf(buffer, 1024,
        "%d", *as_int);

    return buffer;
}

void test_enqueue_dequeue(void) {
    size_t size = 10;
    char description[] = "test enqueue and dequeue";
    Queue* queue = test_generate_queue(description, size);

    printf("After queue creation:\n");
    print_queue(queue, queue_int_item_to_string);

    int original_length = queue->length;
    void* out = malloc(sizeof(int));

    for (size_t i = 0; i < original_length; i++) {
        dequeue(queue, out);
    }

    free(out);

    printf("Dequeued all. Queue after removal:\n");
    print_queue(queue, queue_int_item_to_string);
    CU_ASSERT_EQUAL(queue->length, 0);
    queue_destroy(queue);
}

// Test peek: after several enqueues, peek should return the first item.
void test_peek(void) {
    Queue queue;
    enum DscReturnCode ret = create_queue(sizeof(int), &queue);
    CU_ASSERT_EQUAL(ret, DSC_OK);

    int item;
    // Enqueue integers 0 to 4.
    for (int i = 0; i < 5; i++) {
        item = i;
        ret = enqueue(&queue, &item);
        CU_ASSERT_EQUAL(ret, DSC_OK);
    }

    int peeked;
    ret = peek(&queue, &peeked);
    CU_ASSERT_EQUAL(ret, DSC_OK);
    CU_ASSERT_EQUAL(peeked, 0);

    // Dequeue one element and peek again.
    int dequeued;
    ret = dequeue(&queue, &dequeued);
    CU_ASSERT_EQUAL(ret, DSC_OK);
    CU_ASSERT_EQUAL(dequeued, 0);

    ret = peek(&queue, &peeked);
    CU_ASSERT_EQUAL(ret, DSC_OK);
    CU_ASSERT_EQUAL(peeked, 1);

    queue_destroy(&queue);
}

// Test dequeue on an empty queue.
void test_dequeue_empty(void) {
    Queue queue;
    enum DscReturnCode ret = create_queue(sizeof(int), &queue);
    CU_ASSERT_EQUAL(ret, DSC_OK);

    int value;
    ret = dequeue(&queue, &value);
    CU_ASSERT_EQUAL(ret, DSC_ERROR_IS_EMPTY);

    queue_destroy(&queue);
}

// Test that enqueueing a NULL pointer returns an invalid parameter error.
void test_enqueue_null(void) {
    Queue queue;
    enum DscReturnCode ret = create_queue(sizeof(int), &queue);
    CU_ASSERT_EQUAL(ret, DSC_OK);

    ret = enqueue(&queue, NULL);
    CU_ASSERT_EQUAL(ret, DSC_ERROR_INVALID_PARAM);

    queue_destroy(&queue);
}

// Test dynamic resizing: enqueue enough items to force an increase in capacity,
// then dequeue all items to force a decrease back to the initial capacity (assumed 8).
void test_dynamic_resize(void) {
    Queue queue;
    enum DscReturnCode ret = create_queue(sizeof(int), &queue);
    CU_ASSERT_EQUAL(ret, DSC_OK);

    int item;
    // Enqueue 20 items to force capacity increase.
    for (int i = 0; i < 20; i++) {
        item = i;
        ret = enqueue(&queue, &item);
        CU_ASSERT_EQUAL(ret, DSC_OK);
    }
    // The capacity should have grown above the initial capacity of 8.
    CU_ASSERT_TRUE(queue.buffer->capacity > 8);

    // Dequeue all items to trigger potential capacity decrease.
    while(queue.length > 0) {
        int dummy;
        ret = dequeue(&queue, &dummy);
        CU_ASSERT_EQUAL(ret, DSC_OK);
    }
    // When all items are removed, the ring buffer should have shrunk back to 8.
    CU_ASSERT_EQUAL(queue.buffer->capacity, 8);

    queue_destroy(&queue);
}

// Test alternating enqueue and dequeue operations: each enqueue is immediately dequeued.
void test_alternating_operations(void) {
    Queue queue;
    enum DscReturnCode ret = create_queue(sizeof(int), &queue);
    CU_ASSERT_EQUAL(ret, DSC_OK);

    int item, dequeued;
    for (int i = 0; i < 10; i++) {
        item = i;
        ret = enqueue(&queue, &item);
        CU_ASSERT_EQUAL(ret, DSC_OK);

        ret = dequeue(&queue, &dequeued);
        CU_ASSERT_EQUAL(ret, DSC_OK);
        CU_ASSERT_EQUAL(dequeued, i);
        CU_ASSERT_EQUAL(queue.length, 0);
    }

    queue_destroy(&queue);
}

// Test creating a queue with an invalid item size (zero) should fail.
void test_create_queue_invalid_item_size(void) {
    Queue queue;
    enum DscReturnCode ret = create_queue(0, &queue);
    CU_ASSERT_EQUAL(ret, DSC_ERROR_INVALID_PARAM);
    // No need to call queue_destroy as creation failed.
}