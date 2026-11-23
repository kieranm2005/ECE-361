CC       = cc
CFLAGS   = -std=c11 -Wall -Wextra -Wpedantic -g
SANFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer

BIN       = treestats
TESTBIN   = test_bst
REMTEST   = test_remove

SRC       = bst.c treestats.c
TEST      = tests/test_bst.c
RMTEST    = tests/test_remove.c

.PHONY: all test memcheck bonus clean

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

$(REMTEST): $(RMTEST) bst.c bst.h
	$(CC) $(CFLAGS) $(SANFLAGS) -o $@ $(RMTEST) bst.c

bonus: $(REMTEST)
	./$(REMTEST)

clean:
	rm -f $(BIN) $(TESTBIN) $(REMTEST)