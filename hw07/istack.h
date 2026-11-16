#ifndef ISTACK_H
#define ISTACK_H

#include <stddef.h>
#include <stdbool.h>

#define ISTACK_START 8 /* room at first; doubles when full */

typedef struct istack istack_t; /* opaque */


/*Creates a new empty stack with room for ISTACK_START values
* Returns NULL if memory runs out (and nothing is leaked)*/
istack_t *istack_create(void);

/* Frees the stack and everything it holds; NULL does nothing.*/
void istack_destroy(istack_t *s);

/* Pushes v. If the stack is full, first doubles the room with realloc.
 * Returns 0 on success, or -1 if s is NULL or memory runs out.
 * On failure the stack is unchanged. */
int istack_push(istack_t *s, int v);

/* Removes the top value and stores it in *v (v may be NULL).
 * Returns 0 on success, or -1 if s is NULL or empty.
 * On failure *v is unchanged. */
int istack_pop(istack_t *s, int *v);

/* Stores the top value in *v (v may be NULL) without removing it.
 * Returns 0 on success, or -1 if s is NULL or empty.
 * On failure *v is unchanged. */
int istack_peek(const istack_t *s, int *v);

/* Returns the number of values held; 0 for NULL. */
size_t istack_count(const istack_t *s);

/* Returns true if the stack holds no values; true for NULL. */
bool istack_is_empty(const istack_t *s);

/* Returns the room right now; 0 for NULL. */
size_t istack_capacity(const istack_t *s);

/* Returns a new heap array of the values, bottom first, with their number
 * in *n. The caller owns it and frees it.
 * Returns NULL and *n = 0 if s is NULL or empty, or memory runs out. */
int *istack_to_array(const istack_t *s, size_t *n);

#endif