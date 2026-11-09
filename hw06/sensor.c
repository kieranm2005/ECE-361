#include <math.h>

#include "sensor.h"
#include "iom361_r4.h"

#define SENSOR_FULL_SCALE   1048576.0   /* 2**20 */
#define SENSOR_MASK         0x000FFFFFu /* bits 19.0 */

/* round a double to 0.1*/
static float round_to_tenth(double v)
{
    return (float)(floor(v * 10.0 + 0.5) / 10.0);
}

int sensor_read(uint32_t *base, time_t now, reading_t *r)
{
    int rc;
    uint32_t raw_t;
    uint32_t raw_h;

    // Read both registers; if either fails, change nothing
    raw_t = iom361_readReg(base, TEMP_REG, &rc);
    if (rc != 0)
        return rc;

    raw_h = iom361_readReg(base, HUMID_REG, &rc);
    if (rc != 0)
        return rc;

    raw_t &= SENSOR_MASK;
    raw_h &= SENSOR_MASK;

    double temp_c = (double)raw_t / SENSOR_FULL_SCALE * 200.0 - 50.0;
    double rh_pct = (double)raw_h / SENSOR_FULL_SCALE * 100.0;

    r->t      = now;
    r->temp_c = round_to_tenth(temp_c);
    r->rh_pct = round_to_tenth(rh_pct);
    return 0;
}