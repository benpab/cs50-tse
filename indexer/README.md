```
#############
# extension used
#############
```
# Indexer

The TSE indexer is a standalone program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. Its companion, the index tester, loads an index file produced by the indexer and saves it to another file.

The indexer does the following:

1. execute from a command line with usage syntax:
```
./indexer pageDirectory indexFilename
```

2. validate it received exactly two command-line arguments and that:
 pageDirectory is the pathname for a directory produced by the Crawler, and
 indexFilename is the pathname of a file that can be written;

3. read documents from the pageDirectory

4. build an inverted-index data structure mapping from words to (documentID, count) pairs.

5. create a file indexFilename and write the index to that file, in the format described below.

6. output nothing to stdout.

7. exit zero if successful; exit with an error message to stderr and non-zero exit status on encountering an unrecoverable error.

### Compilation

To run the program, run the following series of commands:
```
$ make
$ ./indexer <pageDirectory> <indexFilename>
```

To remove program files, run the following command:
```
$ make clean
```

### Assumptions



