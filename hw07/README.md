# ECE 361 HW7 — Dynamic int stack, bracket checker, memory checks

## Build and test

    make            # builds ./balance (no warnings)
    make test       # runs tests/test_istack and regress.sh
    make memcheck   # rebuilds with AddressSanitizer and UndefinedBehaviorSanitizer and runs make test
    make clean

## Part 4: Bugs made on purpose

Bug 1 - Double free in balance.c

First line of report: ==14668==ERROR: AddressSanitizer: heap-use-after-free on address 0x503000000040 at pc 0x62c525c605e0 bp 0x7ffe2c3d1270 sp 0x7ffe2c3d1260

What happened in memory: After the first destruction, s is a dangling pointer. AddressSanitizer reports a use-after-free when the second stack destruction tries to read s->data.

Bug 2 - Pushing onto a stack after destroying it

First line of report: ==16322==ERROR: AddressSanitizer: heap-use-after-free on address 0x503000000048 at pc 0x6151ef8aa69a bp 0x7fff19dc2370 sp 0x7fff19dc2360

What happened in memory: After the stack was destroyed, the stack variable still points to the freed memory. AddressSanitizer reports a heap-use-after-free when istack_push reads fields  in the already freed memory.

Bug 3 - Leaving out istack_destroy in balance.c

First line of report: ==16849==ERROR: LeakSanitizer: detected memory leaks

What happened in memory: The memory allocated by istack_create() and istack_push() is dereferenced after main() exits, but it is not freed. This causes a memory leak, which is reported by LeakSanitizer as a direct leak of the 24 bytes of an istack struct (data is 8 bytes, count is 8 bytes, capacity is 8 bytes).