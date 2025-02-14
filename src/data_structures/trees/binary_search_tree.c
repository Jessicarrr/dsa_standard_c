#include "data_structures/trees/binary_search_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"
#include <string.h>

int* bst_get_preorder() { return NULL; }
int* bst_get_postorder() { return NULL; }
int* bst_get_inorder() { return NULL; }

static Node* bst_insert_recursive(BinarySearchTree* tree, Node* current,
                                  void* data) {

    if (current == NULL) {
        
        Node* new_node = create_bst_node(data, tree->data_size);
        tree->root = new_node;
        return tree->root;
    }

    printf("Compare: data = %p, current->data = %p\n", data, current->data);
    if (tree->compare(data, current->data) == HIGHER) {
        if (current->right != NULL) {
            return bst_insert_recursive(tree, current->right, data);
        }
        current->right = create_bst_node(data, tree->data_size);
        return current->right;
    }
    else {
        if (current->left != NULL) {
            return bst_insert_recursive(tree, current->left, data);
        }
        current->left = create_bst_node(data, tree->data_size);
        return current->left;
    }
}

void bst_insert(BinarySearchTree* tree, void* data) {
    bst_insert_recursive(tree, tree->root, data); 
}

bool bst_remove(void* data) { return false; }

Node* create_bst_node(void* data, size_t data_size){ 
    Node* new_node = malloc(sizeof(Node));

    if (new_node == NULL) {
        print_out_of_memory();
        return NULL;
    }
    
    new_node->data = malloc(data_size);
    memcpy(new_node->data, data, data_size);
    print_debug("Create node with data %p\n", new_node->data);
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

struct BinarySearchTree* create_binary_search_tree(int (*compare)(void* data_1,
                                                            void* data_2),
                                                   size_t data_size) {
    struct BinarySearchTree* tree = malloc(sizeof(BinarySearchTree));

    if(tree == NULL) {
        print_out_of_memory();
        return NULL;
    }

    tree->size = 0;
    tree->data_size = data_size;
    tree->get_postorder = bst_get_postorder;
    tree->get_preorder = bst_get_preorder;
    tree->get_inorder = bst_get_inorder;
    tree->insert = bst_insert;
    tree->remove = bst_remove;
    tree->compare = compare;

    return tree;
}
void free_binary_search_tree(BinarySearchTree* tree);

static void print_recursive(Node* node, int level) {
    if (node == NULL) {
        //printf("(null node)\n");
        return;
    }

    printf("level: %d, data: %d\n", level, *((int*)node->data));
    print_recursive(node->left, level + 1);
    print_recursive(node->right, level + 1);

    
}

void print_binary_search_tree(BinarySearchTree* tree) {

    print_recursive(tree->root, 0);

}
