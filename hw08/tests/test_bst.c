#include "../bst.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static int failures = 0;

static void check(bool cond, const char *name) {
    if (cond) {
        printf("PASS %s\n", name);
    } else {
        printf("FAIL %s\n", name);
        failures++;
    }
}

/* The original's seven keys, inserted in the original's order. */
static bst_t *make_seven(void) {
    static const int keys[7] = {27, 14, 35, 10, 19, 31, 42};
    bst_t *t = bst_create();
    for (int i = 0; i < 7; i++) {
        bst_insert(t, keys[i]);
    }
    return t;
}

/*
 * The input that crashed the faithful port: 15.
 * Path: 27 -> 14 -> 19 -> right = NULL, then the original's loop
 * condition dereferences NULL. Visits = 3 (27, 14, 19).
 */
static void test_crash(void) {
    bst_t *empty = bst_create();      /* non-NULL tree, root == NULL */
    size_t visits = 0;
    check(!bst_contains(empty, 1, &visits), "empty tree: not found");
    check(visits == 0, "empty tree: 0 visits");
    bst_destroy(empty);

    /* Single-node tree: both misses leave via a NULL child. */
    bst_t *s = bst_create();
    bst_insert(s, 42);
    size_t v = 0;
    check(!bst_contains(s, 7, &v) && v == 1, "single-node miss left");
    check(!bst_contains(s, 99, &v) && v == 1, "single-node miss right");
    bst_destroy(s);
}

/* The original's seven-key tree: size 7, height 3. */
static void test_seven(void) {
    bst_t *t = make_seven();
    check(bst_size(t) == 7, "seven: size 7");
    check(bst_height(t) == 3, "seven: height 3");

    size_t visits = 0;
    check(bst_contains(t, 31, &visits) && visits == 3,
          "seven: 31 found in 3 visits");

    visits = 0;
    check(!bst_contains(t, 15, &visits) && visits == 3,
          "seven: 15 not found in 3 visits");
    bst_destroy(t);
}

/* Equal key is kept, to the right: size 2, height 2, first hit in 1 visit. */
static void test_duplicate(void) {
    bst_t *t = bst_create();
    bst_insert(t, 5);
    bst_insert(t, 5);
    check(bst_size(t) == 2, "duplicate: size 2");
    check(bst_height(t) == 2, "duplicate: height 2");
    size_t visits = 0;
    check(bst_contains(t, 5, &visits) && visits == 1,
          "duplicate: first 5 in 1 visit");
    bst_destroy(t);
}

/* Increasing order degenerates to a chain: height == number of keys. */
static void test_increasing(void) {
    bst_t *t = bst_create();
    for (int i = 1; i <= 10; i++) {
        bst_insert(t, i);
    }
    check(bst_size(t) == 10, "increasing: size 10");
    check(bst_height(t) == 10, "increasing: height 10");
    size_t visits = 0;
    check(bst_contains(t, 10, &visits) && visits == 10,
          "increasing: last in 10 visits");
    bst_destroy(t);
}

/* NULL handling and destroying a full tree. */
static void test_null_and_destroy(void) {
    check(bst_size(NULL) == 0, "NULL: size 0");
    check(bst_height(NULL) == 0, "NULL: height 0");
    size_t visits = 123;
    check(!bst_contains(NULL, 1, &visits) && visits == 0,
          "NULL: contains false, 0 visits");
    check(bst_insert(NULL, 1) == -1, "NULL: insert -1");
    bst_destroy(NULL); /* must not crash */
    check(true, "NULL: destroy does nothing");

    bst_t *t = make_seven();
    bst_destroy(t);
    check(true, "destroy full tree");
}

int main(void) {
    test_crash();
    test_seven();
    test_duplicate();
    test_increasing();
    test_null_and_destroy();

    if (failures) {
        printf("%d failure(s)\n", failures);
        return 1;
    }
    printf("all tests passed\n");
    return 0;
}