CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra
BONUS   = -O2

OBJS    = main.o reading.o stats.o histogram.o

TEST_OBJS = tests/test_stats.o stats.o

.PHONY: all test clean

all: readings

readings: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

main.o: main.c reading.h stats.h histogram.h
	$(CC) $(CFLAGS) -c -o $@ main.c

reading.o: reading.c reading.h
	$(CC) $(CFLAGS) -c -o $@ reading.c

stats.o: stats.c stats.h
	$(CC) $(CFLAGS) -c -o $@ stats.c

histogram.o: histogram.c histogram.h
	$(CC) $(CFLAGS) -c -o $@ histogram.c

tests/test_stats.o: tests/test_stats.c stats.h
	$(CC) $(CFLAGS) -c -o $@ tests/test_stats.c

tests/test_stats: $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJS)

test: tests/test_stats readings
	./tests/test_stats
	sh regress.sh

clean:
	rm -f readings tests/test_stats $(OBJS) tests/test_stats.o