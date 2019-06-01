
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "escapeless.h"

static void verror(const char *format, va_list args) {
    fprintf(stderr, "error: ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}

static void error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    verror(format, args);
    va_end(args);
}

static int read_byte(void) {
    int c = getchar();
    if(c == EOF && ferror(stdin))
        error("cannot read input data: %s", strerror(errno));
    return c;
}

static void write_byte(int c) {
    if(putchar(c) == EOF)
        error("cannot write output data: %s", strerror(errno));
}

static void encode(const unsigned char *takeouts, unsigned takeouts_size) {
    unsigned char block[0x100], takeouts_map[0x100];
    unsigned max_block_size, block_size;
    unsigned i;
    int c;

    /* TODO: Validate takeouts size. */

    max_block_size = 0x100 - takeouts_size;

    for(;;) {
        /* Read subsequent block of data. */
        block_size = 0;
        while(block_size < max_block_size && (c = read_byte()) != EOF)
            block[block_size++] = (unsigned char) c;

        if(block_size == 0)
            break;

        /* Encode that block. */
        escapeless_encode(takeouts_map, takeouts, takeouts_size,
                          block, block_size);

        /* Write the substitutes for the takeout bytes. */
        for(i = 0; i != takeouts_size; ++i)
            write_byte(takeouts_map[i]);

        /* Write the block with replaced takeout bytes. */
        for(i = 0; i != block_size; ++i)
            write_byte(block[i]);
    }
}

static void decode(const unsigned char *takeouts, unsigned takeouts_size) {
    unsigned char block[0x100], takeouts_map[0x100];
    unsigned max_block_size, block_size;
    unsigned i;
    int c;

    /* TODO: Validate takeouts size. */

    max_block_size = 0x100 - takeouts_size;

    for(;;) {
        /* Read the substitutes for the takeout bytes. */
        for(i = 0; i < takeouts_size; i++) {
            if((c = read_byte()) == EOF)
                break;
            takeouts_map[i] = (unsigned char) c;
        }

        /* See of we reached the end of file. */
        if(i == 0)
            break;

        if(i < takeouts_size)
            error("unexpected end of file");

        /* Read the payload part of the block. */
        block_size = 0;
        while(block_size < max_block_size && (c = getchar()) != EOF)
            block[block_size++] = (unsigned char) c;

        /* Empty block should have never been encoded. */
        if(block_size == 0)
            error("empty block in input");

        /* Decode the block. */
        escapeless_decode(takeouts_map, takeouts, takeouts_size,
                          block, block_size);

        /* Write decoded block. */
        for(i = 0; i < block_size; i++)
            write_byte(block[i]);
    }
}

static void usage(void) {
    fprintf(stderr, "usage: tester {encode|decode} <takeouts>\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    const char *command, *takeouts;
    size_t takeouts_size;

    if(argc != 3)
        usage();

    takeouts = argv[2];
    takeouts_size = strlen(takeouts);
    if(takeouts_size >= 0x100)
        error("too many takeout bytes");

    command = argv[1];
    if(strcmp(command, "encode") == 0) {
        encode((const unsigned char*) takeouts, (unsigned) takeouts_size);
    } else if(strcmp(command, "decode") == 0) {
        decode((const unsigned char*) takeouts, (unsigned) takeouts_size);
    } else {
        error("unknown command: %s", command);
    }

    return EXIT_SUCCESS;
}
