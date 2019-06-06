all: tester

LIB_HEADER = escapeless.h
LIB_SRC = escapeless.cpp

TESTER_SRC = tester.cpp

TESTS = ../tests
TESTER = ./tester

CC_OPTS = -std=c++11 -W -Wall -Werror -pedantic -g -O2

tester: $(TESTER_SRC) $(LIB_SRC) $(LIB_HEADER)
	c++ $(CC_OPTS) -o tester $(TESTER_SRC) $(LIB_SRC)

clean:
	-rm -f tester

include $(TESTS)/Makefile.inc
