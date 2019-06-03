# escapeless
Efficient binary encoding for large alphabets.

### Features

* Low fixed-size overhead.
* Compression-friendly output.
* Arbitrary alphabets.
* Fast and simple algorithm.
* Does not involve heavy-weight arithmetic.


### Comparison chart

| Encoding         | Alphabet Size | Overhead |
| ---------------- | ------------- | -------- |
| escapeless255    | 255           |     0.4% |
| escapeless254    | 254           |     0.8% |
| escapeless253    | 253           |     1.2% |
| [yEnc](http://www.yenc.org/yenc-draft.1.3.txt)          | 252 | 1.6%*, 0-100% |
| escapeless252    | 252           |     1.6% |
| escapeless251    | 251           |     2.0% |
| escapeless250    | 250           |     2.4% |
| escapeless240    | 240           |     6.7% |
| escapeless230    | 230           |    11.4% |
| escapeless225    | 225           |    13.8% |
| [Base122](http://blog.kevinalbs.com/base122)            | 122 | 14.3% |
| [baseE91](http://base91.sourceforge.net/)               |  91 | 22%*, 14-23% |
| [Ascii85](https://en.wikipedia.org/wiki/Ascii85)        |  85 | 25.0% |
| [Z85](https://rfc.zeromq.org/spec:32/Z85/)              |  85 | 25.0% |
| [Base64](https://en.wikipedia.org/wiki/Base16)          |  64 | 33.3% |
| [uuencode](https://en.wikipedia.org/wiki/Uuencoding)    |  64 | 33.3% |
| [Base58](https://en.wikipedia.org/wiki/Base58)          |  58 | 36.6% |
| [Base36 / 64-bit](https://en.wikipedia.org/wiki/Base36) |  36 | 59.2%*, 0-62.5% |
| [Base32](https://en.wikipedia.org/wiki/Base32)          |  32 | 60.0% |
| [Base36 / 32-bit](https://en.wikipedia.org/wiki/Base36) |  36 | 62.0%*, 0-75% |
| [Base16](https://en.wikipedia.org/wiki/Base16)          |  16 | 100.0% |

(*) On uniform distribution of input octets.


### Basic idea

Given a source alphabet of size S and a target alphabet of size
N < S, break the sequence of input characters into blocks so that
the number of characters in each block does not exceed N − 1.

Since a block can contain at most N − 1 different characters and
the target alphabet contains N characters, it is known that all
those used characters can be mapped to the target alphabet and at
least one extra character of the target alphabet will remain
unmapped.
For example:
```
 A B C D E F G H I J K L    12  Characters of the source alphabet (S)
 A   C D E     H I   K L     8  Characters of the target alphabet (N)
   x       x x     x         4  Characters missing in the target alphabet (takeouts)
   | | | |     | | |         7  Characters used in the block
 .         . .       . .     5  Characters not used in the block
```

Here, one possible mapping is:
```
 B −> A
 J −> K
```
with `L` left unmapped and all other characters of the target
alphabet mapped to themselves.

What that unmapped character is for, is to make it possible to
map unused takeouts, like `F` and `G` in the example, to a
character of the target alphabet that does not represent any
characters of the source alphabet for that block.
Taking that into account, here's how a complete mapping would
look:
```
 B −> A
 F -> L
 G -> L
 J −> K
```

Once the mapping is determined, we can output the encoded block
with takeout characters in it replaced with members of the target
alphabet.
To let a decoder know the mapping, we also have to prepend each
of the encoded blocks with a series of characters the takeouts
are mapped to and assume that the decoder will be given the same
set of takeout characters specified in the same order.


### Overhead formula

For a source alphabet of size S, a target alphabet of size N and
a block of N − 1 characters, the size of the encoded block is:
```
 encoded_block_size = takeouts_map_size + block_size =
                      (S − N) + (N - 1) =
                      S - 1
```

The overhead is thus:
```
 overhead = (encoded_block_size - block_size) / block_size =
            ((S - 1) - (N - 1)) / (N - 1) =
            (S - 1 - N + 1) / (N - 1) =
            (S - N) / (N - 1)
```


### Encoding algorithm

1. Break the input message into blocks so that no block contains
   more than N - 1 characters, where N is the size of the target
   alphabet.
   Process every block seprately as specified below.

2. Map every takeout character to a character of the target
   alphabet that is not used in the block and is not a takeout
   character.
   All takeouts not used in the block shall map to the same
   character.

3. Replace takeout characters of the block using that map.

4. Output the map followed by the rewritten block.
