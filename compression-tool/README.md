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

The first 6 bytes consist of the file's magic number -- which is hex for the string "OLIVE" -- and header length 
information -- encoded in the last byte -- therefore the very first bytes of a hexdump should be `4f4c 4956 45xx`. 
The header length information in `xx` is equal to the *number of characters in the original, uncompressed file* 
(***note: this is temporary, may change when LZ is implemented***), whose frequencies are stored in the header 
to allow the Huffman tree to be rebuilt for decoding.

After these first, fixed-length 6 bytes follows a sequence of {character, frequency}, repeated `xx` times. Since 
each char is 1 byte and each frequency is 8 bytes, the total header length is of `6 + (9*xx)`.

