all: escapeless

HEADERS = \
    libescapeless.h

SRC = \
    libescapeless.c \
    escapeless.c

escapeless: $(SRC) $(HEADERS)
	cc -g -O2 -o escapeless $(SRC)

test:
	./escapeless

clean:
	-rm escapeless
