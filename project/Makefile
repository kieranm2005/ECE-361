CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra
TARGET  = serverroom

OBJS    = main.o \
          sensor.o fsm.o store.o aux.o report.o \
          iom361_r4.o

.PHONY: all test clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

main.o: main.c iom361_r4.h
	$(CC) $(CFLAGS) -c -o $@ main.c

sensor.o: sensor.c sensor.h
	$(CC) $(CFLAGS) -c -o $@ sensor.c

fsm.o: fsm.c fsm.h
	$(CC) $(CFLAGS) -c -o $@ fsm.c

store.o: store.c store.h sensor.h
	$(CC) $(CFLAGS) -c -o $@ store.c

aux.o: aux.c aux.h sensor.h
	$(CC) $(CFLAGS) -c -o $@ aux.c

report.o: report.c report.h store.h sensor.h
	$(CC) $(CFLAGS) -c -o $@ report.c

iom361_r4.o: iom361_r4.c iom361_r4.h
	$(CC) $(CFLAGS) -c -o $@ iom361_r4.c

# Project tests in HW 9
test: $(TARGET)
	@echo "project tests land in HW9; $(TARGET) built"

clean:
	rm -f $(TARGET) $(OBJS)