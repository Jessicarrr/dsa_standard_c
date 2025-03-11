#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <stdbool.h>
#include <stddef.h>
#include "dsc_return_codes.h"

typedef enum NodeComparison {
    LOWER,
    EQUAL,
    HIGHER
} NodeComparison;

typedef struct BstNode {
    void* data;
    struct BstNode* left;
    struct BstNode* right;
} BstNode;

typedef struct BinarySearchTree {
    struct BstNode* root;
    size_t length;
    size_t item_size;
    NodeComparison (*compare)(void* data_1, void* data_2);
} BinarySearchTree;

enum DscReturnCode create_bst_node(
    void* data,
    BstNode* out,
    size_t item_size);

enum DscReturnCode create_binary_search_tree(
    NodeComparison (*compare)(void* data_1, void* data_2),
    size_t item_size, 
    BinarySearchTree* out);

enum DscReturnCode bst_insert(BinarySearchTree* tree, void* data);
enum DscReturnCode bst_remove(void* data);
bool bst_has_value(BinarySearchTree* tree);

enum DscReturnCode bst_get_preorder(BinarySearchTree* tree, void* out);
enum DscReturnCode bst_get_postorder(BinarySearchTree* tree, void* out);
enum DscReturnCode bst_get_inorder(BinarySearchTree* tree, void* out);

enum DscReturnCode bst_destroy(BinarySearchTree* tree);
enum DscReturnCode bst_node_destroy(BstNode* node);

void print_bst(
    BinarySearchTree* tree,
    char* (*to_string)(void* data));

#endif
