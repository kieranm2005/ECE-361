#ifndef BST_H
#define BST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct bst bst_t;          /* opaque */

/* Creates a new, empty tree. Returns NULL if memory runs out. */
bst_t *bst_create(void);

/* Frees every node and the tree. NULL does nothing. */
void bst_destroy(bst_t *t);

/*
 * Adds key as the original does: smaller keys go left, all others right,
 * so an equal key is kept, to the right.
 * Returns 0 on success, or -1 if t is NULL or memory runs out
 * (in which case the tree is unchanged).
 */
int bst_insert(bst_t *t, int key);

/*
 * The original's search, without printing: whether key is in the tree.
 * If visits is not NULL, *visits is the number of nodes compared,
 * found or not. For a NULL t: false, 0 visits.
 */
bool bst_contains(const bst_t *t, int key, size_t *visits);

/* Number of keys. For NULL: 0. */
size_t bst_size(const bst_t *t);

/*
 * Number of nodes on the longest path from the root down
 * (empty: 0, one node: 1). For NULL: 0.
 */
size_t bst_height(const bst_t *t);

#endif