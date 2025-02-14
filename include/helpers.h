#ifndef HELPERS_H
#define HELPERS_H

#include <stdbool.h>
#include <stdarg.h>

#define COLOR_BOLD  "\033[1m"
#define COLOR_OFF   "\033[0m"

extern bool dsc_is_debug;

bool is_in_array(int* arr, int size, int value); 
void print_array_with_bold_elements(int* arr, int size, int* bold_indexes,
                                    int bold_indexes_size); 
void swap_elements(int* arr, int left, int right);

int* copy_array(int* array, int size);

int copy_array_to(int* source_array, int source_size,
                   int* destination_array, int destination_size);

void print_out_of_memory();

void print_debug(char* stuff_to_say, ...);

#endif
