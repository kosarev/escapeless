
/*  Efficient binary encoding for large alphabets.
    https://github.com/kosarev/escapeless

    Copyright (C) 2019 Ivan Kosarev.
    ivan@kosarev.info

    Published under the MIT license.
*/

#ifndef LIBESCAPELESS_H
#define LIBESCAPELESS_H

void escapeless_encode(unsigned char *takeouts_map,
                       const unsigned char *takeouts, unsigned takeouts_size,
                       unsigned char *block, unsigned block_size);

void escapeless_decode(const unsigned char *takeouts_map,
                       const unsigned char *takeouts, unsigned takeouts_size,
                       unsigned char *block, unsigned block_size);

#endif  /* LIBESCAPELESS_H */
