#ifndef QUICK_SORT_H
#define QUICK_SORT_H

void quick_sort(int* arr, int size);
void quick_sort_with_pointers(int* arr, int size, int left, int right); 
int partition(int* arr, int size, int left, int right);

#endif
