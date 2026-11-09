#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "iom361_r4.h"
#include "sensor.h"
#include "panel.h"
#include "scenario.h"

static int failures = 0;

#define CHECK(cond, name)                                            \
    do {                                                             \
        if (cond) {                                                  \
            printf("PASS  %s\n", (name));                            \
        } else {                                                     \
            printf("FAIL  %s\n", (name));                            \
            failures++;                                              \
        }                                                            \
    } while (0)

static int approx(float a, float b, float eps)
{
    return fabsf(a - b) <= eps;
}

/* --- sensor --- */

static void test_sensor_91(void)
{
    int rc;
    uint32_t *base = iom361_initialize(4, 4, &rc);
    iom361_set_display(false);
    _iom361_set_sensor1(91.0f, 40.0f);

    reading_t r = { .t = 0, .temp_c = -999.0f, .rh_pct = -999.0f };
    int s = sensor_read(base, (time_t)1234, &r);
    CHECK(s == 0 && approx(r.temp_c, 91.0f, 0.05f) &&
          approx(r.rh_pct, 40.0f, 0.05f) && r.t == (time_t)1234,
          "sensor reads 91.0 C back as 91.0");
}

static void test_sensor_failed_read_unchanged(void)
{
    /* do not initialize: iom361 returns rc 5 and 0xDEADBEEF */
    uint32_t bogus = 0;
    reading_t r = { .t = 7, .temp_c = 11.0f, .rh_pct = 22.0f };
    int s = sensor_read(&bogus, (time_t)99, &r);
    CHECK(s != 0 && r.t == (time_t)7 && r.temp_c == 11.0f && r.rh_pct == 22.0f,
          "failed sensor read leaves *r unchanged");
}

/* --- panel --- */

static void test_pressed_released(void)
{
    CHECK(panel_pressed(0x1, 0x0) == 0x1, "press: 0->1 is pressed");
    CHECK(panel_pressed(0x1, 0x1) == 0x0, "held switch is not pressed again");
    CHECK(panel_released(0x0, 0x1) == 0x1, "release: 1->0 is released");
    CHECK(panel_released(0x1, 0x1) == 0x0, "held switch is not released");
}

static void test_led_preserves_others(void)
{
    int rc;
    uint32_t *base = iom361_initialize(4, 4, &rc);
    iom361_set_display(false);

    iom361_writeReg(base, LEDS_REG, 0x0u, &rc);
    CHECK(panel_led(base, 1, 1) == 0x2, "panel_led(1,on) sets bit 1");
    CHECK(panel_led(base, 2, 1) == 0x6, "panel_led(2,on) preserves bit 1");
    CHECK(panel_led(base, 1, 0) == 0x4, "panel_led(1,off) preserves bit 2");

    CHECK(panel_led(base, 1, 1) == 0x6, "reset for toggle test");
    CHECK(panel_toggle(base, 2) == 0x2, "panel_toggle(2) clears bit 2, keeps 1");
    CHECK(panel_toggle(base, 0) == 0x3, "panel_toggle(0) sets bit 0, keeps 1");

    CHECK(panel_led(base, -1, 1) == -1, "panel_led(-1) is -1");
    CHECK(panel_led(base, 32, 1) == -1, "panel_led(32) is -1");
    CHECK(panel_toggle(base, -1) == -1, "panel_toggle(-1) is -1");
    CHECK(panel_toggle(base, 32) == -1, "panel_toggle(32) is -1");
}

/* --- scenario --- */

static void write_file(const char *path, const char *text)
{
    FILE *fp = fopen(path, "w");
    if (fp == NULL) {
        perror(path);
        return;
    }
    fputs(text, fp);
    fclose(fp);
}

static void test_scenario_ok(void)
{
    const char *path = "test_scn_ok.txt";
    write_file(path,
        "# a comment\n"
        "\n"
        "start 1791273600\n"
        "# another comment\n"
        "0 24.0 45.0 0x0\n"
        "2 25.0 45.0 0x1\n"
        "5 26.0 50.0 0x3\n");

    scenario_t s;
    int rc = scenario_load(path, &s);
    CHECK(rc == 0 && s.nlines == 3 && s.start == (time_t)1791273600,
          "scenario: comments and blank lines load");

    scenario_line_t ln;
    CHECK(scenario_at(&s, 1, &ln) == 0 && ln.tick == 0 && ln.temp_c == 24.0f,
          "scenario: values hold between lines (tick 1 -> line tick 0)");
    CHECK(scenario_at(&s, 2, &ln) == 0 && ln.tick == 2 && ln.switches == 0x1u,
          "scenario: exact line matches");
    CHECK(scenario_at(&s, 5, &ln) == 0 && ln.tick == 5 && ln.switches == 0x3u,
          "scenario: last line matches");
    CHECK(scenario_last_tick(&s) == 5u, "scenario: last tick is 5");
}

static void test_scenario_no_start(void)
{
    const char *path = "test_scn_nostart.txt";
    write_file(path, "0 24.0 45.0 0x0\n");
    scenario_t s;
    CHECK(scenario_load(path, &s) == 1,
          "scenario: data line before start is line 1");
}

static void test_scenario_missing_field(void)
{
    const char *path = "test_scn_missing.txt";
    write_file(path, "start 0\n0 24.0 45.0\n");
    scenario_t s;
    CHECK(scenario_load(path, &s) == 2,
          "scenario: missing field is line 2");
}

static void test_scenario_nonmonotonic(void)
{
    const char *path = "test_scn_ticks.txt";
    write_file(path, "start 0\n0 24.0 45.0 0x0\n0 25.0 45.0 0x0\n");
    scenario_t s;
    CHECK(scenario_load(path, &s) == 3,
          "scenario: non-increasing tick is line 3");
}

static void test_scenario_tick_before_first(void)
{
    const char *path = "test_scn_first.txt";
    write_file(path, "start 0\n3 24.0 45.0 0x0\n");
    scenario_t s;
    scenario_line_t ln;
    CHECK(scenario_load(path, &s) == 0 && scenario_at(&s, 2, &ln) == -1,
          "scenario: tick before first line has no world");
}

int main(void)
{
    test_sensor_91();
    test_sensor_failed_read_unchanged();

    test_pressed_released();
    test_led_preserves_others();

    test_scenario_ok();
    test_scenario_no_start();
    test_scenario_missing_field();
    test_scenario_nonmonotonic();
    test_scenario_tick_before_first();

    if (failures) {
        printf("%d test(s) failed\n", failures);
        return 1;
    }
    printf("all tests passed\n");
    return 0;
}