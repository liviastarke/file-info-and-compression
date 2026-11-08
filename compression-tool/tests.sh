#!/bin/bash
# 
# Executes compression and decompression for all files in the texts folder and saves the output in output.txt file
#
LANG=C
for file in ./test-files/*
do
	time ./compression-tool -c "$file"
	du --bytes --apparent-size "$file" | awk '{print $1}'
	du --bytes --apparent-size "test_out_file.olv" | awk '{print $1}'
	time ./compression-tool -d "test_out_file.olv"
	diff -s "$file" "test_reconstruct_in_file"
done >> script_results.txt
