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

## Insertion order vs. height
