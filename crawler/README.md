# Crawler

The TSE crawler is a standalone program that crawls the web and retrieves webpages starting from a "seed" URL. It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but limiting its exploration to a given "depth".

The crawler does the following:

1. execute from a command line with usage syntax ./crawler seedURL pageDirectory maxDepth
2. mark the pageDirectory as a 'directory produced by the Crawler' by creating a file named .crawler in that directory
3. crawl all "internal" pages reachable from seedURL, following links to a maximum depth of maxDepth; where maxDepth=0 means that crawler only explores the page at seedURL, and maxDepth=1 means that crawler only explores the page at seedURL and those pages to which seedURL links, and so forth inductively.
4. print nothing to stdout, other than logging its progress and write each explored page to the pageDirectory with a unique document ID.
5. exit zero if successful; exit with an error message to stderr and non-zero exit status if it encounters an unrecoverable error

### Compilation

To run the program, run the following series of commands:
```
$ make
$ ./crawler <seedURL> <pageDirectory> <maxDepth>
```

To remove program files, run the following command:
```
$ make clean
```

In this implementation, crawler can **not** have a maxdepth **greater than 10**

Assumption: The pageDirectory does not contain any files whose name is an integer (i.e., 1, 2, ...).


