#include <stdio.h>
#include "sorting/selection_sort.h"
#include "searching/binary_search.h"

int main() {
    printf("Hello, World!\n");
    int array[] = { 5, 10, 15, 22, 1, 566, 2414, 1, 2, 6, 8 };
    int array_size = sizeof(array) / sizeof(array[0]);

    //selection_sort(array, array_size);
    binary_search(array, array_size, 22);

    return 0;
}