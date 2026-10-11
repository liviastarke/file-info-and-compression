# Just another command line file compression tool

## Huffman compression

Modular implementation divided in:

- bitstream: handles file operations, abstracting the logic of writing variable-length huffman codes to the output file.

- pq: auxilary priority queue file structure to arrange huffman nodes in order for huffman tree construction.

- tree: creates huffman nodes and arranges them into a huffman tree based on character frequency counts. *Depends on: pq, char frequency counts from bitstream.*

- table: handles huffman encoding and decoding by constructing a table relating characters and their huffman codes. *Depends on: tree.*

- LZ encoder: *To be implemented*

- main file: makes good use of the functions from the modules, integrating them in a single not-so-readable file compression program. *Depends on: all modules.*


## To do:

Priorities:

- [ ] Implement file compression/decompression option (use arguments or file extension to decide on compression or decompression).

- [ ] Decide on internal structure of compressed file, header size and information, etc, in order to be able to reconstruct the huffman tree from the compressed file.

Unsatisfied requirements:

- [ ] LZ-like dictionary-based encoding before huffman compression as a separate module.

Improvements:

- [ ] Improve file handling:

    - [ ] Verify input file's path and ensure safe file operations.

    - [ ] Open/Create output file with a path similar to the input file.


