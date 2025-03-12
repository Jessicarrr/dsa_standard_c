#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"
#include <string.h>

#include "data_structures/trees/binary_search_tree.h"
#include "dsc_result.h"

enum DscReturnCode create_bst_node(
        void* data,
        BstNode* out,
        size_t item_size)
{
    if(out == NULL) {
        fprintf(stderr, "BstNode 'out' param in create_bst_node must not be null "
                "- Binary Search Tree.");
        return DSC_ERROR_INVALID_PARAM;
    }

    out->right = NULL;
    out->left = NULL;
    out->data = malloc(item_size);

    if (out->data == NULL) {
        fprintf(stderr, "An error occurred trying to allocate memory to BstNode->data "
                "- do you have enough memory?");
        return DSC_ERROR_MEM_ALLOC;
    }

    memcpy(out->data, data, item_size);
    return DSC_OK;
}

enum DscReturnCode create_binary_search_tree(
        NodeComparison (*compare)(void* data_1, void* data_2),
        size_t item_size, 
        BinarySearchTree* out)
{
    if(compare == NULL) {
        fprintf(stderr, "int '*compare' param in create_binary_search_tree "
                "must not be null. - Binary Search Tree.");
        return DSC_ERROR_INVALID_PARAM;
    }

    if (out == NULL) {
        fprintf(stderr, "BinarySearchTree 'out' param in create_binary_search_tree "
            "must not be null. - Binary Search Tree.");
        return DSC_ERROR_INVALID_PARAM;
    }

    out->root = NULL;
    out->length = 0;
    out->item_size = item_size;
    out->compare = compare;
    return DSC_OK;
}

static enum DscReturnCode bst_insert_recursive(
    BinarySearchTree* tree,
    BstNode** node_ptr,
    void* data)
{
    if (*node_ptr == NULL) {
        BstNode* new_node = (BstNode*)malloc(sizeof(BstNode));
        if (new_node == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            return DSC_ERROR_MEM_ALLOC;
        }

        enum DscReturnCode result = create_bst_node(data, new_node, tree->item_size);

        if (result != DSC_OK) {
            free(new_node);
            return result;
        }

        *node_ptr = new_node;
        return DSC_OK;
    }

    NodeComparison cmp = tree->compare(data, (*node_ptr)->data);

    // Choose the branch without changing the parent's pointer immediately.
    if (cmp == LOWER || cmp == EQUAL) {
        return bst_insert_recursive(tree, &((*node_ptr)->left), data);
    } else { // HIGHER
        return bst_insert_recursive(tree, &((*node_ptr)->right), data);
    }
}

enum DscReturnCode bst_insert(BinarySearchTree* tree, void* data) {
    if (tree == NULL) {
        fprintf(stderr, "Invalid param 'tree' in bst_insert - Binary Search Tree.\n");
        return DSC_ERROR_INVALID_PARAM;
    }
    if(data == NULL) {
        fprintf(stderr, "Invalid param 'data' in bst_insert - Binary Search Tree.\n");
        return DSC_ERROR_INVALID_PARAM;
    }
    
    enum DscReturnCode ret = bst_insert_recursive(tree, &(tree->root), data);

    if (ret == DSC_OK) {
        tree->length++;
    }
    return ret;
}


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
