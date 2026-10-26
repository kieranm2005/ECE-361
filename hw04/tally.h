#ifndef TALLY_H
#define TALLY_H

#include <stdio.h>

#define TALLY_MAX 4 /* tallies that can exist at the same time */
#define TALLY_NAMES 16 /* different names one tally can hold */
#define TALLY_LEN 15 /* longest name, without the '\0' */

typedef struct tally tally_t; /* opaque */

/*
 * Takes no arguments. Returns a pointer to a new empty tally, or NULL when
 * TALLY_MAX tallies are already in use. Assumes the caller will destroy the
 * returned tally when it is no longer needed.
 */
tally_t *tally_create(void);

/*
 * Takes a tally pointer previously returned by tally_create, or NULL.
 * Returns nothing. Assumes the tally is not used after it is destroyed;
 * a NULL pointer has no effect.
 */
void tally_destroy(tally_t *t);

/*
 * Takes a valid tally pointer and a nonempty name no longer than TALLY_LEN.
 * Returns the name's new count, or -1 if an argument is invalid or a new name
 * cannot fit. Assumes name points to a null-terminated string and that t has
 * not been destroyed.
 */
int tally_add(tally_t *t, const char *name);

/*
 * Takes a valid tally pointer and a name to find. Returns the number of times
 * the name was added, or 0 if the name is absent or an argument is invalid.
 * Assumes name points to a null-terminated string and t has not been destroyed.
 */
int tally_count(const tally_t *t, const char *name);

/*
 * Takes a valid tally pointer. Returns the number of different names it holds,
 * or 0 for NULL or an invalid pointer. Assumes t has not been destroyed.
 */
int tally_size(const tally_t *t);

/*
 * Takes a valid tally pointer and an output stream. Returns nothing. Assumes
 * out is open for writing; prints one line per name in strcmp order. A NULL
 * tally or output stream has no effect.
 */
void tally_print(const tally_t *t, FILE *out);

#endif