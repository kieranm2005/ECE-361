CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -Werror
LDFLAGS =
LDLIBS  = -lm

OBJS    = sensor.o panel.o scenario.o main.o iom361_r4.o
TOBJS   = sensor.o panel.o scenario.o iom361_r4.o tests/test_hw06.o

all: replay tests/test_hw06

replay: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

tests/test_hw06: $(TOBJS)
	$(CC) $(CFLAGS) -I. $(LDFLAGS) -o $@ $(TOBJS) $(LDLIBS)

test: tests/test_hw06
	./tests/test_hw06

# one rule per .o listing the headers it includes
sensor.o: sensor.c sensor.h iom361_r4.h
	$(CC) $(CFLAGS) -c -o $@ sensor.c

panel.o: panel.c panel.h iom361_r4.h
	$(CC) $(CFLAGS) -c -o $@ panel.c

scenario.o: scenario.c scenario.h
	$(CC) $(CFLAGS) -c -o $@ scenario.c

main.o: main.c sensor.h panel.h scenario.h iom361_r4.h
	$(CC) $(CFLAGS) -c -o $@ main.c

iom361_r4.o: iom361_r4.c iom361_r4.h
	$(CC) $(CFLAGS) -c -o $@ iom361_r4.c

tests/test_hw06.o: tests/test_hw06.c sensor.h panel.h scenario.h iom361_r4.h
	$(CC) $(CFLAGS) -I. -c -o $@ tests/test_hw06.c

clean:
	rm -f $(OBJS) tests/test_hw06.o replay tests/test_hw06

.PHONY: all test clean