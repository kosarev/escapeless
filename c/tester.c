
/*  Efficient binary encoding for large alphabets.
    https://github.com/kosarev/escapeless

    Copyright (C) 2019 Ivan Kosarev.
    ivan@kosarev.info

    Published under the MIT license.
*/

#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "escapeless.h"

#define MAX_BYTE_VECTOR_SIZE 0x100
#define MAX_LINE_SIZE 512

struct byte_vector {
    unsigned size;
    unsigned char bytes[MAX_BYTE_VECTOR_SIZE];
};

static void verror(const char *format, va_list args) {
    fprintf(stderr, "tester: ");
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

static int parse_hex_digit(unsigned *b, const char **p) {
    if(**p >= '0' && **p <= '9') {
        *b = **p - '0';
        (*p)++;
        return 1;
    }

    if(**p >= 'a' && **p <= 'f') {
        *b = **p - 'a' + 10;
        (*p)++;
        return 1;
    }

    return 0;
}

static int parse_byte(unsigned *b, const char **p) {
    unsigned high, low;
    if(!parse_hex_digit(&high, p))
        return 0;

    if(!parse_hex_digit(&low, p))
        error("invalid hex digit");

    *b = (high << 4) | low;
    return 1;
}

static int parse_token(const char *t, const char **p) {
    size_t len = strlen(t);
    if(strncmp(*p, t, len) != 0)
        return 0;

    *p += len;
    return 1;
}

static void append_byte(struct byte_vector *v, unsigned b) {
    if(v->size >= MAX_BYTE_VECTOR_SIZE)
        error("too many bytes specified");

    v->bytes[v->size++] = b;
}

static void append_vector(struct byte_vector *a,
                          const struct byte_vector *b) {
    unsigned i;
    for(i = 0; i < b->size; i++)
        append_byte(a, b->bytes[i]);
}

static int match_vectors(const struct byte_vector *a,
                         const struct byte_vector *b) {
    return a->size == b->size &&
           memcmp(a->bytes, b->bytes, a->size) == 0;
}

static void append_bytes_up_to(struct byte_vector *v, unsigned b) {
    unsigned i;

    if(v->size == 0)
        error("no byte before '..'");

    i = v->bytes[v->size - 1];
    do {
        i = (i + 1) % 0x100;
        append_byte(v, i);
    } while(i != b);
}

static void split_vectors(struct byte_vector *l, struct byte_vector *r,
                          const struct byte_vector *s, unsigned size) {
    assert(size <= s->size);

    *l = *s;
    l->size = size;

    r->size = s->size - size;
    memcpy(r->bytes, s->bytes + size, r->size);
}

static int read_line(char *line) {
    size_t len;

    if(fgets(line, MAX_LINE_SIZE, stdin) == NULL) {
        if(ferror(stdin))
            error("cannot read input: %s", strerror(errno));
        return 0;
    }

    /* Strip the trailing new-line character. */
    len = strlen(line);
    if(len > 0) {
        assert(line[len - 1] == '\n');
        line[len - 1] = '\0';
    }
    return 1;
}

static void read_byte_vector(struct byte_vector *v) {
    char line[MAX_LINE_SIZE];
    const char *p;
    unsigned b;

    /* Skip empty lines and comments. */
    for(;;) {
        if(!read_line(line))
            error("unexpected end of file");

        if(line[0] != '\0' && line[0] != '#')
            break;
    }

    /* Parse the line containing byte values. */
    v->size = 0;
    p = line;
    while(*p != '\0') {
        if(*p == ' ') {
            p++;
            continue;
        }

        if(parse_byte(&b, &p)) {
            append_byte(v, b);
            continue;
        }

        if(parse_token("..", &p)) {
            if(!parse_byte(&b, &p))
                error("expected byte after '..'");
            append_bytes_up_to(v, b);
            continue;
        }

        error("invalid line: %s", line);
    }
}

static void dump_byte_vector(const char *title, const struct byte_vector *v) {
    unsigned i;

    printf("%s: [", title);
    for(i = 0; i < v->size; i++) {
        if(i > 0)
            printf(" ");
        printf("%02x", v->bytes[i]);
    }
    printf("]\n");
}

static void check_end_of_file(void) {
    char line[MAX_LINE_SIZE];

    while(read_line(line)) {
        if(line[0] == '\0' || line[0] == '#')
            continue;

        error("extra line: %s", line);
    }
}

static void test_encoding(const struct byte_vector *input,
                          const struct byte_vector *takeouts,
                          const struct byte_vector *encoded) {
    struct byte_vector takeouts_map;
    struct byte_vector block;
    struct byte_vector output;

    block = *input;
    takeouts_map.size = takeouts->size;
    escapeless_encode(takeouts_map.bytes, takeouts->bytes, takeouts->size,
                      block.bytes, block.size);

    output.size = 0;
    append_vector(&output, &takeouts_map);
    append_vector(&output, &block);

    if(!match_vectors(&output, encoded)) {
        dump_byte_vector("input", input);
        dump_byte_vector("takeouts", takeouts);

        dump_byte_vector("takeouts_map", &takeouts_map);
        dump_byte_vector("block", &block);
        dump_byte_vector("output", &output);

        dump_byte_vector("encoded", encoded);

        error("encoded output doesn't match");
    }
}

static void test_decoding(const struct byte_vector *input,
                          const struct byte_vector *takeouts,
                          const struct byte_vector *encoded) {
    struct byte_vector takeouts_map;
    struct byte_vector block;

    split_vectors(&takeouts_map, &block, encoded, takeouts->size);

    escapeless_decode(takeouts_map.bytes, takeouts->bytes, takeouts->size,
                      block.bytes, block.size);

    if(!match_vectors(&block, input)) {
        dump_byte_vector("input", input);
        dump_byte_vector("takeouts", takeouts);

        dump_byte_vector("takeouts_map", &takeouts_map);
        dump_byte_vector("block", &block);

        error("decoded block doesn't match");
    }
}

static void test(const struct byte_vector *input,
                 const struct byte_vector *takeouts,
                 const struct byte_vector *encoded) {
    test_encoding(input, takeouts, encoded);
    test_decoding(input, takeouts, encoded);
}

int main(void) {
    struct byte_vector input, takeouts, encoded;

    read_byte_vector(&input);
    read_byte_vector(&takeouts);
    read_byte_vector(&encoded);
    check_end_of_file();

    test(&input, &takeouts, &encoded);

    return EXIT_SUCCESS;
}
