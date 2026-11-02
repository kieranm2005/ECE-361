#include <stdio.h>
#include "lock.h"

int main(void)
{
    state_t s = LOCKED; // Initialize the lock state to LOCKED
    int ch; // Variable to store the character read from input
    while ((ch = getchar()) != EOF) { // Read characters from input until EOF
        if (ch == '\n' || ch == '\r' || ch == ' ' || ch == '\t') // Skip whitespace characters
            continue;
        int e = lock_event(s, (char)ch); // Determine the event based on the current state and input character
        if (e < 0) { // If the event is invalid
            fprintf(stderr, "ignoring unknown char '%c'\n", ch); // Print an error message
            continue; // Skip to the next character
        }
        action_t a; // Variable to store the action to be performed
        state_t next = lock_step(s, (event_t)e, &a); // Compute the next state and action
        printf("%s %c %s -> %s %s\n", // Print the transition details
               state_name(s), ch, event_name((event_t)e),
               state_name(next), action_name(a));
        s = next; // Update the current state to the next state
    }
    printf("final: %s\n", state_name(s)); // Print the final state of the lock
    return 0;
}