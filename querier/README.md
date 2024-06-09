
# Quier

The third component of the Tiny Search Engine is the Querier, which reads the index produced by the Indexer and the page files produced by the Crawler, to interactively answer written queries entered by the user.


The Querier does the following:

1. execute from a command line with usage syntax:
```
./indexer pageDirectory indexFilename
```

2. Query the pageDirectory using the index using input from stdin
 ```
query? <user input>
```

3. Sort the results.

6. Print results to stdout.

7. exit zero if successful; exit with an error message to stderr and non-zero exit status on encountering an unrecoverable error.

### Compilation

To run the program, run the following series of commands:
```
$ make
$ ./querier <pageDirectory> <indexFilename>
```

To remove program files, run the following command:
```
$ make clean
```

### Assumptions



