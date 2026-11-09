#ifndef PANEL_H
#define PANEL_H

#include <stdint.h>

/**
 * panel_pressed() - switches that went from off to on between two readings
 * returns  (now & ~before), one bit per switch
 */
uint32_t panel_pressed(uint32_t now, uint32_t before);

/**
 * panel_released() - switches that went from on to off between two readings
 * returns  (~now & before), one bit per switch
 */
uint32_t panel_released(uint32_t now, uint32_t before);

/**
 * panel_led() - turns LED n on or off with read-modify-write
 *
 * Reads LEDS_REG, changes bit n, writes it back, so every other LED keeps its
 * state. If n is not 0 to 31, or if any register access fails, returns -1 and
 * does not touch the register.
 *
 * returns  the new LEDS_REG value on success, -1 on failure
 */
int panel_led(uint32_t *base, int n, int on);

/**
 * panel_toggle() - toggles LED n with read-modify-write
 *
 * Same error behavior as panel_led().
 *
 * returns the new LEDS_REG value on success, -1 on failure
 */
int panel_toggle(uint32_t *base, int n);

#endif