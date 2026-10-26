#include <stdio.h>
#include <string.h>

#include "tally.h"

#define LINE_SIZE 256

static int read_line(char *line, size_t size)
{
	int has_newline;
	int character;

	if (fgets(line, size, stdin) == NULL)
		return 0;

	has_newline = strchr(line, '\n') != NULL;
	if (has_newline)
		line[strcspn(line, "\n")] = '\0';
	else {
		while ((character = getchar()) != '\n' && character != EOF)
			;
	}
	return 1;
}

int main(void)
{
	char line[LINE_SIZE];
	tally_t *tally;
	int skipped;

	tally = tally_create();
	if (tally == NULL) {
		fprintf(stderr, "unable to create tally\n");
		return 1;
	}

	skipped = 0;
	while (read_line(line, sizeof line)) {
		if (line[0] == '\0' || line[0] == '#')
			continue;
		if (tally_add(tally, line) < 0)
			skipped++;
	}

	if (ferror(stdin)) {
		fprintf(stderr, "error reading standard input\n");
		tally_destroy(tally);
		return 1;
	}

	tally_print(tally, stdout);
	printf("names: %d, lines skipped: %d\n", tally_size(tally), skipped);
	tally_destroy(tally);
	return 0;
}