#ifndef HELPERS_H
#define HELPERS_H

#include <stdbool.h>

#define COLOR_BOLD  "\033[1m"
#define COLOR_OFF   "\033[0m"

bool is_in_array(int* arr, int size, int value); 
void print_array_with_bold_elements(int* arr, int size, int* bold_indexes,
                                    int bold_indexes_size); 
void swap_elements(int* arr, int left, int right);

#endif
