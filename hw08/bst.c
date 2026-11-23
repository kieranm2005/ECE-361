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
    struct node *current;

    if (t == NULL || t->root == NULL) { // If the tree is NULL or the root of the tree is NULL (indicating that the tree is empty), return false.
        if (visits != NULL) {
            *visits = 0;
        }
        return false;
    }

    current = t->root;
    while (current != NULL) { // While the current node is not NULL, continue traversing the tree to search for the key.
        count++;
        if (current->data == key) { // If the data of the current node matches the key being searched for, it means the key is found in the tree.
            if (visits != NULL) {
                *visits = count;
            }
            return true;
        }
        if (current->data > key) { // If the data of the current node is greater than the key being searched for, move to the left child of the current node to continue searching.
            current = current->leftChild;
        } else {
            current = current->rightChild;
        }
    }

    if (visits != NULL) {
        *visits = count;
    }
    return false;
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

static struct node *min_node(struct node *n) {
    while (n->leftChild != NULL) { // Traverse the left children of the subtree until reaching the leftmost node, which has the minimum value.
        n = n->leftChild;
    }
    return n; // Return the leftmost node, which contains the minimum value in the subtree.
}

int bst_remove(bst_t *t, int key) {
    if (t == NULL) {
        return -1;
    }

    struct node *parent = NULL;
    struct node *current = t->root;

    while (current != NULL && current->data != key) {
        parent = current;
        if (key < current->data) {
            current = current->leftChild;
        } else {
            current = current->rightChild;
        }
    }

    if (current == NULL) {
        return -1;                  /* key not in the tree */
    }

    /* Two children: take right subtree's minimum key, remove that node. */
    if (current->leftChild != NULL && current->rightChild != NULL) {
        struct node *min_parent = current;
        struct node *min = current->rightChild;
        while (min->leftChild != NULL) {
            min_parent = min;
            min = min->leftChild;
        }
        current->data = min->data;  /* move the key up */
        parent = min_parent;
        current = min;              /* now remove min (0 or 1 child) */
    }

    /* Zero or one child: splice current out of the tree. */
    struct node *child = (current->leftChild != NULL)
                       ? current->leftChild
                       : current->rightChild;

    if (parent == NULL) {
        t->root = child;            /* removing the root */
    } else if (parent->leftChild == current) {
        parent->leftChild = child;
    } else {
        parent->rightChild = child;
    }

    free(current);
    t->size--;
    return 0;
}