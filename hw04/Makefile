CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

OBJECTS = events.o tally.o
TEST = events
TEST_TALLY = tests/test_tally

.PHONY: all test clean

all: $(TEST)

$(TEST): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

events.o: events.c tally.h
	$(CC) $(CFLAGS) -c -o $@ events.c

tally.o: tally.c tally.h
	$(CC) $(CFLAGS) -c -o $@ tally.c

tests/test_tally.o: tests/test_tally.c tally.h
	$(CC) $(CFLAGS) -c -o $@ tests/test_tally.c

$(TEST_TALLY): tests/test_tally.o tally.o
	$(CC) $(CFLAGS) -o $@ $^

test: $(TEST) $(TEST_TALLY)
	./$(TEST_TALLY)
	./$(TEST) < data/door.txt

clean:
	rm -f $(OBJECTS) $(TEST) tests/test_tally.o $(TEST_TALLY)