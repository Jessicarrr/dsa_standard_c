#include <stdio.h>
#include <stdlib.h>
#include "data_structures/linear/list.h"
#include "test_helpers.h"
#include <CUnit/Basic.h>
#include "test_struct.h"
#include "dsc_result.h"

List* test_generate_list(char* name, size_t size) {
    printf("\n\033[1;35m=== Testing Lists (Dynamic array): %s ===\033[0m\n", name);
    DscError *error = NULL;
    List* list = create_list(sizeof(int), &error);
    if (!list) {
        if (error) {
            fprintf(stderr, "Error in create_list: %s\n", error->message);
            free(error);
        }
        CU_FAIL("Failed to create list in test_generate_list");
        return NULL;
    }
    for (size_t i = 0; i < size; i++) {
        int num = i;
        list_insert(list, &num, &error);
        if (error) {
            fprintf(stderr, "Error in list_insert: %s\n", error->message);
            free(error);
            error = NULL;
            CU_FAIL("Failed to insert element in test_generate_list");
        }
    }
    return list;
}

List* test_generate_struct_list(char* name, size_t size) {
    printf("\n\033[1;35m=== Testing Lists (Dynamic array): %s ===\033[0m\n", name);
    DscError *error = NULL;
    List* list = create_list(sizeof(TestStruct), &error);
    if (!list) {
        if (error) {
            fprintf(stderr, "Error in create_list: %s\n", error->message);
            free(error);
        }
        CU_FAIL("Failed to create list in test_generate_struct_list");
        return NULL;
    }
    for (size_t i = 0; i < size; i++) {
        TestStruct* tester = create_test_struct();
        list_insert(list, tester, &error);
        if (error) {
            fprintf(stderr, "Error in list_insert: %s\n", error->message);
            free(error);
            error = NULL;
            CU_FAIL("Failed to insert element in test_generate_struct_list");
        }
        free(tester);
    }
    return list;
}

void test_create_small_list(void) {
    char name[] = "Generate small list";
    List* list = test_generate_list(name, 5);
    CU_ASSERT_PTR_NOT_NULL_FATAL(list);
    CU_ASSERT_EQUAL(list->length, 5);
    print_list(list);
    list_destroy(list);
}

void test_create_larger_list(void) {
    char name[] = "Generate larger list";
    List* list = test_generate_list(name, 25); 
    CU_ASSERT_PTR_NOT_NULL_FATAL(list);
    CU_ASSERT_EQUAL(list->length, 25);
    print_list(list);
    list_destroy(list);
}

void test_create_list_and_remove(void) {
    char name[] = "Create list and remove";
    List* list = test_generate_list(name, 33);
    CU_ASSERT_PTR_NOT_NULL_FATAL(list);
    DscError *error = NULL;
    
    list_remove(list, 0, &error);
    if (error) {
        fprintf(stderr, "Error in list_remove (index 0): %s\n", error->message);
        free(error);
        error = NULL;
        CU_FAIL("Failed in list_remove at index 0");
    }
    
    list_remove(list, 10, &error);
    if (error) {
        fprintf(stderr, "Error in list_remove (index 10): %s\n", error->message);
        free(error);
        error = NULL;
        CU_FAIL("Failed in list_remove at index 10");
    }
    
    list_remove(list, 10, &error);
    if (error) {
        fprintf(stderr, "Error in list_remove (second call index 10): %s\n", error->message);
        free(error);
        error = NULL;
        CU_FAIL("Failed in list_remove at index 10 (second call)");
    }
    
    list_remove(list, 15, &error);
    if (error) {
        fprintf(stderr, "Error in list_remove (index 15): %s\n", error->message);
        free(error);
        error = NULL;
        CU_FAIL("Failed in list_remove at index 15");
    }
    
    void* tenth_pointer = list_get_pointer_to(list, 10, &error);
    if (error) {
        fprintf(stderr, "Error in list_get_pointer_to (index 10): %s\n", error->message);
        free(error);
        error = NULL;
        CU_FAIL("Failed in list_get_pointer_to at index 10");
    }
    
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
    CU_ASSERT_PTR_NOT_NULL_FATAL(list);
    DscError *error = NULL;
    
    for (int i = 0; i < 15; i++) {
        list_remove(list, 0, &error);
        if (error) {
            fprintf(stderr, "Error in list_remove during loop: %s\n", error->message);
            free(error);
            error = NULL;
            CU_FAIL("Failed in list_remove loop in test_create_list_and_remove_many");
        }
    }
    
    int value = 5;
    list_get_value_at(list, 0, &value, &error);
    if (error) {
        fprintf(stderr, "Error in list_get_value_at (index 0): %s\n", error->message);
        free(error);
        error = NULL;
        CU_FAIL("Failed in list_get_value_at in test_create_list_and_remove_many");
    }
    
    int value_should_be_fifteen = value;
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
    DscError *error = NULL;
    
    list_remove(list, 0, &error);
    list_remove(list, 15, &error);
    print_list(list);
    int result = list_get_value_at(list, 0, NULL, &error);
    CU_ASSERT_EQUAL(result, -1);
    list_destroy(list);
}

