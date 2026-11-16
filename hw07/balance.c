#include "istack.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char kind; // the kind of bracket that was opened
} open_info;

static int kind_matches(char open, char close)
{
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}'); // check if the open and close brackets match
}

int main(void)
{
    istack_t *s = istack_create(); // create a new stack to keep track of open brackets
    if (s == NULL) { // check if stack creation failed
        fprintf(stderr, "out of memory\n"); // print an error message if memory allocation fails
        return 1; // return an error code if stack creation fails
    }

    size_t line = 1, col = 0; // initialize line and column counters to keep track of the position in the input
    size_t deepest = 0; // initialize a variable to keep track of the deepest level of nested brackets
    size_t pairs   = 0; // initialize a variable to keep track of the number of matched pairs of brackets
    int    c; // variable to hold the current character read from input
    int    exit_status = 0; // initialize a variable to keep track of the exit status of the program

    while ((c = fgetc(stdin)) != EOF) { // read characters from standard input until EOF is reached
        if (c == '\n') { // check if the current character is a newline character
            line++; // increment the line counter if a newline character is encountered
            col = 0; // reset the column counter to 0 for the new line
            continue; // skip the rest of the loop and continue to the next iteration
        }
        col++; // increment the column counter for each character read

        char ch = (char)c; // cast the current character to a char type for processing
        if (ch == '(' || ch == '[' || ch == '{') { // check if the current character is an opening bracket
            if (istack_push(s, ch) != 0) { // attempt to push the opening bracket onto the stack
                istack_destroy(s); // destroy the stack if pushing fails (e.g., due to memory allocation failure)
                return 1; // return an error code if pushing onto the stack fails
            }
            if (istack_count(s) > deepest)
                deepest = istack_count(s); // update the deepest level of nested brackets if the current stack count exceeds the previous deepest value
        } else if (ch == ')' || ch == ']' || ch == '}') { // check if the current character is a closing bracket
            int top = 0;
            if (istack_peek(s, &top) != 0) { // attempt to peek at the top of the stack to get the last opened bracket
                printf("line %zu, column %zu: found '%c', nothing open\n",
                       line, col, ch); // print an error message if there is no matching opening bracket for the current closing bracket
                exit_status = 1; // set the exit status to indicate an error occurred
                goto done; // jump to the done label to clean up and exit
            }

            if (!kind_matches((char)top, ch)) { // check if the top of the stack matches the current closing bracket
                printf("line %zu, column %zu: found '%c', expected '%c'\n",
                       line, col, ch,
                       top == '(' ? ')' : top == '[' ? ']' : '}'); // print an error message if the brackets do not match, indicating what was expected
                exit_status = 1; // set the exit status to indicate an error occurred
                goto done; // jump to the done label to clean up and exit
            }
            (void)istack_pop(s, NULL); // pop the matching opening bracket from the stack since a valid pair has been found
            pairs++; // increment the count of matched pairs of brackets
        }
    }

    if (!istack_is_empty(s)) { // check if there are any unmatched opening brackets left in the stack after processing all input
        int top = 0; // variable to hold the top of the stack (the last unmatched opening bracket)
        (void)istack_peek(s, &top); // peek at the top of the stack to get the last unmatched opening bracket
        printf("end of input: %zu still open, innermost '%c'\n",
               istack_count(s), (char)top); // print an error message indicating how many unmatched opening brackets remain and what the innermost unmatched bracket is
        exit_status = 1; // set the exit status to indicate an error occurred due to unmatched opening brackets
    } else { // if the stack is empty, all brackets have been matched correctly
        printf("balanced: %zu pairs, deepest %zu\n", pairs, deepest); // print a success message indicating that the brackets are balanced, along with the number of matched pairs and the deepest level of nesting
        exit_status = 0; // set the exit status to indicate success
    }

done:
istack_destroy(s); // destroy the stack to free any allocated memory before exiting the program
    return exit_status; // return the exit status of the program, indicating success or failure based on whether the brackets were balanced
}