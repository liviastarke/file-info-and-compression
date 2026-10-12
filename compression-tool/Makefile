OBJ = main.o tree.o pq.o table.o bitstream.o
CFLAGS = -g

huffman-compression : $(OBJ)
	cc $(OBJ) -o huffman-compression

# individual targets for testing modules
# remember to create a main function in these beforehand
pq        : pq.o
tree      : tree.o pq.o
table     : table.o
bitstream : bitstream.o

# Define as dependências de cada arquivo objeto (.o),
# permitindo que o Make identifique quando é necessário recompilar
# um módulo após alterações nos arquivos fonte (.c) ou cabeçalhos (.h).

# create objects
# reference https://www.gnu.org/software/make/manual/make.html#make-Deduces
main.o      : tree.h pq.h table.h bitstream.h common.h check_pointer.h
table.o     : table.h common.h
tree.o      : tree.h pq.h common.h check_pointer.h
pq.o        : pq.h common.h
bitstream.o : bitstream.h common.h

# clean compilation and objects
clean:
	rm -f huffman-compression tree pq table bitstream $(OBJ)


