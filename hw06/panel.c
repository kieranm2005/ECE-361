#include "panel.h"
#include "iom361_r4.h"

uint32_t panel_pressed(uint32_t now, uint32_t before)
{
    return now & ~before;
}

uint32_t panel_released(uint32_t now, uint32_t before)
{
    return ~now & before;
}

int panel_led(uint32_t *base, int n, int on)
{
    if (n < 0 || n > 31) // check for valid LED index
        return -1;

    int rc;
    uint32_t leds = iom361_readReg(base, LEDS_REG, &rc);
    if (rc != 0)
        return -1;

    uint32_t mask = (uint32_t)1u << n;
    if (on)
        leds |= mask; // turn on the LED
    else
        leds &= ~mask; // turn off the LED

    leds = iom361_writeReg(base, LEDS_REG, leds, &rc);
    if (rc != 0)
        return -1;
    return (int)leds;
}

int panel_toggle(uint32_t *base, int n)
{
    if (n < 0 || n > 31) // check for valid LED index
        return -1;

    int rc;
    uint32_t leds = iom361_readReg(base, LEDS_REG, &rc);
    if (rc != 0)
        return -1;

    leds ^= (uint32_t)1u << n; // toggle the LED

    leds = iom361_writeReg(base, LEDS_REG, leds, &rc);
    if (rc != 0)
        return -1;
    return (int)leds;
}