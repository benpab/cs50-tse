# Indexer Implementation Spec

## Data structures 

We use a data structure `index`, implemented in `index.h`, to store the information collected by `indexer`

## Control flow

The Indexer is implemented in one file `indexer.c`, with four functions.

### main

The `main` function simply validates the inputs, calls `indexBuild` and then exits zero.

### indexBuild

Given the crawler directory and a filename for the resulting index file, it constructs and index and then saves it.

* using the `pageDirectory`, constructs filenames using fileNameGenerator
* attempts to open webpage files present in the `pageDirectory`, and then sends them to `indexPage`
* attempts to save the index to the specific `filename`
* if any trouble is found, print an error to stderr and exit non-zero.

### indexPage

Does the real work of opening files in the `pageDirectory`, parsing their contents, and saving information to the index.
Pseudocode:
	read the url and html for the file
    create a temporary webpage struct for reading
    while there are words left in the page:
        if the word is long enough, add to the index
    delete the webpage

### fileNameGenerator

This is just a short, local, helper function that returns a filename using the `pagedirectory` and the `docID` for the next file.

## Other modules

### index

We create a re-usable module `index.c` to handles the index struct and corresponding methods.  mentioned in the design
We chose to write this as a separate module, in `../common`, anticipating future use by the Querier.

Pseudocode for `index_new`:

	allocating space for a new index in memory
	return the pointer for the new index


Pseudocode for `index_add`:

	check the provided index for the word:
        if it doesn't exist, create a new entry as a counter set
        add to the occurance count of the counter set for the specific docID

Pseudocode for `index_get`:

    look in the index for the particular word, return the counter set corresponding to it, if it exists

Pseudocode for `index_save`:

    iteratively go through the index, print the word and doc id pairs for each word

Pseudocode for `index_load`:

    open a provided index filename
    create a new index to store the info
    for each line in the index file:
        create a new entry in the index
        add the corresponding docID count pairs
    return the pointer to the index

Pseudocode for `index_delete`:

    calls delete functions for the inner structs within index

### libcs50

We leverage the modules of libcs50, most notably `file`, `hashtable`, and `webpage`.

## Function prototypes

### indexer

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `crawler.c` and is not repeated here.

```c
int main(const int argc, char* argv[]);
static void indexBuild(char *pageDir, char *indexFilename);
static void indexPage(index_t* index, FILE *fp, int id);
static char* fileNameGenerator(char* pageDir, int docID);

```

### index

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `pagedir.h` and is not repeated here.

```c
index_t *index_new(int num_slots);
bool index_add(index_t *index, char *word, int docID);
counters_t *index_get(index_t *index, const char *word);
void index_delete(index_t *index);
bool index_save(index_t *index, FILE *fp);
index_t *index_load(char *indexFilename);

```

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

That said, certain errors are caught and handled internally: for example, out-of-memory errors, unwriteable or unreadable files, etc. are caught and result in a non-zero exit code

## Testing plan

Here is an implementation-specific testing plan.

### Unit testing

There are only two units (indexer and index), both are testing separately and together in testing.sh
