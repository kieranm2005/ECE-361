#include "../bst.h"

#include <stdbool.h>
#include <stdio.h>

static int failures = 0;

static void check(bool cond, const char *name) {
    if (cond) printf("PASS %s\n", name);
    else { printf("FAIL %s\n", name); failures++; }
}

/* Build the original's seven-key tree: 27 14 35 10 19 31 42 */
static bst_t *seven(void) {
    static const int k[7] = {27, 14, 35, 10, 19, 31, 42};
    bst_t *t = bst_create();
    for (int i = 0; i < 7; i++) bst_insert(t, k[i]);
    return t;
}

/* Confirm a key is present (or not) without caring about visit counts. */
static bool has(const bst_t *t, int key) {
    return bst_contains(t, key, NULL);
}

/* Walk the tree via in-order using only the public API is not possible,
 * so we verify structure by membership + size + height after each remove. */

int main(void) {
    /* --- Remove a leaf: 10 --- */
    {
        bst_t *t = seven();
        check(bst_remove(t, 10) == 0, "remove 10 returns 0");
        check(bst_size(t) == 6, "after 10: size 6");
        check(!has(t, 10), "after 10: 10 gone");
        check(has(t, 14) && has(t, 19) && has(t, 27), "after 10: others intact");
        check(bst_height(t) == 3, "after 10: height 3");
        bst_destroy(t);
    }

    /* --- Remove a one-child node: 14 (only right child 19 remains) --- */
    {
        bst_t *t = seven();
        bst_remove(t, 10);              /* make 14 have only right child */
        check(bst_remove(t, 14) == 0, "remove 14 returns 0");
        check(bst_size(t) == 5, "after 14: size 5");
        check(!has(t, 14), "after 14: 14 gone");
        check(has(t, 19), "after 14: 19 still reachable (spliced up)");
        bst_destroy(t);
    }

    /* --- Remove a two-child node: 27 (root) --- */
    {
        bst_t *t = seven();
        check(bst_remove(t, 27) == 0, "remove 27 returns 0");
        check(bst_size(t) == 6, "after 27: size 6");
        check(!has(t, 27), "after 27: 27 gone");
        /* 31 is the min of 27's right subtree; it moves to the root. */
        check(has(t, 31), "after 27: 31 present");
        check(has(t, 10) && has(t, 42), "after 27: leaves intact");
        check(bst_height(t) == 3, "after 27: height 3");
        bst_destroy(t);
    }

    /* --- Duplicates: only one copy is removed --- */
    {
        bst_t *t = bst_create();
        bst_insert(t, 5);
        bst_insert(t, 5);
        check(bst_size(t) == 2, "dup: size 2");
        check(bst_remove(t, 5) == 0, "dup: remove one");
        check(bst_size(t) == 1, "dup: size 1");
        check(has(t, 5), "dup: one 5 remains");
        check(bst_remove(t, 5) == 0, "dup: remove last");
        check(bst_size(t) == 0, "dup: size 0");
        check(bst_height(t) == 0, "dup: height 0");
        bst_destroy(t);
    }

    /* --- Missing key / NULL: -1, tree unchanged --- */
    {
        bst_t *t = seven();
        check(bst_remove(t, 99) == -1, "missing 99 returns -1");
        check(bst_size(t) == 7, "missing: size unchanged");
        check(bst_remove(NULL, 1) == -1, "NULL tree returns -1");
        bst_destroy(t);
    }

    /* --- Remove all keys one at a time until empty --- */
    {
        bst_t *t = seven();
        static const int order[7] = {27, 14, 35, 10, 19, 31, 42};
        for (int i = 0; i < 7; i++) {
            check(bst_remove(t, order[i]) == 0, "drain: remove succeeds");
        }
        check(bst_size(t) == 0, "drain: size 0");
        check(bst_height(t) == 0, "drain: height 0");
        bst_destroy(t);
    }

    if (failures) {
        printf("%d failure(s)\n", failures);
        return 1;
    }
    printf("all remove tests passed\n");
    return 0;
}