CC      = cc
CFLAGS  = -std=c11 -Wall -Wextra -Wpedantic -g
SANFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer

SRC     = bst.c treestats.c
TEST    = tests/test_bst.c
BIN     = treestats
TESTBIN = test_bst

.PHONY: all test memcheck clean bonus

all: $(BIN)

$(BIN): $(SRC) bst.h
	$(CC) $(CFLAGS) -o $@ $(SRC)

$(TESTBIN): $(TEST) bst.c bst.h
	$(CC) $(CFLAGS) -o $@ $(TEST) bst.c

test: $(TESTBIN)
	./$(TESTBIN)

memcheck:
	$(CC) $(CFLAGS) $(SANFLAGS) -o $(BIN) $(SRC)
	$(CC) $(CFLAGS) $(SANFLAGS) -o $(TESTBIN) $(TEST) bst.c
	./$(TESTBIN)

clean:
	rm -f $(BIN) $(TESTBIN) test_remove