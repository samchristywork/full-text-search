CC := gcc

all: build/fts

build/fts: src/fts.c
	mkdir -p build/
	${CC} src/fts.c -o build/fts

valgrind: build/fts
	valgrind --track-origins=yes -s ./build/fts ./test_data/colors.csv

rlwrap: build/fts
	rlwrap ./build/fts ./test_data/colors.csv

clean:
	rm -rf build/
