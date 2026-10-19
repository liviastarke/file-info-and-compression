# Just another command line file compression tool

## Huffman compression

Modular implementation divided in:

- bitstream: handles file operations, abstracting the logic of writing variable-length Huffman codes to the 
output file.

- pq: auxilary priority queue file structure to arrange Huffman nodes in order for Huffman tree construction.

- tree: creates Huffman nodes and arranges them into a Huffman tree based on character frequency counts. 
*Depends on: pq, char frequency counts from bitstream.*

- table: handles Huffman encoding and decoding by constructing a table relating characters and their Huffman 
codes. *Depends on: tree.*

- LZ encoder: *To be implemented*

- main file: makes good use of the functions from the modules, integrating them in a single not-so-readable 
file compression program. *Depends on: all modules.*


## To do:

Priorities:

- [ ] Implement file compression/decompression option (use arguments or file extension to decide on compression 
or decompression).

- [x] Decide on internal structure of compressed file, header size and information, etc, in order to be able to 
reconstruct the Huffman tree from the compressed file.

    - [x] Implement writing the header.

    - [ ] Implement reading the header.

Unsatisfied requirements:

- [ ] LZ-like dictionary-based encoding before Huffman compression as a separate module.

Improvements:

- [ ] Improve file handling:

    - [ ] Verify input file's path and ensure safe file operations.

    - [ ] Open/Create output file with a path similar to the input file.


## A guide to reading the OLIVE header hexdump

The first 14 bytes consist of the file's magic number - which is hex for the string "OLIVE" -, the total number
of symbols to be encoded/decoded by Huffman's algorithm (*for now it is the total size of the original file, but
will change when LZ is implemented*) and the number of distinct symbols right before Huffman encoding - from
which the header length may be calculated. Therefore, the very first bytes of a hexdump should be always
`4f4c 4956 45xx xxxx xxxx xxxx xxyy`. For now, with Huffman encoding/decoding only:

1. The number of total "symbols" `xxxx xxxx xxxx xxxx` encoded in an `uint64_t` has the purpose of letting the
program differentiate Huffman encoded information in the file from possible padding bits at the end of the file.
It is effectively a way to tell the program when to stop decoding.

2. The header length information in `yy` is equal to the *number of distinct characters in the original,
uncompressed file* (***note: this is temporary, may change when LZ is implemented***), whose frequencies are
stored in the header to allow the Huffman tree to be rebuilt for decoding.

After these first, fixed-length 14 bytes follows a sequence of {character, frequency}, repeated `yy` times. Since 
each char is 1 byte and each frequency is 8 bytes, the total header length is of `14 + (9*yy)`.

#### Sample header (partial):

```
00000000: 4f4c 4956 45d9 0700 0000 0000 0049 0a3b  OLIVE........I.;
00000010: 0000 0000 0000 0020 4201 0000 0000 0000  ....... B.......
00000020: 2101 0000 0000 0000 0022 0200 0000 0000  !........"......
00000030: 0000 230f 0000 0000 0000 0028 1100 0000  ..#........(....
00000040: 0000 0000 2911 0000 0000 0000 002a 2600  ....)........*&.
00000050: 0000 0000 0000 2b03 0000 0000 0000 002c  ......+........,
00000060: 1200 0000 0000 0000 2d05 0000 0000 0000  ........-.......
00000070: 002e 1400 0000 0000 0000 3009 0000 0000  ..........0.....
00000080: 0000 0031 0900 0000 0000 0000 3205 0000  ...1........2...
00000090: 0000 0000 0033 0100 0000 0000 0000 3401  .....3........4.
000000a0: 0000 0000 0000 0035 0200 0000 0000 0000  .......5........
```

**Interpretation:**

> An ASCII representation of "." means no representation.

hex representation   | Represented value | meaning
---------------------|-------------------|---------------------------
4f4c 4956 45         | OLIVE             | File signature (magic number)
d9 0700 0000 0000 00 | 2009 bytes        | Total number of encoded symbols
49                   | 73                | Total number of frequencies in the header
0a                   | \n                | The char '\n'
3b 0000 0000 0000 00 | 59                | Frequency of the char '\n'
20                   | ' '               | The space char
4201 0000 0000 0000  | 322               | Frequency of the char ' '

