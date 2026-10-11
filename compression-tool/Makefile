OBJ = main.o tree.o pq.o table.o bitstream.o
CFLAGS = -g

huffman-compression : $(OBJ)
	cc $(OBJ) -o huffman-compression

# individual targets for testing modules
pq        : pq.o
tree      : tree.o pq.o
table     : table.o
bitstream : bitstream.o

# create objects
#main.o      : tree.h pq.h table.h bitstream.h common.h
#table.o     : table.h common.h
#tree.o      : tree.h pq.h common.h
#pq.o        : pq.h common.h
#bitstream.o : bitstream.h common.h

# Define as dependências de cada arquivo objeto (.o),
# permitindo que o Make identifique quando é necessário recompilar
# um módulo após alterações nos arquivos fonte (.c) ou cabeçalhos (.h).

# create objects
main.o      : main.c tree.h pq.h table.h bitstream.h common.h check_pointer.h
table.o     : table.c table.h common.h
tree.o      : tree.c tree.h pq.h common.h check_pointer.h
pq.o        : pq.c pq.h common.h
bitstream.o : bitstream.c bitstream.h common.h

# clean compilation and objects
clean:
	rm -f huffman-compression tree pq table bitstream $(OBJ)


