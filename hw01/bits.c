#include "bits.h"

static uint32_t get_lowest(uint32_t x, int width) {
if (width == 32) {
        return x;
    }
uint32_t mask = (1u << width) - 1;
return x & mask;
}

void print_binary(uint32_t x, int width) {
uint32_t x_masked = get_lowest(x, width);
for (int i = width - 1; i >= 0; i--) {
    printf("%u", (x_masked >> i) & 1); //Shift bit at position 'i' to LSB, then mask to get 0 or 1
    if (i % 4 == 0 && i != 0) { //Every 4 bits print a space
        printf(" ");
    }
}
printf("\n"); //Print a newline after
}

static uint32_t input_validation(int pos, int width) {
    if (width < 1 || width > 32 || pos < 0 || pos > 31 || pos + width > 32) {
        return 0; //Invalid inputs, return 0
    }
    else {
        return 1;
    }
}

uint32_t get_field(uint32_t word, int pos, int width) {
    if (input_validation(word, pos, width) == 0) { //Input validation
        return 0; //Invalid inputs, return 0
    }
    word = word << (32 - width); //Discard bits to the right
    word = word >> ((32 - width) + pos); //Shift to LSB
    return word;
}

uint32_t set_field(uint32_t word, int pos, int width, uint32_t value) {
    if (input_validation(word, pos, width) == 0) { //Input validation
        return 0; //Invalid inputs, return 0
    }
    value = get_lowest(value, width); //Mask the value to the lowest 'width' bits
    value = value << pos; //Shift the value to the correct position
    uint32_t mask;
    if (width == 32) {
        mask = 0;
    }
    else {
        mask = ~(((1u << width) - 1) << pos); //Create a mask to clear the bits in the specified field
    }
    word = word & mask; //Clear the bits in the specified field
    word = word ^ value; //Set the new value in the specified field
    return word;
}

int32_t sign_extend(uint32_t value, int width) { //Interprets the lowest width bits of value as a two’s complement number and returns it as an int32_t. sign_extend(0xF8, 8) returns -8.
    value = get_lowest(value, width); //Get the lowest 'width' bits of the value
    if (width == 32) {
        return (int32_t)value; //No sign extension needed for 32 bits
    }
    if ((value >> (width - 1)) & 1) { //Check if the sign bit is set
        value |= ~((1u << width) - 1); //Sign extend by setting all bits to the left to 1
    }
    return (int32_t)value; //Return the sign-extended value as an int32_t
}