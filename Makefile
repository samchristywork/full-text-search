all: build/fts

build/fts: fts.c
	mkdir -p build/
	gcc -g -Wall -Wpedantic fts.c -o build/fts

valgrind: build/fts
	valgrind --track-origins=yes -s ./build/fts ./test_data/test.csv

rlwrap: build/fts
	rlwrap ./build/fts ./test_data/test.csv

clean:
	rm -rf build/
