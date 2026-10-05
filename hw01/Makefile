CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g
app: main.o util.o
$(CC) $(CFLAGS) -o $@ $^
%.o: %.c util.h
$(CC) $(CFLAGS) -c $<
clean:
rm -f *.o app
.PHONY: clean