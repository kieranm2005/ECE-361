#include <stdio.h>
#include <string.h>

#include "../tally.h"

static int tests_run;
static int tests_failed;

static void report(const char *name, int passed) /* Receives the test name and result. */
{
	tests_run++;
	if (passed)
		printf("PASS: %s\n", name);
	else {
		printf("FAIL: %s\n", name);
		tests_failed++;
	}
}

static int test_counting(void) /* Tests adding and counting names. */
{
	tally_t *t = tally_create();
	int passed;

	passed = t != NULL && tally_add(t, "door") == 1
		&& tally_add(t, "door") == 2
		&& tally_count(t, "door") == 2
		&& tally_size(t) == 1;
	tally_destroy(t);
	return passed;
}

static int test_missing_name(void) /* Tests counting a name that was never added. */
{
	tally_t *t = tally_create();
	int passed;

	passed = t != NULL && tally_add(t, "present") == 1
		&& tally_count(t, "missing") == 0
		&& tally_count(t, "present") == 1
		&& tally_size(t) == 1;
	tally_destroy(t);
	return passed;
}

static int test_name_lengths(void) /* Tests adding names of valid and invalid lengths. */
{
	char valid[TALLY_LEN + 1];
	char invalid[TALLY_LEN + 2];
	tally_t *t;
	int passed;

	memset(valid, 'a', TALLY_LEN);
	valid[TALLY_LEN] = '\0';
	memset(invalid, 'b', TALLY_LEN + 1);
	invalid[TALLY_LEN + 1] = '\0';
	t = tally_create();
	passed = t != NULL && tally_add(t, valid) == 1
		&& tally_count(t, valid) == 1
		&& tally_add(t, invalid) == -1
		&& tally_size(t) == 1;
	tally_destroy(t);
	return passed;
}

static int test_invalid_arguments(void) /* Tests passing NULL and empty arguments. */
{
	FILE *file = tmpfile();
	tally_t *t = tally_create();
	int passed;

	passed = t != NULL && tally_add(t, "") == -1
		&& tally_add(t, NULL) == -1
		&& tally_add(NULL, "name") == -1
		&& tally_count(t, NULL) == 0
		&& tally_count(NULL, "name") == 0
		&& tally_size(NULL) == 0;
	if (passed && file == NULL)
		passed = 0;
	if (passed) {
		tally_print(NULL, file);
		rewind(file);
		passed = fgetc(file) == EOF;
	}
	tally_print(t, NULL);
	tally_destroy(NULL);
	passed = passed && tally_add(t, "valid") == 1
		&& tally_count(t, "valid") == 1;
	if (file != NULL)
		fclose(file);
	tally_destroy(t);
	return passed;
}

static int test_full_tally(void) /* Tests adding names until the tally is full. */
{
	char name[8];
	tally_t *t = tally_create();
	int i;
	int passed = t != NULL;

	for (i = 0; passed && i < TALLY_NAMES; i++) {
		snprintf(name, sizeof name, "n%d", i);
		passed = tally_add(t, name) == 1;
	}
	passed = passed && tally_add(t, "new") == -1
		&& tally_add(t, "n0") == 2
		&& tally_count(t, "n0") == 2
		&& tally_size(t) == TALLY_NAMES;
	tally_destroy(t);
	return passed;
}

static int test_tally_limit(void) /* Tests creating the maximum number of tallies. */
{
	tally_t *tallies[TALLY_MAX];
	tally_t *extra;
	int i;
	int passed = 1;

	for (i = 0; i < TALLY_MAX; i++) {
		tallies[i] = tally_create();
		passed = passed && tallies[i] != NULL;
	}
	extra = tally_create();
	passed = passed && extra == NULL;
	for (i = 0; i < TALLY_MAX; i++)
		tally_destroy(tallies[i]);
	return passed;
}

static int test_destroy_and_independence(void) /* Tests destroying a tally and creating a new one. */
{
	tally_t *first = tally_create();
	tally_t *second;
	int passed;

	passed = first != NULL && tally_add(first, "first") == 1;
	tally_destroy(first);
	second = tally_create();
	passed = passed && second != NULL
		&& tally_count(second, "first") == 0
		&& tally_add(second, "second") == 1;
	tally_destroy(second);
	return passed;
}

static int test_concurrent_independence(void) /* Tests two live tallies independently. */
{
	tally_t *first = tally_create();
	tally_t *second = tally_create();
	int passed;

	passed = first != NULL && second != NULL
		&& tally_add(first, "first") == 1
		&& tally_add(first, "first") == 2
		&& tally_count(second, "first") == 0
		&& tally_add(second, "second") == 1
		&& tally_count(first, "second") == 0;
	tally_destroy(first);
	tally_destroy(second);
	return passed;
}

static int test_print_order(void) /* Tests that tally_print outputs names in sorted order. */
{
	char output[64];
	FILE *file;
	tally_t *t = tally_create();
	int passed;

	passed = t != NULL && tally_add(t, "charlie") == 1 /* Adds names in non-sorted order. */
		&& tally_add(t, "alpha") == 1  
		&& tally_add(t, "bravo") == 1;
	file = tmpfile();
	if (passed && file == NULL)
		passed = 0;
	if (passed) {
		tally_print(t, file); // Writes the tally to the temporary file.
		rewind(file); // Resets the file position indicator to the beginning of the file.
		passed = fgets(output, sizeof output, file) != NULL
			&& strcmp(output, "alpha 1\n") == 0
			&& fgets(output, sizeof output, file) != NULL
			&& strcmp(output, "bravo 1\n") == 0
			&& fgets(output, sizeof output, file) != NULL
			&& strcmp(output, "charlie 1\n") == 0
			&& fgets(output, sizeof output, file) == NULL;
	}
	if (file != NULL)
		fclose(file);
	tally_destroy(t);
	return passed;
}

int main(void) /* Runs all tests and reports the results. */
{
	report("counting up", test_counting());
	report("name never added", test_missing_name());
	report("name length limits", test_name_lengths());
	report("NULL and empty arguments", test_invalid_arguments());
	report("full tally", test_full_tally());
	report("tally allocation limit", test_tally_limit());
	report("destroy and reuse", test_destroy_and_independence());
	report("concurrent tally independence", test_concurrent_independence());
	report("print order", test_print_order());
	printf("Summary: %d tests, %d passed, %d failed\n",
		tests_run, tests_run - tests_failed, tests_failed);
	return tests_failed == 0 ? 0 : 1; /* Returns 0 if all tests passed, 1 otherwise. */
}