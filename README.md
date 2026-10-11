# Just some C file handling experiments

## File info retrieval

*bitmaphandler.c* and *pnghandler.c* are simple programs that read, respectively, bitmap headers and png chunks 
into structs and display some basic info in a human-readable format.

## Compression algorithms

*huffman-compression.c* applies Huffman Coding to text files and prints the codes and the binary compressed text 
in a string of zeros and ones. It is not supposed to actually write a compressed file, although I tried to do 
that. It was never supposed to be "scalable" (nor modified in any way). I wrote it as an in-class exercise. So I 
decided it would be easier to write a new program from scratch.

In *compression-tool* we (3 contributors) are developing a command line file compression tool, based on the huffman algorithm and 
LZ (in progress). Read *compression-tool/README.md* for details.

## Contribute!

If you are willing to explore file headers and metadata or practice C, you can make your own changes and help expand 
this code's functionality.

