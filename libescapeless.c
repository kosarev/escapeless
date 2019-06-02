
#include <assert.h>

#include "libescapeless.h"

void escapeless_encode(unsigned char *takeouts_map,
                       const unsigned char *takeouts, unsigned takeouts_size,
                       unsigned char *block, unsigned block_size) {
    unsigned char map[0x100] = { 0x01 };
    unsigned i, k;

    assert(takeouts_size < 0x100);
    assert(block_size <= 0x100 - takeouts_size);

    for(i = 0; i != block_size; ++i)
        map[ block[i] ] = block[i];

    k = 0;
    for(i = 0; i != 0x100 && k < takeouts_size; ++i) {
        if(map[i] != i) {
            map[ takeouts[k] ] = (unsigned char) i;
            takeouts_map[k++] = (unsigned char) i;
        }
    }

    for(i = 0; i != block_size; ++i)
        block[i] = map[ block[i] ];
}

void escapeless_decode(const unsigned char *takeouts_map,
                       const unsigned char *takeouts, unsigned takeouts_size,
                       unsigned char *block, unsigned block_size) {
    unsigned char map[0x100];
    unsigned i, k;

    for(i = 0; i != 0x100; ++i)
        map[i] = (unsigned char) i;

    for(k = 0; k != takeouts_size; ++k)
        map[ takeouts_map[k] ] = takeouts[k];

    for(i = 0; i != block_size; ++i)
        block[i] = map[ block[i] ];
}
