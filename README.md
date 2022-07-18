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

## Usage

Simply invoke the program with a filename as an argument. Large files may take a
moment to load. The program will print out the string "READY" when it has
finished tokenizing the file. Then, you can just type the term you want to find
and hit "Enter". A list of all of the hits will be printed out, along with the
amount of time the search query took.

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
