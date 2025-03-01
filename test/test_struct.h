#ifndef TEST_STRUCT_H
#define TEST_STRUCT_H

#include <stdlib.h>
#include <stdio.h>

static int current_index = 0;

typedef struct TestStruct {
    int index;
    char const* description;

} TestStruct;

TestStruct* create_test_struct() {
    TestStruct* tester = (TestStruct*) malloc(sizeof(TestStruct));
    tester->index = current_index;
    current_index++;
    char const* description = "A small description.";
    tester->description = description;
    return tester;
}

char* test_struct_to_string(void* item) {
    TestStruct* test = (TestStruct*) item;
    char* buffer = (char*)malloc(1024);

    snprintf(buffer, 1024,
        "TestStruct: idx %d description: %s", test->index, test->description);

    return buffer;
}

#endif