all: escapeless

LIB_HEADER = escapeless.h
LIB_SRC = escapeless.c

TESTER_SRC = tester.c
CLI_SRC = cli.c

TESTS = ../tests
TESTER = ./tester

CC_OPTS = -std=c90 -W -Wall -Werror -pedantic -g -O2

escapeless: $(CLI_SRC) $(LIB_SRC) $(LIB_HEADER)
	cc $(CC_OPTS) -o escapeless $(CLI_SRC) $(LIB_SRC)

tester: $(TESTER_SRC) $(LIB_SRC) $(LIB_HEADER)
	cc $(CC_OPTS) -o tester $(TESTER_SRC) $(LIB_SRC)

clean:
	-rm escapeless

include $(TESTS)/Makefile.inc
