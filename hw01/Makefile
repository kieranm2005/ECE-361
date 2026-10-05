CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

OBJECTS = bits.o status.o
TEST = tests/test_bits

.PHONY: all test clean

all: $(OBJECTS)

test: $(TEST)
	./$(TEST)

$(TEST): tests/test_bits.c $(OBJECTS)
	$(CC) $(CFLAGS) -I. -o $@ $^

bits.o: bits.c bits.h
	$(CC) $(CFLAGS) -c $< -o $@

status.o: status.c status.h bits.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TEST)