#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <stdbool.h>
#include <stddef.h>

typedef enum NodeComparison {
    LOWER,
    EQUAL,
    HIGHER
} NodeComparison;

typedef struct Node {
    void* data;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct BinarySearchTree {
    struct Node* root;
    int size;
    size_t data_size;
    int (*compare)(void* data_1, void* data_2);
    int* (*get_preorder)();
    int* (*get_postorder)();
    int* (*get_inorder)();
    void (*insert)(struct BinarySearchTree* tree, void* data);
    bool (*remove)(void* data);
} BinarySearchTree;

int* bst_get_preorder();
int* bst_get_postorder();
int* bst_get_inorder();
void bst_insert(BinarySearchTree* tree, void* data);
bool bst_remove(void* data);

Node* create_bst_node(void* data, size_t data_size);
struct BinarySearchTree* create_binary_search_tree(
        int (*compare)(void* data_1, void* data_2), size_t data_size);
void free_binary_search_tree(BinarySearchTree* tree);

void print_binary_search_tree(BinarySearchTree* tree);
#endif
