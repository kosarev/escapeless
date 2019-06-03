
#include <assert.h>

#include "libescapeless.h"

void escapeless_encode(unsigned char *takeouts_map,
                       const unsigned char *takeouts, unsigned takeouts_size,
                       unsigned char *block, unsigned block_size) {
    /* Initially map[c] != c for any character c. */
    unsigned char map[0x100] = { 0x01 };
    unsigned i, k, c;

    assert(takeouts_size < 0x100 - 1);
    assert(block_size <= 0x100 - 1 - takeouts_size);

    /* Start with takeouts mapped to non-themselves, so
       takeouts_map[k] != takeouts[k] for them. */
    for(k = 0; k < takeouts_size; k++)
        takeouts_map[k] = takeouts[k] ^ 1;

    /* Mark characters used in the block with map[c] == c. */
    for(i = 0; i < block_size; i++)
        map[ block[i] ] = block[i];

    /* Similarly, mark unused takeouts with map[c] == c and map
       them to themselves, so takeouts_map[k] == takeouts[k]. */
    for(k = 0; k < takeouts_size; k++) {
        c = takeouts[k];
        if(map[c] != c) {
            map[c] = (unsigned char) c;
            takeouts_map[k] = (unsigned char) c;
        }
    }

    /* Map used takeouts to unmarked values. */
    c = 0;
    k = 0;
    for(;;) {
        assert(c < 0x100);
        if(map[c] == c) {
            c++;
            continue;
        }

        if(k < takeouts_size) {
            if(takeouts_map[k] != takeouts[k])
                takeouts_map[k] = (unsigned char) c++;
            k++;
            continue;
        }

        break;
    }

    /* Map all unused takeouts to a (or the) remaining unmarked
       value. */
    for(k = 0; k < takeouts_size; k++) {
        if(takeouts_map[k] == takeouts[k])
            takeouts_map[k] = (unsigned char) c;
    }

    /* Propagate the takeouts map to the byte values map. */
    for(k = 0; k < takeouts_size; k++)
        map[ takeouts[k] ] = takeouts_map[k];

    /* Replace used takeouts. */
    for(i = 0; i != block_size; i++)
        block[i] = map[ block[i] ];
}

void escapeless_decode(const unsigned char *takeouts_map,
                       const unsigned char *takeouts, unsigned takeouts_size,
                       unsigned char *block, unsigned block_size) {
    unsigned char map[0x100];
    unsigned i, k;

    assert(takeouts_size < 0x100 - 1);
    assert(block_size <= 0x100 - 1 - takeouts_size);

    for(i = 0; i != 0x100; ++i)
        map[i] = (unsigned char) i;

    /* Map takeouts to their original values. */
    for(k = 0; k != takeouts_size; ++k)
        map[ takeouts_map[k] ] = takeouts[k];

    /* Restore takeouts in the block. */
    for(i = 0; i != block_size; ++i)
        block[i] = map[ block[i] ];
}
