CC      ?= gcc
CFLAGS  ?= -std=c11 -Wall -Wextra -Wpedantic 
SANFLAGS = -fsanitize=address,undefined

TARGET  = balance
TESTBIN = tests/test_istack
OBJS    = istack.o balance.o
TESTOBJS= istack.o tests/test_istack.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

istack.o: istack.c istack.h
	$(CC) $(CFLAGS) -c -o $@ $<

balance.o: balance.c istack.h
	$(CC) $(CFLAGS) -c -o $@ $<

tests/test_istack.o: tests/test_istack.c istack.h
	$(CC) $(CFLAGS) -I. -c -o $@ $<

$(TESTBIN): $(TESTOBJS)
	$(CC) $(CFLAGS) -o $@ $^

test: $(TARGET) $(TESTBIN)
	./$(TESTBIN)
	sh regress.sh

memcheck:
	$(MAKE) clean
	$(MAKE) CFLAGS="$(CFLAGS) $(SANFLAGS)" LDFLAGS="$(SANFLAGS)" test

clean:
	rm -f $(TARGET) $(TESTBIN) *.o tests/*.o

.PHONY: all test memcheck clean