#include "bits.h"

void print_binary(uint32_t x, int width) {
uint32_t mask = (1u << width) - 1; //Creates a mask where the lowest 'width' bits are set to 1
uint32_t x_masked = x & mask; //Set all bits above the lowest 'width' bits to 0
for (int i = width - 1; i >= 0; i--) {
    printf("%u", (x_masked >> i) & 1); //Shift bit at position 'i' to LSB, then mask to get 0 or 1
    if (i % 4 == 0 && i != 0) { //Every 4 bits print a space
        printf(" ");
    }
}
printf("\n"); //Print a newline after
}

uint32_t get_field(uint32_t word, int pos, int width) {
    if (width > 32) {
        width = 32; //Cap maximum width to 32
    }
    else if (width <= 0) {
        width = 1; //Cap minimum width to 1
    }
    if (pos >= 32) {
        pos = 31; //Cap maximum position to 31
    }
    else if (pos < 0) {
        pos = 0; //Cap minimum position to 0
    }
    word = word << (32 - width); //Discard bits to the right
    word = word >> ((32 - width) + pos); //Shift to LSB
    return word;
}

uint32_t set_field(uint32_t word, int pos, int width, uint32_t value) {
    
}

int32_t sign_extend(uint32_t value, int width) {
    
}