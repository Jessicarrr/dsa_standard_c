#include <stdio.h>

#include "data_structures/linear/list.h"

#include "test_helpers.h"
#include <CUnit/Basic.h>
#include "test_struct.h"

List* test_generate_list(char* name, size_t size) {
    printf("\n\033[1;35m=== Testing Lists (Dynamic array): %s ===\033[0m\n",
            name);

    List* list = create_list(sizeof(int));

    for(size_t i = 0; i < size; i++) {
        int num = i;
        int* ptr = &num;
        list_insert(list, ptr);
    }

    return list;
}

List* test_generate_struct_list(char* name, size_t size) {
    printf("\n\033[1;35m=== Testing Lists (Dynamic array): %s ===\033[0m\n",
        name);

    List* list = create_list(sizeof(TestStruct));
    
    for(size_t i = 0; i  < size; i++) {
        TestStruct* tester = create_test_struct();
        list_insert(list, tester);
        free(tester);
    }

    return list;
}

void test_create_small_list(void) {
    char name[] = "Generate small list";
    List* list = test_generate_list(name, 5);

    CU_ASSERT_EQUAL(list->length, 5);

    print_list(list);
    list_destroy(list);
}

void test_create_larger_list(void) {
    char name[] = "Generate larger list";
    List* list = test_generate_list(name, 25); 

    CU_ASSERT_EQUAL(list->length, 25);

    print_list(list);
    list_destroy(list);
}

void test_create_list_and_remove(void) {
    char name[] = "Create list and remove";
    List* list = test_generate_list(name, 33);

    // now has 0 to 32.

    list_remove(list, 0);
    // now has 1 to 32

    list_remove(list, 10);
    // now has 1 to 10, 12 to 32

    list_remove(list, 10);
    // now has 1-10, 13 to 32

    list_remove(list, 15);
    // removed 18?
    // now has 1-10, 13-17, 19-32

    void* tenth_pointer = list_get_pointer_to(list, 10);
    int* tenth_int_pointer = (int*) tenth_pointer;
    int tenth_value = *tenth_int_pointer;

    printf("Does %d == 13? address == %p\n", tenth_value, tenth_pointer);
    CU_ASSERT_EQUAL(tenth_value, 13);
    print_list(list);
    list_destroy(list);
}

void test_create_list_and_remove_many(void) {
    char name[] = "Create list and remove many";
    List* list = test_generate_list(name, 26);

    for(int i = 0; i < 15; i++) {
        list_remove(list, 0);
    }

    // array starts 0 to 26
    // remove the first 15 (0 to 14)
    // left: 15 to 26.

    int value = 5;
    void* value_pointer = &value;

    list_get_value_at(list, 0, value_pointer);
    int* int_pointer = (int*) value_pointer;
    int value_should_be_fifteen = *int_pointer;

    printf("Does %d == %d?\n", value_should_be_fifteen, 15);
    CU_ASSERT_EQUAL(value_should_be_fifteen, 15);

    print_list(list);
    list_destroy(list);
}

void test_remove_from_null_list(void) {
    char name[] = "Remove from null list";
    List* list = test_generate_list(name, 0);
    free(list);
    list = NULL;

    list_remove(list, 0);
    list_remove(list, 15);
    print_list(list);

    int result = list_get_value_at(list, 0, NULL);

    // the result should be -1 which is an error result
    CU_ASSERT_EQUAL(result, -1);

    list_destroy(list);
}

void test_remove_from_empty_list(void) {
    char name[] = "Remove from empty list";
    List* list = test_generate_list(name, 0);

    list_remove(list, 15);
    list_remove(list, 0);

    print_list(list);

    int result = list_get_value_at(list, 0, NULL);

    // the result should be -1 as an error since the list is empty
    CU_ASSERT_EQUAL(result, -1);

    list_destroy(list);
}

