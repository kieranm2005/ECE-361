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
bool heat; // 1 = heater on
bool cool; // 1 = compressor on
bool fan; // 1 = fan on
bool fault; // 1 = fault present
uint8_t mode; // 0 = off, 1 = heat, 2 = cool, 3 = auto, 4 = fan only, 5 = invalid
bool reserved; // Must be zero
int8_t setpoint; // Two's complement
} status_t;

status_t status_unpack(uint16_t word);

#endif // STATUS_H