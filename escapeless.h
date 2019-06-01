
#ifndef ESCAPELESS_H
#define ESCAPELESS_H

void escapeless_encode(unsigned char *takeouts_map,
                       const unsigned char *takeouts, unsigned takeouts_size,
                       unsigned char *block, unsigned block_size);

void escapeless_decode(const unsigned char *takeouts_map,
                       const unsigned char *takeouts, unsigned takeouts_size,
                       unsigned char *block, unsigned block_size);

#endif  /* ESCAPELESS_H */
