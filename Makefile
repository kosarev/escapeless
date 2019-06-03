all: escapeless

test: test_moby test_all_bytes test_one_missed

HEADERS = \
    libescapeless.h

SRC = \
    libescapeless.c \
    escapeless.c

escapeless: $(SRC) $(HEADERS)
	cc -g -O2 -o escapeless $(SRC)

test_moby:
	./escapeless encode aef <tests/moby.txt | diff - tests/moby.txt.encoded
	./escapeless decode aef <tests/moby.txt.encoded | diff - tests/moby.txt

test_all_bytes:
	./escapeless encode B <tests/all_bytes.bin | diff - tests/all_bytes.bin.encoded
	./escapeless decode B <tests/all_bytes.bin.encoded | diff - tests/all_bytes.bin

test_one_missed:
	./escapeless encode AB <tests/one_missed.bin | diff - tests/one_missed.bin.encoded
	./escapeless decode AB <tests/one_missed.bin.encoded | diff - tests/one_missed.bin

clean:
	-rm escapeless
