#ifndef STATUS_H
#define STATUS_H

#include <stdint.h>
#include <stdbool.h>

// Thermostat status word bit positions and widths
#define HEAT_POS 0
#define HEAT_WIDTH 1
#define COOL_POS 1
#define COOL_WIDTH 1
#define FAN_POS 2
#define FAN_WIDTH 1
#define FAULT_POS 3
#define FAULT_WIDTH 1
#define MODE_POS 4
#define MODE_WIDTH 3
#define RESERVED_POS 7
#define RESERVED_WIDTH 1
#define SETPOINT_POS 8
#define SETPOINT_WIDTH 8

// Mode values
#define MODE_OFF 0
#define MODE_HEAT 1
#define MODE_COOL 2
#define MODE_AUTO 3
#define MODE_FAN_ONLY 4
#define MODE_INVALID 5 // Modes 5, 6, 7

// status_t
typedef struct {

} status_t;

#endif // STATUS_H