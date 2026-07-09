#!/bin/bash
# 
# Executes huffman-compression for all files in the texts folder and saves the output in output.txt file
# Executa huffman-compression para todos os arquivos no diretório texts e salva a saída do programa no arquivo output.txt
#
LANG=C
for file in ./texts/*
do
	./huffman-compression "$file"
done | (
OUTPUT=$(grep --text "RESULTADOS" | tee output.txt | awk '/RESULTADOS.*%/{ gsub(/%/,"");print($7)}' | paste -s -d+)
NOL=$(echo $OUTPUT | tr -dc '+' | wc -c)
MEDIA=$(echo  "( $OUTPUT ) / ($NOL + 1)"  | bc -l | numfmt --format '%.2f')
echo "A média da taxa de compressão é ${MEDIA}%" >> output.txt 
)
