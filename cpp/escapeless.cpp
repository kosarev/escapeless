
/*  Efficient binary encoding for large alphabets.
    https://github.com/kosarev/escapeless

    Copyright (C) 2019 Ivan Kosarev.
    ivan@kosarev.info

    Published under the MIT license.
*/

#include <cassert>

#include "escapeless.h"

namespace escapeless {

void encode(unsigned char *takeouts_map,
            const unsigned char *takeouts, unsigned takeouts_size,
            unsigned char *block, unsigned block_size) {
    assert(takeouts_size < 0x100 - 1);
    assert(block_size <= 0x100 - 1 - takeouts_size);

    /* Initially map[c] != c for any character c. */
    unsigned char map[0x100] = { 0x01 };

    /* Start with takeouts mapped to non-themselves, so
       takeouts_map[k] != takeouts[k] for them. */
    for(unsigned k = 0; k != takeouts_size; ++k)
        takeouts_map[k] = takeouts[k] ^ 1;

    /* Mark characters used in the block with map[c] == c. */
    for(unsigned i = 0; i != block_size; ++i)
        map[ block[i] ] = block[i];

    /* Similarly, mark unused takeouts with map[c] == c and map
       them to themselves, so takeouts_map[k] == takeouts[k]. */
    for(unsigned k = 0; k != takeouts_size; ++k) {
        unsigned c = takeouts[k];
        if(map[c] != c) {
            map[c] = static_cast<unsigned char>(c);
            takeouts_map[k] = static_cast<unsigned char>(c);
        }
    }

    /* Map used takeouts to unmarked values. */
    unsigned c = 0;
    unsigned k = 0;
    for(;;) {
        assert(c < 0x100);
        if(map[c] == c) {
            ++c;
            continue;
        }

        if(k < takeouts_size) {
            if(takeouts_map[k] != takeouts[k])
                takeouts_map[k] = static_cast<unsigned char>(c++);
            ++k;
            continue;
        }

        break;
    }

    /* Map all unused takeouts to a (or the) remaining unmarked
       value. */
    for(unsigned k = 0; k != takeouts_size; ++k) {
        if(takeouts_map[k] == takeouts[k])
            takeouts_map[k] = static_cast<unsigned char>(c);
    }

    /* Propagate the takeouts map to the byte values map. */
    for(unsigned k = 0; k != takeouts_size; ++k)
        map[ takeouts[k] ] = takeouts_map[k];

    /* Replace used takeouts. */
    for(unsigned i = 0; i != block_size; ++i)
        block[i] = map[ block[i] ];
}

void decode(const unsigned char *takeouts_map,
            const unsigned char *takeouts, unsigned takeouts_size,
            unsigned char *block, unsigned block_size) {
    assert(takeouts_size < 0x100 - 1);
    assert(block_size <= 0x100 - 1 - takeouts_size);

    unsigned char map[0x100];
    for(unsigned i = 0; i != 0x100; ++i)
        map[i] = static_cast<unsigned char>(i);

    /* Map takeouts to their original values. */
    for(unsigned k = 0; k != takeouts_size; ++k)
        map[ takeouts_map[k] ] = takeouts[k];

    /* Restore takeouts in the block. */
    for(unsigned i = 0; i != block_size; ++i)
        block[i] = map[ block[i] ];
}

}  // namespace escapeless
