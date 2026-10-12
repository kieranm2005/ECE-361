#include "iom361_r4.h"

#include <stdio.h>

#define DEVICE_NAME "serverroom"
#define NUM_SWITCHES 1   // ACK
#define NUM_LEDS     3   // WARN, ALARM, BACKUP_REQUEST

int main(void)
{
    int rc = 0; // Return code for iom361_initialize
    uint32_t *base = iom361_initialize(NUM_SWITCHES, NUM_LEDS, &rc); // Initialize the device and get the base address of the memory-mapped I/O

    if (base == NULL) { // Check if the device initialization failed
        fprintf(stderr, "%s: iom361_initialize failed: %d\n",
                DEVICE_NAME, rc);
        return 1;
    }

    printf("%s\n", DEVICE_NAME); // Print the device name to indicate successful initialization
    return 0;
}