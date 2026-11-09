#include <ctype.h> // for isspace()
#include <errno.h> // for errno
#include <stdio.h> // for fopen, fgets, fclose
#include <stdlib.h> // for strtoul, strtof, strtoll
#include <string.h> // for strcmp, strlen, strchr

#include "scenario.h"

#define LINE_BUF 256

// trim leading and trailing whitespace in place; returns the start pointer
static char *trim(char *s)
{
    while (*s && isspace((unsigned char)*s)) // skip leading whitespace
        s++;
    char *end = s + strlen(s);
    while (end > s && isspace((unsigned char)end[-1])) // skip trailing whitespace
        *--end = '\0';
    return s;
}

// parse an unsigned 32-bit decimal number; returns 0 on success
static int parse_u32(const char *tok, uint32_t *out)
{
    errno = 0; // clear errno before parsing
    char *end = NULL; // pointer to the end of the parsed number
    unsigned long v = strtoul(tok, &end, 10); // parse the unsigned 32-bit decimal number
    if (errno != 0 || end == tok || *end != '\0' || v > 0xFFFFFFFFul) // check for errors and overflow
        return -1;
    *out = (uint32_t)v; // store the parsed unsigned 32-bit decimal number in the output parameter
    return 0;
}

// parse a float; returns 0 on success
static int parse_float(const char *tok, float *out)
{
    errno = 0; // clear errno before parsing
    char *end = NULL; // pointer to the end of the parsed number
    float v = strtof(tok, &end); // parse the float value
    if (errno != 0 || end == tok || *end != '\0') // check for errors and overflow
        return -1;
    *out = v; // store the parsed float value in the output parameter
    return 0;
}

// parse "0x<hex>"; returns 0 on success. A bare "5" is rejected on purpose.
static int parse_switches(const char *tok, uint32_t *out)
{
    if (tok[0] != '0' || (tok[1] != 'x' && tok[1] != 'X'))
        return -1;
    errno = 0; // clear errno before parsing
    char *end = NULL; // pointer to the end of the parsed number
    unsigned long v = strtoul(tok, &end, 16);
    if (errno != 0 || end == tok || *end != '\0' || v > 0xFFFFFFFFul) // check for errors and overflow
        return -1;
    *out = (uint32_t)v; // store the parsed hexadecimal value in the output parameter
    return 0;
}

int scenario_load(const char *path, scenario_t *s)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
        return 1;   // cannot open: report line 1

    s->start = 0; // initialize the start time of the scenario to 0
    s->nlines = 0; // initialize the number of lines in the scenario to 0

    char buf[LINE_BUF];
    int lineno = 0;
    int have_start = 0;

    while (fgets(buf, sizeof buf, fp) != NULL) {
        lineno++;

        // a line that did not fit is an error, not a silent truncation
        if (strchr(buf, '\n') == NULL && !feof(fp)) { // line did not fit in the buffer
            fclose(fp); // close the file before returning the error
            return lineno; // return the line number of the error
        }

        char *line = trim(buf); // trim leading and trailing whitespace
        if (*line == '\0' || *line == '#') // skip empty lines and comments
            continue;

        // split the line into tokens based on spaces and tabs
        char *tok[5]; // array to store the tokens of the current line
        int ntok = 0; // number of tokens in the current line
        for (char *p = strtok(line, " \t"); p != NULL; p = strtok(NULL, " \t")) { // iterate over tokens in the current line
            if (ntok == 5) {        // too many fields
                fclose(fp);
                return lineno;
            }
            tok[ntok++] = p; // store the current token in the array of tokens
        }

        if (strcmp(tok[0], "start") == 0) { // handle the "start" line
            if (have_start || ntok != 2) { // check for duplicate start or wrong number of fields
                fclose(fp);
                return lineno;
            }
            errno = 0;
            char *end = NULL; // pointer to the end of the parsed number
            long long v = strtoll(tok[1], &end, 10); // parse the start time as a long long integer
            if (errno != 0 || end == tok[1] || *end != '\0') { // check for errors and invalid input
                fclose(fp);
                return lineno;
            }
            s->start = (time_t)v; // store the parsed start time in the scenario structure
            have_start = 1; // indicate that the start line has been found
            continue;
        }

        // a data line
        if (!have_start || ntok != 4) { // check for missing start or wrong number of fields
            fclose(fp);
            return lineno;
        }

        scenario_line_t ln;
        if (parse_u32(tok[0], &ln.tick) != 0 ||
            parse_float(tok[1], &ln.temp_c) != 0 ||
            parse_float(tok[2], &ln.rh_pct) != 0 ||
            parse_switches(tok[3], &ln.switches) != 0) { // parse the switches field
            fclose(fp);
            return lineno;
        }

        if (s->nlines > 0 && ln.tick <= s->lines[s->nlines - 1].tick) { // check for non-increasing ticks
            fclose(fp);
            return lineno;
        }

        if (s->nlines >= SCENARIO_MAX_LINES) { // check for exceeding the maximum number of lines
            fclose(fp);
            return lineno;
        }

        s->lines[s->nlines++] = ln; // store the parsed line in the scenario structure
    }

    fclose(fp);
    return 0;
}

int scenario_at(const scenario_t *s, uint32_t tick, scenario_line_t *out)
{
    if (s->nlines == 0 || tick < s->lines[0].tick) // check for empty scenario or tick before the first line
        return -1;

    int lo = 0; // binary search for the largest tick less than or equal to the given tick
    int hi = s->nlines - 1; // the highest index in the scenario lines array
    int ans = 0; // store the index of the largest tick less than or equal to the given tick
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2; // calculate the middle index for binary search
        if (s->lines[mid].tick <= tick) { // if the middle tick is less than or equal to the target tick
            ans = mid; // update the answer to the current middle index
            lo = mid + 1; // search the right half
        } else { // if the middle tick is greater than the target tick
            hi = mid - 1; // search the left half
        }
    }
    *out = s->lines[ans]; // store the found line in the output parameter
    return 0; // indicate success
}

uint32_t scenario_last_tick(const scenario_t *s)
{
    return s->nlines ? s->lines[s->nlines - 1].tick : 0; // return the last tick in the scenario, or 0 if the scenario is empty
}