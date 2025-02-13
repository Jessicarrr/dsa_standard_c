#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

typedef struct Node {
    void* data;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct BinarySearchTree {
    struct Node* head;
    int (*compare)(void* data_1, void* data_2);

} BinarySearchTree;

#endif
