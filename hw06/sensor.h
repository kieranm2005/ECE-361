#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include <time.h>

typedef struct {
    time_t t;           // timestamp, Unix epoch seconds, UTC
    float  temp_c;      // degrees C, rounded to 0.1
    float  rh_pct;      // %RH, rounded to 0.1
} reading_t;

/**
 * sensor_read() - reads TEMP_REG and HUMID_REG and fills in *r
 *
 * Keeps bits 19.0 of each register, converts with the AHT20 formulas
 *   Temp(C) = ST  / 2**20 * 200 - 50
 *   RH(%)   = SRH / 2**20 * 100
 * rounds both to 0.1, and stores them with the timestamp now.
 */
int sensor_read(uint32_t *base, time_t now, reading_t *r);

#endif