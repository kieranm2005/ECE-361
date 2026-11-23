#include "bst.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    bst_t *t = bst_create(); // Create a new binary search tree.
    if (t == NULL) { // Check if the tree was created successfully.
        return 1;
    }

    size_t n = 0; // Initialize a counter for the number of values read.
    int value; // Variable to store the value read from input.
    int read_ok; // Variable to check the result of scanf.

    while ((read_ok = scanf("%d", &value)) == 1) { // Read integers from standard input until a non-integer is encountered.
        if (bst_insert(t, value) != 0) { // Insert the value into the binary search tree. If insertion fails, print an error message and exit.
            bst_destroy(t); // Free the memory allocated for the tree.
            return 1;
        }
        n++;
    }

    if (read_ok == 0) { // If scanf fails to read an integer.
        printf("not a number after %zu values\n", n); // Print an error message indicating how many values were read before the failure.
        bst_destroy(t);
        return 1;
    }

    if (n == 0) { // If no values were read (the tree is empty).
        printf("empty tree\n");
        bst_destroy(t);
        return 0;
    }

    printf("n %zu, height %zu\n", n, bst_height(t)); // Print the number of values read and the height of the binary search tree.
    bst_destroy(t);
    return 0;
}