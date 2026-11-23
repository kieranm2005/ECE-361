#include "bst.h"

#include <stdlib.h>

struct node {
    int data;
    struct node *leftChild;
    struct node *rightChild;
};

struct bst {
    struct node *root;
    size_t size;
};

bst_t *bst_create(void) {
    bst_t *t = malloc(sizeof *t); // Allocate memory for the tree.
    if (t == NULL) { // If memory allocation fails, return NULL.
        return NULL;
    }
    t->root = NULL; // Initialize the root of the tree to NULL, indicating that the tree is empty.
    t->size = 0; // Initialize the size of the tree to 0, indicating that there are no nodes in the tree.
    return t;
}

// Helper for bst_destroy: frees a subtree.
static void free_subtree(struct node *n) {
    if (n == NULL) {
        return;
    }
    free_subtree(n->leftChild); // Recursively free the left subtree.
    free_subtree(n->rightChild); // Recursively free the right subtree.
    free(n);
}

/* From bst_destroy: frees every node and the tree. */
void bst_destroy(bst_t *t) {
    if (t == NULL) {
        return;
    }
    free_subtree(t->root); // Free all nodes in the tree starting from the root.
    free(t); // Free the memory allocated for the tree structure itself.
}

/*
 * From insert(): adds key. Smaller keys go left, all others right,
 * so an equal key is kept, to the right.
 */
int bst_insert(bst_t *t, int key) {
    if (t == NULL) { //
        return -1;
    }

    struct node *tempNode = malloc(sizeof *tempNode); // Allocate memory for a new node to be inserted into the tree.
    if (tempNode == NULL) {
        return -1;
    }
    tempNode->data = key; // Set the data of the new node to the key being inserted.
    tempNode->leftChild = NULL; // Initialize the left child of the new node to NULL, indicating that it has no left child yet.
    tempNode->rightChild = NULL; // Initialize the right child of the new node to NULL, indicating that it has no right child yet.

    if (t->root == NULL) {
        t->root = tempNode; // If the tree is empty (root is NULL), set the root of the tree to the new node, making it the first node in the tree.
        t->size++; // Increment the size of the tree to reflect the addition of the new node.
        return 0;
    }

    struct node *current = t->root; // Start traversing the tree from the root to find the appropriate position for the new node.
    for (;;) { // Loop until the appropriate position is found.
        if (key < current->data) { // If the key being inserted is less than the data of the current node, move to the left child.
            if (current->leftChild == NULL) { // If the left child of the current node is NULL, it means we have found the appropriate position for the new node.
                current->leftChild = tempNode; //
                t->size++;
                return 0;
            }
            current = current->leftChild; // Move to the left child of the current node to continue searching for the appropriate position.
        } else {
            if (current->rightChild == NULL) { // If the right child of the current node is NULL, it means we have found the appropriate position for the new node.
                current->rightChild = tempNode;
                t->size++;
                return 0;
            }
            current = current->rightChild; // Move to the right child of the current node to continue searching for the appropriate position.
        }
    }
}

bool bst_contains(const bst_t *t, int key, size_t *visits) {
    size_t count = 0;
    if (t == NULL) { if (visits) *visits = 0; return false; }

    struct node *current = t->root;
    while (current->data != key) {
        count++;
        if (current->data > key)
            current = current->leftChild;
        else
            current = current->rightChild;
        if (current == NULL) { if (visits) *visits = count; return false; }
    }
    count++;
    if (visits) *visits = count;
    return true;
}

size_t bst_size(const bst_t *t) {
    if (t == NULL) {
        return 0;
    }
    return t->size;
}

/* Helper for bst_height: height of a subtree. */
static size_t subtree_height(const struct node *n) {
    if (n == NULL) { // If the node is NULL, the height is 0.
        return 0;
    }
    size_t l = subtree_height(n->leftChild); // Recursively calculate the height of the left subtree.
    size_t r = subtree_height(n->rightChild); // Recursively calculate the height of the right subtree.
    return 1 + (l > r ? l : r); // Return the height of the current node, which is 1 plus the maximum height of its subtrees.
}

/* From bst_height: nodes on the longest path from root down. */
size_t bst_height(const bst_t *t) {
    if (t == NULL) { // If the tree is NULL, the height is 0.
        return 0;
    }
    return subtree_height(t->root);
}