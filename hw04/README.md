# Events

## Build and test

From this directory, run:

```sh
make
```

This builds `events` with `gcc -std=c11 -Wall -Wextra`. To build and run the
tally tests and the events example, run:

```sh
make test
```

The sample events input is in `data/door.txt`. Remove the generated executable
and object files with:

```sh
make clean
```

## Opaque type, the compiler error, what this type prevents, why it is useful

Line 9 of the former `tests/test_opaque.c` attempted to access `t->n`, which
produced the error `pointer or reference to incomplete type "struct tally" is
not allowed` because `tally_t` is declared as an opaque type in `tally.h`.

An opaque type prevents code outside the implementation from seeing or
accessing the structure's fields directly; this is useful because it protects
the implementation of the tally_t struct from being changed, and allows it to be changed
without requiring callers to be changed, because callers use only the tally operation functions to interact with tally objects.