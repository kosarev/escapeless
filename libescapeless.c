
#include <assert.h>

#include "libescapeless.h"

void escapeless_encode(unsigned char *takeouts_map,
                       const unsigned char *takeouts, unsigned takeouts_size,
                       unsigned char *block, unsigned block_size) {
    /* Initially map[v] != v for any byte value v. */
    unsigned char map[0x100] = { 0x01 };
    unsigned i, k, v;

    assert(takeouts_size < 0x100 - 1);
    assert(block_size <= 0x100 - 1 - takeouts_size);

    /* Start with takeouts mapped to non-themselves, so
       takeouts_map[k] != takeouts[k] for them. */
    for(k = 0; k < takeouts_size; k++)
        takeouts_map[k] = takeouts[k] ^ 1;

    /* Mark values used in the block with map[v] == v. */
    for(i = 0; i < block_size; i++)
        map[ block[i] ] = block[i];

    /* Similarly, mark unused takeouts with map[v] == v and map
       them to themselves, so takeouts_map[k] == takeouts[k]. */
    for(k = 0; k < takeouts_size; k++) {
        v = takeouts[k];
        if(map[v] != v) {
            map[v] = (unsigned char) v;
            takeouts_map[k] = (unsigned char) v;
        }
    }

    /* Map used takeouts to unmarked values. */
    v = 0;
    k = 0;
    for(;;) {
        assert(v < 0x100);
        if(map[v] == v) {
            v++;
            continue;
        }

        if(k < takeouts_size) {
            if(takeouts_map[k] != takeouts[k])
                takeouts_map[k] = (unsigned char) v++;
            k++;
            continue;
        }

        break;
    }

    /* Map all unused takeouts to a (or the) remaining unmarked
       value. */
    for(k = 0; k < takeouts_size; k++) {
        if(takeouts_map[k] == takeouts[k])
            takeouts_map[k] = (unsigned char) v;
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
