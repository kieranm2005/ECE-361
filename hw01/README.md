# What the library does

This library provides bit-field utilities for 32-bit unsigned words and a
decoder for a 16-bit thermostat status word.

`print_binary` prints the lowest `width` bits of a value from most significant
to least significant, inserting a space after each group of four bits. It
always prints a newline. `print_binary_to` provides the same behavior for a
caller-supplied C `FILE` stream.

`get_field` extracts a field from `word`. `set_field` replaces a field in
`word`, masking `value` to the field width first. `sign_extend` interprets the
lowest bits of a value as a two's-complement signed number.

`status_unpack` decodes the heat, cool, fan, fault, mode, reserved, and
setpoint fields defined in `status.h`.

# How to build it

From this directory, run:

```sh
make
```

This compiles `bits.c` and `status.c` into `bits.o` and `status.o` using C11,
`-Wall`, and `-Wextra`.

# How to run the tests

From this directory, run:

```sh
make test
```

The test program checks the bit operations at their boundaries and tests
`status_unpack` with several status words. It prints `PASS` or `FAIL` for each
check, prints a summary, and exits with a nonzero status if any check fails.

Remove all generated files with:

```sh
make clean
```

# Valid ranges of inputs and behavior at boundaries

For `get_field` and `set_field`, valid inputs satisfy:

- `1 <= width <= 32`
- `0 <= pos <= 31`
- `pos + width <= 32`

If any of these conditions is false, both functions return `0`. For
`set_field`, this means the original word is not returned; the result is zero.
At the boundaries, width `1` and width `32` are supported, as is a one-bit
field at position `31`. A value passed to `set_field` that is wider than the
field is truncated to its lowest `width` bits before insertion.

For `sign_extend`, the intended width range is `1` through `32`. Width `1`
sign-extends the value `1` to `-1`; width `32` preserves the full 32-bit
two's-complement value. The most negative value for an 8-bit field, `0x80`,
becomes `-128`.

For `print_binary` and `print_binary_to`, use a width from `1` through `32`.
The value is limited to its lowest `width` bits. Width `1` prints one bit and
width `32` prints all bits, with four-bit spacing.

`status_unpack` reads these fields from the 16-bit status word:

- bits 0-3: heat, cool, fan, and fault flags
- bits 4-6: mode
- bit 7: reserved
- bits 8-15: signed setpoint

Mode values `0` through `4` are reported unchanged. Mode values `5`, `6`, and
`7` are invalid and are all reported as `MODE_INVALID`.