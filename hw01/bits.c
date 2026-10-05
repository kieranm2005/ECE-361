#include "bits.h"

static uint32_t get_lowest(unint32_t x, int width) {
uint32_t mask = (1u << width) - 1;
uint32_t x_masked = x & mask;
return x_masked;
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

static uint32_t input_validation(uint32_t word, int pos, int width) {
    if (width < 1 || width > 32 || pos < 0 || pos > 31 || pos + width > 32) {
        return 0; //Invalid inputs, return 0
    }
    else {
        return 1;
    }
}

uint32_t get_field(uint32_t word, int pos, int width) {
    if (input_validation(word, pos, width) == 0) {
        return 0; //Invalid inputs, return 0
    }
    word = word << (32 - width); //Discard bits to the right
    word = word >> ((32 - width) + pos); //Shift to LSB
    return word;
}

uint32_t set_field(uint32_t word, int pos, int width, uint32_t value) {
    if (input_validation(word, pos, width) == 0) {
        return 0; //Invalid inputs, return 0
    }
}

int32_t sign_extend(uint32_t value, int width) {
    
}