void test_list_remove_last_element(void) {
    char name[] = "Remove last element";
    List* list = test_generate_list(name, 25);
    //list 0 to 24

    int last_element = list->length - 1;

    list_remove(list, last_element);

    //list 0 to 23

    int* result = (int*)list_get_pointer_to(list, list->length - 1);

    printf("Does %d == 23?\n", *result);
    CU_ASSERT_EQUAL(*result, 23);

    print_list(list);
    list_destroy(list);
}

void test_list_remove_duplicate_values(void) {
    printf("\n\033[1;35m=== Testing Lists (Dynamic array): "
            "Remove duplicate values===\033[0m\n");

    List* list = create_list(sizeof(int));

    for (int i = 0; i < 2; i++) {
        for (int i = 0; i < 64; i++) {
            int num = i;
            int* ptr = &num;
            list_insert(list, ptr);
        }
    }

    //printf("List lengths: ");

    int original_length = list->length;

    for(int i = 0; i < original_length / 2; i++) {
        list_remove(list, 0);
        //printf("%d, ", list->length);
    }

    //printf("\n");
    
    int* first_pointer = (int*)list_get_pointer_to(list, 0);
    CU_ASSERT_EQUAL(*first_pointer, 0);

    print_list(list);
    list_destroy(list);
}

void test_list_remove_negative_index(void) {
    char name[] = "Remove negative indexes";
    List* list = test_generate_list(name, 32);

    list_remove(list, -5);
    print_list(list);

    int* first = (int*)list_get_pointer_to(list, 0);
    CU_ASSERT_EQUAL(*first, 0);

    list_destroy(list);
}

void test_list_add_remove_over_time(void) {
    char name[] = "List add and remove over time";
    List* list = test_generate_list(name, 11);

    // list currently 0 - 10
    list_remove(list, 3);
    // list now 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
    // list now: 0, 1, 2, 4, 5, 6, 7, 8, 9, 10
    
    int number = 5;
    int* ptr = &number;

    list_insert(list, ptr);
    list_insert(list, ptr);
    list_insert(list, ptr);

    //  0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 5, 5, 5

    list_remove(list, 0);
    list_remove(list, 23);
    list_remove(list, 25050);

    // 1, 2, 4, 5, 6, 7, 8, 9, 10, 5, 5, 5

    list_insert(list, ptr);
    list_insert(list, ptr);

    // // 1, 2, 4, 5, 6, 7, 8, 9, 10, 5, 5, 5, 5, 5
    
    list_remove(list, 0);
    list_remove(list, 10);
    list_remove(list, 0);

    // 4, 5, 6, 7, 8, 9, 10, 5, 5, 5, 5

    list_insert(list, ptr);
    list_insert(list, ptr);

    // 4, 5, 6, 7, 8, 9, 10, 5, 5, 5, 5, 5, 5

    int* first = (int*)list_get_pointer_to(list, 0);
    CU_ASSERT_EQUAL(*first, 4);

    print_list(list);
    list_destroy(list);
}

void test_list_place_at_index(void) {
    char name[] = "List add at specific indexes";
    List* list = test_generate_list(name, 5);
    int value_to_insert = 33;
    int* value_ptr = &value_to_insert;
    int index = 3;

    printf("List before changes: ");
    print_list(list);
    list_insert_at(list, value_ptr, index);
    printf("Inserted number 5 into middle of array.");
    print_list(list);
    int resultValue = -1;
    int* result = &resultValue;
    list_get_value_at(list, index, result);
    CU_ASSERT_EQUAL(*result, value_to_insert);
    list_destroy(list);
}

void test_list_of_structs(void) {
    char name[] = "List of structs";
    List* list = test_generate_struct_list(name, 10);

    TestStruct* test = (TestStruct*)malloc(sizeof(TestStruct));

    list_get_value_at(list, 3, test);
    //TestStruct* test = (TestStruct*) ptr;
    printf("List of structs, index value is %d", test->index);

    CU_ASSERT_EQUAL(test->index, 3);
    free(test);
    list_destroy(list);
}
