# CS50 TSE Querier
## Design Spec

According to the [Querier Requirements Spec](REQUIREMENTS.md), The third component of the Tiny Search Engine is the Querier, which reads the index produced by the Indexer and the page files produced by the Crawler, to interactively answer written queries entered by the user.

### User interface

The querier is intialized from the command-line; it must always have two arguments.

```
./querier pageDirectory indexFilename
```

For example, if `crawled` is a pageDirectory in `../data`, and letters.ndx is the corresponding index

``` bash
$ indexer ../data/crawled ../data/letters.ndx
```

At this point, the user will be given a prompt, from which to execute their queries
```
query? <user-input>
```

### Inputs and outputs

**Input**: The querier reads the index file to create an index of words contained in the pages of the page directory in order to query.

The querier also takes input from stdin as arguments for the query.

**Output**: query results are sent to stdout, with any errors being sent to stderr.

### Functional decomposition into modules

The following main functions are used:

 1. *main*, which parses arguments and initializes other modules;
 2. *parseInput*, which builds an array of words from the query to use for searching;
 2. *runQuery*, which uses the query to calculate query scores.

And some helper modules that provide data structures:

 1. *index*, a module providing the data structure to represent the in-memory index, and functions to read and write index files;
 2. *dual_holder*, a module allowing for two counter sets to be passed as one argument in a function;
 3. *sort_holder*, a module allowing for multiple integer arrays to be passed as one argument in a fcuntion;
 4. *word*, a module providing a function to normalize a word.

### Pseudo code for logic/algorithmic flow

The querier will run as follows:

    parse the command line, validate parameters, initialize other modules
    load the index from the index file
    take line from stdin as query
    if the query is not an EOF:
        build the structure for parsing the query
        use runQuery to get scores for the query
        sort the query results and print

Where runQuery:

    creates score counters for each word
    intersects scores for each "and"
    merges scores for each "or"

### Major data structures

The key data structures are the *index*, mapping from *word* to *(docID, #occurrences)* pairs and the *counters* which keep query scores
The *index* is a *hashtable* keyed by *word* and storing *counters* as items.
The *counters* is keyed by *docID* and stores a count of the number of occurrences of queried words. 

### Testing plan

*Unit testing*.  A program *test.sh* will serve as a unit test for the *querier* module; it reads an index file into the internal, take input from stdin, then writes the stdout and stderr to a file.

1. Test `querier` with various invalid arguments.
	2. no arguments
	3. one argument
	4. three or more arguments
	5. invalid `pageDirectory` (non-existent path)
	6. invalid `indexFile` (non-existent path)
    7. various stdin inputs
