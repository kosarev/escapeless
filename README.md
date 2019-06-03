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
| escapeless230    | 230           |    11.3% |
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
