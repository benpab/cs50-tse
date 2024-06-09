/*
* Header file for index.c module
* 
* Ben Pable - CS50 S24
*/

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdbool.h>
#include "hashtable.h"
#include "counters.h"

/**************** data struct ****************/
typedef struct index index_t;

/**************** helper functions ****************/


void delete_counters(void *item);

/* Function to pass to hashtable_iterate that prints
 * the index file
 *
 * arg is the file pointer to print to, key is the word and
 * item is a pointer to a counter
 */
void hashtable_saver(void *arg, const char *key, void *item);

/**************** function declarations ****************/

//create a new index with the specific number of slots
//returns pointer to the index
index_t *index_new(int num_slots);

//take an index, word, and docID as parameters
//logs the presence of that word in the doc in the index
bool index_add(index_t *index, char *word, int docID);

//gets the counters associated with a specific word for the passed index
counters_t *index_get(index_t *index, const char *word);

// deletes the contents of the passed index
void index_delete(index_t *index);

// takes an index and a file pointer and writes index contents to the file
// returns true if successful, false otherwise
bool index_save(index_t *index, FILE *fp);

index_t *index_load(char *indexFilename);

#endif //__INDEX_H