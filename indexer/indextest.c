/* 
 * indextest.c - CS50 indexer test file
 *
 * The *index tester* is the unit test of the indexer; it loads an index file 
 * produced by the indexer and saves it to another file.
 *
 * Ben Pable - CS50 S24
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "index.h"

/**************** main() ********************/

int main(int argc, char* argv[])
{
	// check number of arguments
	if (argc != 3) {
		fprintf(stderr, "usage: indexFile targetFile\n");
		exit(6);
	}

	char* indexFile = argv[1];
	char* targetFile = argv[2];
	index_t *index = index_load(indexFile);
    if(index==NULL){
        fprintf(stderr, "unable to open index file\n");
        exit(7);
    }
    FILE *fp = fopen(targetFile, "w");
	bool saveRes = index_save(index, fp);
	if(saveRes==false){
        fprintf(stderr, "unable to save new index\n");
        fclose(fp);
        exit(8);
    }
    fclose(fp);
	index_delete(index);
	exit(0);
}