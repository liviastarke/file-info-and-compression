OBJ = main.o tree.o pq.o table.o bitstream.o
CFLAGS = -g

huffman-compression : $(OBJ)
	cc $(OBJ) -o huffman-compression

# individual targets for testing modules
pq        : pq.o
table     : table.o
bitstream : bitstream.o

# create objects
main.o      : tree.h pq.h table.h bitstream.h common.h
table.o     : table.h common.h
tree.o      : tree.h common.h
pq.o        : pq.h common.h
bitstream.o : bitstream.h common.h

# clean compilation and objects
clean :
	rm huffman-compression pq table bitstream $(OBJ)