void test_remove_from_empty_list(void) {
    char name[] = "Remove from empty list";
    List* list = test_generate_list(name, 0);
    CU_ASSERT_PTR_NOT_NULL_FATAL(list);
    DscError *error = NULL;
    
    list_remove(list, 15, &error);
    if (error) {
        fprintf(stderr, "Error in list_remove (index 15): %s\n", error->message);
        free(error);
        error = NULL;
    }
    
    list_remove(list, 0, &error);
    if (error) {
        fprintf(stderr, "Error in list_remove (index 0): %s\n", error->message);
        free(error);
        error = NULL;
    }
    
    print_list(list);
    
    int result = list_get_value_at(list, 0, NULL, &error);
    if (error) {
        fprintf(stderr, "Error in list_get_value_at (index 0): %s\n", error->message);
        free(error);
        error = NULL;
    }
    CU_ASSERT_EQUAL(result, -1);
    list_destroy(list);
}

void test_list_remove_last_element(void) {
    char name[] = "Remove last element";
    List* list = test_generate_list(name, 25);
    CU_ASSERT_PTR_NOT_NULL_FATAL(list);
    DscError *error = NULL;
    
    int last_element = list->length - 1;
    list_remove(list, last_element, &error);
    if (error) {
        fprintf(stderr, "Error in list_remove (last element): %s\n", error->message);
        free(error);
        error = NULL;
        CU_FAIL("Failed in list_remove in test_list_remove_last_element");
    }
    
    int* result = (int*) list_get_pointer_to(list, list->length - 1, &error);
    if (error) {
        fprintf(stderr, "Error in list_get_pointer_to (last element): %s\n", error->message);
        free(error);
        error = NULL;
        CU_FAIL("Failed in list_get_pointer_to in test_list_remove_last_element");
    }
    
    printf("Does %d == 23?\n", *result);
    CU_ASSERT_EQUAL(*result, 23);
    print_list(list);
    list_destroy(list);
}

void test_list_remove_duplicate_values(void) {
    printf("\n\033[1;35m=== Testing Lists (Dynamic array): Remove duplicate values ===\033[0m\n");
    DscError *error = NULL;
    List* list = create_list(sizeof(int), &error);
    if (!list) {
        if (error) {
            fprintf(stderr, "Error in create_list: %s\n", error->message);
            free(error);
            error = NULL;
        }
        CU_FAIL("Failed to create list in test_list_remove_duplicate_values");
        return;
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 64; j++) {
            int num = j;
            list_insert(list, &num, &error);
            if (error) {
                fprintf(stderr, "Error in list_insert: %s\n", error->message);
                free(error);
                error = NULL;
                CU_FAIL("Failed in list_insert in test_list_remove_duplicate_values");
            }
        }
    }
    
    int original_length = list->length;
    for (int i = 0; i < original_length / 2; i++) {
        list_remove(list, 0, &error);
        if (error) {
            fprintf(stderr, "Error in list_remove: %s\n", error->message);
            free(error);
            error = NULL;
            CU_FAIL("Failed in list_remove in test_list_remove_duplicate_values");
        }
    }
    
    int* first_pointer = (int*) list_get_pointer_to(list, 0, &error);
    if (error) {
        fprintf(stderr, "Error in list_get_pointer_to: %s\n", error->message);
        free(error);
        error = NULL;
        CU_FAIL("Failed in list_get_pointer_to in test_list_remove_duplicate_values");
    }
    
    CU_ASSERT_EQUAL(*first_pointer, 0);
    print_list(list);
    list_destroy(list);
}

void test_list_remove_negative_index(void) {
    char name[] = "Remove negative indexes";
    List* list = test_generate_list(name, 32);
    CU_ASSERT_PTR_NOT_NULL_FATAL(list);
    DscError *error = NULL;
    
    /* Negative indices are interpreted as very large unsigned numbers.
       This call simulates an invalid index access. */
    list_remove(list, (size_t)-5, &error);
    if (error) {
        fprintf(stderr, "Error in list_remove (negative index simulated): %s\n", error->message);
        free(error);
        error = NULL;
    }
    
    print_list(list);
    
    int* first = (int*) list_get_pointer_to(list, 0, &error);
    if (error) {
        fprintf(stderr, "Error in list_get_pointer_to (index 0): %s\n", error->message);
        free(error);
        error = NULL;
        CU_FAIL("Failed in list_get_pointer_to in test_list_remove_negative_index");
    }
    
    CU_ASSERT_EQUAL(*first, 0);
    list_destroy(list);
}

