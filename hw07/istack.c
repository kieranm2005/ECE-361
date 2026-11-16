#include "istack.h"
#include <stdlib.h>
#include <string.h>

struct istack {
    int *data;      // pointer to the stack's data
    size_t count;   // number of elements in the stack
    size_t capacity;// current capacity of the stack
};

istack_t *istack_create(void) {
    istack_t *s = malloc(sizeof *s); // allocate memory for the stack structure

    if (s == NULL) { // check if memory allocation failed
        return NULL; // return NULL if memory allocation fails
    }

    s->data = malloc(ISTACK_START * sizeof *s->data); // allocate memory for the stack's data

    if (s->data == NULL) { // check if memory allocation failed
        free(s); // free the stack structure if data allocation fails
        return NULL; // return NULL if memory allocation fails
    }

    s->count = 0; // initialize the count of elements to 0
    s->capacity = ISTACK_START; // initialize the capacity to ISTACK_START

    return s; // return the newly created stack

}

void istack_destroy(istack_t *s) {
    if (s == NULL) {
        return; // do nothing if the stack is NULL
    }
    free(s->data); // free the stack's data
    free(s); // free the stack structure
}

int istack_push(istack_t *s, int v)
{
    if (s == NULL) {
        return -1;
    }

    if (s->count == s->capacity) { // check if the stack is full
        size_t new_capacity = s->capacity * 2; // double the capacity if the stack is full

        if (new_capacity < s->capacity) {
            return -1; // check for overflow when doubling capacity
        }

        int *temp = realloc(s->data, new_capacity * sizeof *temp); // reallocate memory for the stack's data with the new capacity

        if (temp == NULL) // check if memory allocation failed
            return -1;

        s->data     = temp; // update the stack's data pointer to the new memory block
        s->capacity = new_capacity; // update the stack's capacity to the new capacity
    }

    s->data[s->count++] = v; // add the new value to the stack and increment the count
    return 0; // return 0 to indicate success
}

int istack_pop(istack_t *s, int *v)
{
    if (s == NULL || s->count == 0) {
        return -1;
    }

    int top = s->data[--s->count]; // decrement the count and retrieve the top value from the stack

    // if a value pointer is provided, store the popped value there
    if (v != NULL) {
        *v = top;
    }

    return 0;
}

int istack_peek(const istack_t *s, int *v)
{
    if (s == NULL || s->count == 0) // check if the stack is NULL or empty
        return -1;

    if (v != NULL) // check if the value pointer is not NULL
        *v = s->data[s->count - 1]; // store the top value in the provided pointer
    return 0;
}

size_t istack_count(const istack_t *s)
{
    return s ? s->count : 0; // return the count of elements in the stack, or 0 if the stack is NULL
}

bool istack_is_empty(const istack_t *s)
{
    return s ? s->count == 0 : true; // return true if the stack is NULL or empty, false otherwise
}

size_t istack_capacity(const istack_t *s)
{
    return s ? s->capacity : 0; // return the capacity of the stack, or 0 if the stack is NULL
}

int *istack_to_array(const istack_t *s, size_t *n)
{
    if (n == NULL) // check if the size pointer is NULL
        return NULL;
    *n = 0;
    if (s == NULL || s->count == 0) // check if the stack is NULL or empty
        return NULL;

    int *arr = malloc(s->count * sizeof *arr); // allocate memory for the array to hold the stack's values
    if (arr == NULL) // check if memory allocation failed
        return NULL;

    memcpy(arr, s->data, s->count * sizeof *arr); // copy the stack's values to the new array
    *n = s->count; // set the size pointer to the number of values copied
    return arr; // return the new array containing the stack's values
}
