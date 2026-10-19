CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

OBJECTS = ptr.o tests/test_ptr.o
TEST = tests/test_ptr

.PHONY: all test clean

all: ptr.o $(TEST)

$(TEST): ptr.o tests/test_ptr.o
	$(CC) $(CFLAGS) -o $@ $^

ptr.o: ptr.c ptr.h
	$(CC) $(CFLAGS) -c -o $@ ptr.c

tests/test_ptr.o: tests/test_ptr.c ptr.h
	$(CC) $(CFLAGS) -c -o $@ tests/test_ptr.c

test: $(TEST)
	./$(TEST)

clean:
	rm -f $(OBJECTS) $(TEST)