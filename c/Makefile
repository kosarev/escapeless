all: escapeless

test: \
    test_all_bytes \
    test_moby \
    test_one_missed \
    test_other_unused_takeout \
    test_unused_takeout

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

test_moby: tester
	./escapeless encode aef <$(TESTS)/moby.txt | diff - $(TESTS)/moby.txt.encoded
	./escapeless decode aef <$(TESTS)/moby.txt.encoded | diff - $(TESTS)/moby.txt

test_all_bytes: tester
	./tester <$(TESTS)/all_bytes.test

test_unused_takeout: tester
	./tester <$(TESTS)/unused_takeout.test

test_other_unused_takeout: tester
	./tester <$(TESTS)/other_unused_takeout.test

test_one_missed: tester
	./escapeless encode AB <$(TESTS)/one_missed.bin | diff - $(TESTS)/one_missed.bin.encoded
	./escapeless decode AB <$(TESTS)/one_missed.bin.encoded | diff - $(TESTS)/one_missed.bin

clean:
	-rm escapeless
