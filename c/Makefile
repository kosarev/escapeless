all: escapeless

test: test_moby test_all_bytes test_one_missed

HEADERS = \
    escapeless.h

SRC = \
    escapeless.c \
    escapeless_cli.c

TESTS = ../tests

escapeless: $(SRC) $(HEADERS)
	cc -std=c90 -W -Wall -Werror -pedantic -g -O2 -o escapeless $(SRC)

test_moby:
	./escapeless encode aef <$(TESTS)/moby.txt | diff - $(TESTS)/moby.txt.encoded
	./escapeless decode aef <$(TESTS)/moby.txt.encoded | diff - $(TESTS)/moby.txt

test_all_bytes:
	./escapeless encode B <$(TESTS)/all_bytes.bin | diff - $(TESTS)/all_bytes.bin.encoded
	./escapeless decode B <$(TESTS)/all_bytes.bin.encoded | diff - $(TESTS)/all_bytes.bin

test_one_missed:
	./escapeless encode AB <$(TESTS)/one_missed.bin | diff - $(TESTS)/one_missed.bin.encoded
	./escapeless decode AB <$(TESTS)/one_missed.bin.encoded | diff - $(TESTS)/one_missed.bin

clean:
	-rm escapeless
