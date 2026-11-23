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

Running treestats on 1-1023 in increasing order with:
`seq 1 1023 | ./treestats`

Returns:
`n 1023, height 1023` every time

Running treestats on 1-1023 in shuffled order with:
`seq 1 1023 | sort -R | ./treestats`

Returns:
`n 1023, height 18` 1st run
`n 1023, height 21` 2nd run
`n 1023, height 26` 3rd run
`n 1023, height 22` 4th run

Insertion order determines the tree's shape, so sorted input always makes a tree of height 1023 while the height of a shuffled input tree varies within a limited range. A search may take up to 1023 visits in the sorted tree, but the maximum visits in the shuffled tree will be much less.

