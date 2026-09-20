OBJ = main.o tree.o pq.o table.o bitstream.o
CFLAGS = -g

huffman-compression : $(OBJ)
	cc $(OBJ) -o huffman-compression

table : table.o

main.o      : tree.h pq.h table.h bitstream.h common.h
table.o     : table.h common.h
tree.o      : tree.h common.h
pq.o        : pq.h common.h
bitstream.o : bitstream.h common.h

clean :
	rm huffman-compression $(OBJ)


