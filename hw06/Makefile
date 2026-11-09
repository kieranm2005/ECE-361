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
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(TOBJS) $(LDLIBS)

# one rule per .o listing the headers it includes
sensor.o: sensor.c sensor.h iom361_r4.h
	$(CC) $(CFLAGS) -c -o $@ sensor.c

panel.o: panel.c panel.h