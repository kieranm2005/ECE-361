#include "bits.h"
#include "status.h"

status_t status_unpack(uint16_t word) {
    status_t status; // Initialize the status structure
    status.heat = (get_field(word, HEAT_POS, HEAT_WIDTH) != 0);
    status.cool = (get_field(word, COOL_POS, COOL_WIDTH) != 0);
    status.fan = (get_field(word, FAN_POS, FAN_WIDTH) != 0);
    status.fault = (get_field(word, FAULT_POS, FAULT_WIDTH) != 0);

    uint32_t raw_mode = get_field(word, MODE_POS, MODE_WIDTH);
    if (raw_mode >= 5 && raw_mode <= 7) {
        status.mode = MODE_INVALID; // Modes 5, 6, 7 are invalid
    } else {
        status.mode = (uint8_t)raw_mode; // Cast from uint32_t to uint8_t to fit the status_t structure
    }

    status.reserved = (get_field(word, RESERVED_POS, RESERVED_WIDTH) != 0);
    status.setpoint = (int8_t)sign_extend(get_field(word, SETPOINT_POS, SETPOINT_WIDTH), SETPOINT_WIDTH);

    return status;
}