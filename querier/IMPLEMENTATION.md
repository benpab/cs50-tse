# Querier Implementation Spec

## Data structures 

 1. *index*, a module providing the data structure to represent the in-memory index, and functions to read and write index files;
 2. *dual_holder*, a module allowing for two counter sets to be passed as one argument in a function;
 3. *sort_holder*, a module allowing for multiple integer arrays to be passed as one argument in a fcuntion;
 4. *word*, a module providing a function to normalize a word.

## Control flow

The Querier is implemented in one file `querier.c`, with four main functions.

### main

The `main` function  validates the inputs, builds the internal index, and takes the query input from stdin and passes it to the rest of the functions.

### parseInput

Given a line from stdin, an array of words is constructed, and the number of words is returned.


### runQuery

Given the queried words, index, and number of words, it constructs a result score and returns it. It implements a helper function queryHelper, which does the most of the dirty work, going through each word, constructing a score, and then intersecting or merging with the rest depending on the preceeding modifier.

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

We leverage the modules of libcs50, most notably `file`, `hashtable`, and `counters`.

## Function prototypes

```c
int main(const int argc, char* argv[]);
int parseInput(char*** output, char* line);
bool isValidWord(char* word);
static char* fileNameGenerator(char* pageDir, int docID);
void setDeleteHelper(void *item);
counters_t* runQuery(char** queryRes, index_t* index, int count);
void queryHelper(index_t* pageIndex, counters_t* result, char** words, int count);
static void scoreInit(void *arg, const int key, int count);
static void scoreMerger(void *arg, const int key, int count);
static void scoreIntersect(counters_t** main, counters_t* new);
static void scoreIntersectHelper(void *arg, int key, int count);
void sortRes(void* arg, int key, int count);
void printRes(int* sortedRes, int numEntries, char* pageDir);

```

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

That said, certain errors are caught and handled internally: for example, out-of-memory errors, unwriteable or unreadable files, etc. are caught and result in a non-zero exit code

## Testing plan

Here is an implementation-specific testing plan.

### Unit testing

There is only one unit (querier), tested in test.sh
