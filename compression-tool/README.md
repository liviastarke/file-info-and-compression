# Just another command line file compression tool

## Huffman compression

Modular implementation divided in:

- bitstream: handles file operations, abstracting the logic of writing variable-length huffman codes to the output file.

- pq: auxilary priority queue file structure to arrange huffman nodes in order for huffman tree construction.

- tree: creates huffman nodes and arranges them into a huffman tree based on character frequency counts. *Depends on: pq, char frequency counts from bitstream.*

- table: handles huffman encoding and decoding by constructing a table relating characters and their huffman codes. *Depends on: tree.*

## To do:

- [ ] Integration of the modules in *main.c*.

    - [ ] Finish character encoding in *main.c* by accessing the table and calling bw\_write\_bits.

    - [ ] Verify input file's path and ensure safe file operations.

    - [ ] Open/Create output file with a path similar to the input file.

    - [ ] Decide on internal structure of compressed file, header size and information, etc.

- [ ] LZ-like dictionary-based encoding before huffman compression.


