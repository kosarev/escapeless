
/*  Efficient binary encoding for large alphabets.
    https://github.com/kosarev/escapeless

    Copyright (C) 2019 Ivan Kosarev.
    ivan@kosarev.info

    Published under the MIT license.
*/

#ifndef LIBESCAPELESS_H
#define LIBESCAPELESS_H

namespace escapeless {

void encode(unsigned char *takeouts_map,
            const unsigned char *takeouts, unsigned takeouts_size,
            unsigned char *block, unsigned block_size);

void decode(const unsigned char *takeouts_map,
            const unsigned char *takeouts, unsigned takeouts_size,
            unsigned char *block, unsigned block_size);

}  // namespace escapeless

#endif  /* LIBESCAPELESS_H */
