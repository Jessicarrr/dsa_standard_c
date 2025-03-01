#ifndef TEST_STRUCT_H
#define TEST_STRUCT_H

#include <stdlib.h>

static int current_index = 0;

typedef struct TestStruct {
    int index;
    char* description;

} TestStruct;

TestStruct* create_test_struct() {
    TestStruct* tester = (TestStruct*) malloc(sizeof(TestStruct));
    tester->index = current_index;
    current_index++;
    char description[] = "A test description.";
    tester->description = description;
    return tester;
}

#endif