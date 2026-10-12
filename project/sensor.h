/*
 * sensor.h: ECE 361, Fall 2026, project 19 server room cooling
 *
 * Sensor module. Responsible for reading the AHT20 temperature and humidity
 * registers on the iom361 board, converting the raw 20-bit values with the
 * AHT20 formulas, and rounding each reading to 0.1 C and 0.1 %RH so that a
 * scenario that sets 91.0 reads 91.0.
 *
 * Owns the input side of the device: it turns the hardware registers into
 * reading_t values for the rest of the program. It does not decide anything,
 * drive any output, or print. The switch register is read by main.c and
 * handed to fsm.c; sensor.c does not read it.
 */
#ifndef SENSOR_H
#define SENSOR_H

#include <time.h>

/*
 * reading_t: one sample from the AHT20, timestamped.
 *
 *   when  -- the tick's timestamp, as a time_t
 *   temp  -- temperature in degrees C, rounded to 0.1
 *   rh    -- relative humidity in %RH, rounded to 0.1
 */
typedef struct {
    time_t when;
    float  temp;
    float  rh;
} reading_t;

/* Interface to be added in HW 3: sensor_read, and the conversion helpers. */

#endif /* SENSOR_H */