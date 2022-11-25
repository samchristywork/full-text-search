![Banner](https://s-christy.com/status-banner-service/full-text-search/banner-slim.svg)

## Overview

A full text search engine is a type of program that finds some kind of pattern
specified by a search query in a body of text. The naïve way to do this is to go
through every character of the text every time you get a new query string. This
works, but is far less efficient than methods that use tokenization. By running
an initial pass on the text that tokenizes the text and builds a data structure
to hold those tokens, the speed of searching for an individual token can be
greatly improved, and the speed gains become even more pronounced as the body of
text grows larger and larger. This means that tokenization is sometimes the only
viable way to search through extremely large bodies of text.

Traditional and open source tools for this include Elastisearch, Apache Lucene,
and (my favorite), Apache Solr. These tools often expose a web interface like
what you would get with a common search engine. API requests can be as simple as
using cURL with the right query parameters.

The program that I'm going to demonstrate here isn't a web server, but could
easily be turned into one, or into a server that listens on a Unix domain
socket, or just plain old stdin and stdout.

## Features

- Full text search engine written in C
- Tree-based tokenization algorithm allows lightning fast responses even for large data sets
- Case-insensitive queries
- Terminal interface
- Reports progress for large file induction
- REPL-friendly with rlwrap

## Usage

Simply invoke the program with a filename as an argument. Large files may take a
moment to load. The program will print out the string "READY" when it has
finished tokenizing the file. Then, you can just type the term you want to find
and hit "Enter". A list of all of the hits will be printed out, along with the
amount of time the search query took.

## Example

In this example, I invoke the program with the `lorem_ipsum.txt` file as an
argument. The program indexes this file which happens instantly so the program
reports that it took 0.000000 seconds. The program prints `READY` to indicate
that it has finished indexing the file. I then enter in the query "ex" which
returns three results: "ex", "excepteur", and "exercitation". For each result,
the program prints out the byte offset where the token was found, the length of
the token, and eighty characters of context with dollar signs to represent
newlines. Finally, the program prints the number of results returned and how
long it took to process the query.

```
rlwrap ./build/fts ./test_data/lorem_ipsum.txt
Initializing Full Text Search version 1.0.0
0.000000
READY
Input: ex
ex              209     2       ex ea commodo consequat.$Duis aute irure dolor in reprehenderit in voluptate vel
excepteur       344     9       Excepteur sint occaecat cupidatat non proident, sunt in$culpa qui officia deseru
exercitation    174     12      exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.$Duis aute
Found 3 results in 262.407000 microseconds.
```

## Dependencies

```
gcc
make
```

`rlwrap` and `valgrind` are optional. They are only needed for the extra targets
listed in the Makefile.

## License

This work is licensed under the GNU General Public License version 3 (GPLv3).

[<img src="https://s-christy.com/status-banner-service/GPLv3_Logo.svg" width="150" />](https://www.gnu.org/licenses/gpl-3.0.en.html)
