#include <stdio.h> 
#include <string.h> 

#include "tally.h" 

typedef struct {
	char name[TALLY_LEN + 1]; /* Stores the entry name and terminator. */
	int count; /* Stores the number of occurrences. */
} tally_entry_t;

struct tally {
	int in_use; /* Indicates whether this tally is allocated. */
	int size; /* Stores the number of entries currently used. */
	tally_entry_t entries[TALLY_NAMES]; /* Stores entries in sorted order. */
};

static tally_t tallies[TALLY_MAX]; /* Stores all statically allocated tallies. */

static int is_valid_tally(const tally_t *t) /* Receives the tally to validate. */
{
	int i;

	for (i = 0; i < TALLY_MAX; i++) { /* Checks every tally slot. */
		if (t == &tallies[i]) /* Tests whether t refers to this slot. */
			return tallies[i].in_use; /* Returns the slot's active state. */
	} /* Ends the slot search. */
	return 0; /* Rejects null, foreign, or inactive pointers. */
}

static int is_valid_name(const char *name) /* Receives the name to validate. */
{
	size_t length;

	if (name == NULL) /* Checks for a null name. */
		return 0; /* Rejects a null name. */
	length = strlen(name); /* Measures the supplied name. */
	return length > 0 && length <= TALLY_LEN; /* Checks the allowed length. */
}

tally_t * tally_create(void) /* Creates a tally. */
{
	int i;

	for (i = 0; i < TALLY_MAX; i++) { /* Searches for an unused slot. */
		if (!tallies[i].in_use) { /* Checks whether this slot is available. */
			tallies[i].in_use = 1; /* Marks the slot as active. */
			tallies[i].size = 0; /* Clears the entry count. */
			return &tallies[i]; /* Returns the allocated tally. */
		}
	}
	return NULL; /* No slot is available. */
}

void tally_destroy(tally_t *t) /* Receives the tally to release. */
{ 
	int i;

	if (t == NULL) /* Checks for a null pointer. */
		return; /* Ignores a null tally. */
	for (i = 0; i < TALLY_MAX; i++) { /* Searches for the tally slot. */
		if (t == &tallies[i]) { /* Checks whether this is the target slot. */
			tallies[i].in_use = 0; /* Marks the slot as inactive. */
			tallies[i].size = 0; /* Clears the number of entries. */
			return;
		}
	}
}

int tally_add(tally_t *t, const char *name)
{
	size_t length;
	int i;
	int position;

	if (!is_valid_tally(t) || !is_valid_name(name)) /* Input validation. */
		return -1; /* Reports invalid input. */

	length = strlen(name); /* Measures the validated name. */

	position = t->size; /* Defaults insertion to the end. */
	for (i = 0; i < t->size; i++) { /* Searches existing entries. */
		int comparison = strcmp(name, t->entries[i].name); /* Compares names. */

		if (comparison == 0) { /* Checks for an existing name. */
			t->entries[i].count++; /* Increments the existing count. */
			return t->entries[i].count; /* Returns the updated count. */
		}
		if (position == t->size && comparison < 0) /* Finds the first later name. */
			position = i; /* Saves the sorted insertion position. */
	}

	if (t->size == TALLY_NAMES) /* Checks whether the tally is full. */
		return -1; /* Insertion is impossible. */

	for (i = t->size; i > position; i--) /* Shifts later entries right. */
		t->entries[i] = t->entries[i - 1]; /* Copies the preceding entry. */

	memcpy(t->entries[position].name, name, length); /* Copies the name. */
	t->entries[position].name[length] = '\0'; /* Terminates the name. */
	t->entries[position].count = 1; /* Initializes the new count. */
	t->size++; /* Records the new entry. */
	return 1; /* Returns the initial count. */
}

int tally_count(const tally_t *t, const char *name) /* Receives a tally and name. */
{
	int i;

	if (!is_valid_tally(t) || !is_valid_name(name)) /* Input validation. */
		return 0; /* Reports no matching count. */
	for (i = 0; i < t->size; i++) { /* Checks every entry. */
		if (strcmp(name, t->entries[i].name) == 0) /* Compares the names. */
			return t->entries[i].count; /* Returns the matching count. */
	}
	return 0; /* Name was not found. */
}

int tally_size(const tally_t *t) /* Receives the tally to measure. */
{
	if (!is_valid_tally(t)) /* Validates the tally pointer. */
		return 0; /* Reports zero for an invalid tally. */
	return t->size; /* Returns the active entry count. */
}

void tally_print(const tally_t *t, FILE *out) /* Receives a tally and output stream. */
{
	int i;

	if (!is_valid_tally(t) || out == NULL) /*Input validation */
		return; /* Ignores invalid output requests. */
	for (i = 0; i < t->size; i++) /* Visits every stored entry. */
		fprintf(out, "%s %d\n", t->entries[i].name, t->entries[i].count); /* Writes one entry. */
}