void test_list_add_remove_over_time(void) {
    char name[] = "List add and remove over time";
    DscError *error = NULL;
    List* list = test_generate_list(name, 11);
    CU_ASSERT_PTR_NOT_NULL_FATAL(list);

    // Remove element at index 3.
    list_remove(list, 3, &error);
    if (error) { 
        fprintf(stderr, "Error in list_remove(index 3): %s\n", error->message);
        free(error); 
        error = NULL; 
    }

    // Now list has elements 0,1,2,4,5,6,7,8,9,10

    int number = 5;
    // Insert three times.
    list_insert(list, &number, &error);
    if (error) { free(error); error = NULL; }
    list_insert(list, &number, &error);
    if (error) { free(error); error = NULL; }
    list_insert(list, &number, &error);
    if (error) { free(error); error = NULL; }
    // Now list length should be 13.

    // Remove element at index 0.
    list_remove(list, 0, &error);
    if (error) { 
        fprintf(stderr, "Error in list_remove(index 0 first call): %s\n", error->message);
        free(error); 
        error = NULL; 
    }
    // Remove element at index 10.
    list_remove(list, 10, &error);
    if (error) { 
        fprintf(stderr, "Error in list_remove(index 10): %s\n", error->message);
        free(error); 
        error = NULL; 
    }
    // Remove element at index 0.
    list_remove(list, 0, &error);
    if (error) { 
        fprintf(stderr, "Error in list_remove(index 0 second call): %s\n", error->message);
        free(error); 
        error = NULL; 
    }

    // Insert two more elements.
    list_insert(list, &number, &error);
    if (error) { free(error); error = NULL; }
    list_insert(list, &number, &error);
    if (error) { free(error); error = NULL; }

    // Remove element at index 0.
    list_remove(list, 0, &error);
    if (error) { 
        fprintf(stderr, "Error in list_remove(index 0 third call): %s\n", error->message);
        free(error); 
        error = NULL; 
    }
    // Remove element at index 10.
    list_remove(list, 10, &error);
    if (error) { 
        fprintf(stderr, "Error in list_remove(index 10 second call): %s\n", error->message);
        free(error); 
        error = NULL; 
    }
    // Remove element at index 0.
    list_remove(list, 0, &error);
    if (error) { 
        fprintf(stderr, "Error in list_remove(index 0 fourth call): %s\n", error->message);
        free(error); 
        error = NULL; 
    }

    // Insert two more elements.
    list_insert(list, &number, &error);
    if (error) { free(error); error = NULL; }
    list_insert(list, &number, &error);
    if (error) { free(error); error = NULL; }

    // Finally, check the first element.
    int* first = (int*) list_get_pointer_to(list, 0, &error);
    if (error) { 
        fprintf(stderr, "Error in list_get_pointer_to(index 0): %s\n", error->message);
        free(error); 
        error = NULL; 
        CU_FAIL("Failed in list_get_pointer_to in test_list_add_remove_over_time");
    }
    CU_ASSERT_PTR_NOT_NULL(first);
    CU_ASSERT_EQUAL(*first, 5);  // expected value based on prior removals and insertions

    print_list(list);
    list_destroy(list);
}

void test_list_place_at_index(void) {
    char name[] = "List add at specific indexes";
    List* list = test_generate_list(name, 5);
    CU_ASSERT_PTR_NOT_NULL_FATAL(list);
    int value_to_insert = 33;
    int index = 3;
    printf("List before changes: ");
    print_list(list);
    DscError *error = NULL;
    
    list_insert_at(list, &value_to_insert, index, &error);
    if (error) {
        fprintf(stderr, "Error in list_insert_at: %s\n", error->message);
        free(error);
        error = NULL;
        CU_FAIL("Failed in list_insert_at in test_list_place_at_index");
    }
    printf("Inserted number 33 into middle of array.\n");
    print_list(list);
    
    int resultValue = -1;
    list_get_value_at(list, index, &resultValue, &error);
    if (error) {
        fprintf(stderr, "Error in list_get_value_at: %s\n", error->message);
        free(error);
        error = NULL;
        CU_FAIL("Failed in list_get_value_at in test_list_place_at_index");
    }
    CU_ASSERT_EQUAL(resultValue, value_to_insert);
    list_destroy(list);
}

void test_list_of_structs(void) {
    char name[] = "List of structs";
    List* list = test_generate_struct_list(name, 10);
    CU_ASSERT_PTR_NOT_NULL_FATAL(list);
    print_list_custom(list, test_struct_to_string);
    DscError *error = NULL;
    
    TestStruct test;
    list_get_value_at(list, 3, &test, &error);
    if (error) {
        fprintf(stderr, "Error in list_get_value_at (struct index 3): %s\n", error->message);
        free(error);
        error = NULL;
        CU_FAIL("Failed in list_get_value_at in test_list_of_structs");
    }
    printf("List of structs, index value is %d\n", test.index);
    CU_ASSERT_EQUAL(test.index, 3);
    list_destroy(list);
}
