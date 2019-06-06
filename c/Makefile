all: escapeless

LIB_HEADER = escapeless.h
LIB_SRC = escapeless.c

TESTER_SRC = tester.c
CLI_SRC = cli.c

TESTS = ../tests

CC_OPTS = -std=c90 -W -Wall -Werror -pedantic -g -O2

escapeless: $(CLI_SRC) $(LIB_SRC) $(LIB_HEADER)
	cc $(CC_OPTS) -o escapeless $(CLI_SRC) $(LIB_SRC)

tester: $(TESTER_SRC) $(LIB_SRC) $(LIB_HEADER)
	cc $(CC_OPTS) -o tester $(TESTER_SRC) $(LIB_SRC)

test: tester
	./tester <$(TESTS)/all_bytes.test
	./tester <$(TESTS)/basic.test
	./tester <$(TESTS)/one_missed.test
	./tester <$(TESTS)/one_missed_reversed.test
	./tester <$(TESTS)/other_unused_takeout.test
	./tester <$(TESTS)/unused_takeout.test

clean:
	-rm escapeless
