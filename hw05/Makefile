CC      = cc
CFLAGS  = -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra
TARGET  = lock
OBJS    = lock.o main.o
TOBJS   = lock.o tests/test_lock.o
BOBJS   = lock.o check.o tests/test_check.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

lock.o: lock.c lock.h
	$(CC) $(CFLAGS) -c lock.c

main.o: main.c lock.h
	$(CC) $(CFLAGS) -c main.c

tests/test_lock.o: tests/test_lock.c lock.h
	$(CC) $(CFLAGS) -c tests/test_lock.c -o tests/test_lock.o

tests/test_lock: $(TOBJS)
	$(CC) $(CFLAGS) -o $@ $(TOBJS)

check.o: check.c check.h lock.h
	$(CC) $(CFLAGS) -c check.c

tests/test_check.o: tests/test_check.c check.h lock.h
	$(CC) $(CFLAGS) -c tests/test_check.c -o tests/test_check.o

check_app: $(BOBJS)
	$(CC) $(CFLAGS) -o $@ $(BOBJS)

test: tests/test_lock
	./tests/test_lock

bonus: check_app
	./check_app

clean:
	rm -f $(TARGET) check_app lock.o main.o check.o \
	      tests/test_lock.o tests/test_check.o tests/test_lock

.PHONY: all test bonus clean