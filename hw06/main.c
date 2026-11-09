#include <stdio.h>
#include <time.h>

#include "iom361_r4.h"
#include "sensor.h"
#include "panel.h"
#include "scenario.h"

#define N_SWITCHES 4
#define N_LEDS     4

static void print_time(time_t t)
{
    struct tm *tm_utc = gmtime(&t); // convert the timestamp to UTC
    char buf[32]; // buffer to hold the formatted time string

    if (tm_utc == NULL) { // if conversion fails
        // cannot happen for sane timestamps
        fputs("??" "??" "-" "??" "-" "??" " " "??" ":" "??" ":" "??", stdout);
        return;
    }
    if (strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", tm_utc) == 0) { // if formatting fails
        fputs("??" "??" "-" "??" "-" "??" " " "??" ":" "??" ":" "??", stdout);
        return;
    }
    fputs(buf, stdout); // print the formatted time string
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s scenario.txt\n", argv[0]); // print usage message if incorrect number of arguments
        return 2;
    }

    scenario_t sc;
    int line = scenario_load(argv[1], &sc);
    if (line != 0) {
        fprintf(stderr, "%s: line %d: bad scenario line\n", argv[1], line); // print error message if a scenario line is bad
        return 1;
    }
    if (sc.nlines == 0) {
        fprintf(stderr, "%s: no scenario lines\n", argv[1]); // print error message if there are no scenario lines
        return 1;
    }
    if (sc.lines[0].tick != 0) {
        fprintf(stderr, "%s: line %d: first tick is %u, expected 0\n",
                argv[1], 2, (unsigned)sc.lines[0].tick); // print error message if the first tick is not 0
        return 1;
    }

    int rc;
    uint32_t *base = iom361_initialize(N_SWITCHES, N_LEDS, &rc); // initialize the IOM361 device with the specified number of switches and LEDs
    if (base == NULL) {
        fprintf(stderr, "iom361_initialize failed: %d\n", rc); // print error message if initialization fails
        return 1;
    }
    iom361_set_display(false); // disable the display

    uint32_t before_sw = 0; // store the previous switch state
    uint32_t last_tick = scenario_last_tick(&sc); // get the last tick of the scenario

    for (uint32_t tick = 0; tick <= last_tick; tick++) {
        scenario_line_t ln; // structure to hold the current scenario line
        if (scenario_at(&sc, tick, &ln) != 0) { // if there is no scenario line for the current tick
            fprintf(stderr, "internal error: no line for tick %u\n", (unsigned)tick); // print error message if there is no scenario line for the current tick
            return 1;
        }

        _iom361_set_sensor1(ln.temp_c, ln.rh_pct); // set the temperature and humidity sensor values
        _iom361_set_switches(ln.switches); // set the switch states

        time_t now = sc.start + (time_t)tick; // calculate the current time based on the scenario start time and the current tick

        reading_t r;
        if (sensor_read(base, now, &r) != 0) { // read the sensor values
            fprintf(stderr, "sensor_read failed at tick %u\n", (unsigned)tick); // print error message if sensor read fails
            return 1;
        }

        uint32_t now_sw = iom361_readReg(base, SWITCHES_REG, &rc); // read the current switch states
        if (rc != 0) {
            fprintf(stderr, "switch read failed at tick %u\n", (unsigned)tick); // print error message if switch read fails
            return 1;
        }

        uint32_t pressed = panel_pressed(now_sw, before_sw); // determine which switches were pressed since the last tick

        for (int k = 0; k < N_SWITCHES; k++) { // iterate over all switches
            if (pressed & ((uint32_t)1u << k)) { // if the switch was pressed
                if (panel_toggle(base, k) < 0) { // toggle the switch
                    fprintf(stderr, "panel_toggle(%d) failed at tick %u\n",
                            k, (unsigned)tick); // print error message if toggle fails
                    return 1;
                }
            }
        }

        uint32_t leds = iom361_readReg(base, LEDS_REG, &rc); // read the current LED states
        if (rc != 0) {
            fprintf(stderr, "LED read failed at tick %u\n", (unsigned)tick); // print error message if LED read fails
            return 1;
        }

        print_time(r.t); // print the current time
        printf(" temp=%.1f rh=%.1f switches=0x%x pressed=0x%x leds=0x%x\n", // print the sensor values, switch states, pressed switches, and LED states
               (double)r.temp_c, (double)r.rh_pct,
               (unsigned)now_sw, (unsigned)pressed, (unsigned)leds);

        before_sw = now_sw;
    }
    return 0;
}