## Building

From this directory, run:

```sh
make
```

This compiles `ptr.c` and `tests/test_ptr.c` with GCC using C11 and the
`-Wall -Wextra` warning options, then links the object files into
`tests/test_ptr`.

## Testing

Run the test program with:

```sh
make test
```

The tests cover swapping values, finding minimum and maximum values, locating
values above a threshold, and reversing arrays. The command exits
unsuccessfully if any test fails.

## Cleaning

Remove the generated object files and test executable with:

```sh
make clean
```