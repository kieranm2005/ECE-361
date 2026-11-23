# ECE 361 HW8 — Binary Search Tree ADT

## Build and test

    make          # builds treestats
    make test     # builds and runs tests/test_bst.c
    make memcheck # rebuilds with ASan+UBSan and runs the tests
    make clean

## The crash

First line of the memcheck report (ASan):

==11437==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000000 (pc 0x58890f072033 bp 0x7ffe81d8ac00 sp 0x7ffe81d8abd0 T0)

The crash was triggered by `test_crash` which calls a search on an empty tree.

The original never noticed because its root node is never null because the main is hard coded to always insert 7 keys before searching, so search is never called on an empty tree.

Cause: the original's loop condition is `while(current->data != data)`,
which dereferences `current` *before* the body's `if(current == NULL)`
check. Searching for `15` walks `27 → 14 → 19 → right = NULL`; the next
loop test dereferences the NULL node.

Why the original never noticed: its hard-coded `main` searches for `31`
(found) and `15`. On many builds the NULL dereference of the `data`
field happens to read benign memory or the process "gets away with it";
the bug only surfaces under ASan, or on platforms where it faults.
The fix moves the `current == NULL` check into the loop condition.

## Insertion order vs. height

Increasing order (1..1023):

    $ seq 1 1023 | ./treestats
    n 1023, height 1023

Shuffled order:

    $ seq 1 1023 | sort -R | ./treestats
    n 1023, height 22

Insertion order determines the tree's shape. Sorted input degenerates to
a linked list of height n (so a search can cost n visits), while random
input produces a roughly balanced tree of height ~2·log2(n) ≈ 22.