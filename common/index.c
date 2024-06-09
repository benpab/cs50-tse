/*
* index - CS50 index module
*
* see index.h for more detailed information
*
* Ben Pable - CS50 S24
*/

#include <stdlib.h>
#include "word.h"
#include "file.h"
#include "hashtable.h"
#include "counters.h"
#include "index.h"

/**************** local types ****************/
typedef struct index {
    hashtable_t *hashTable;
    int numSlots;
} index_t;

/**************** local functions ****************/
void delete_counters(void *item){
    counters_t *ctr = item;
    counters_delete(ctr);
}

void counters_saver(void *arg, const int key, int count){
    FILE *fp = arg; //cast arg as fp, then print the docID count pairs
    fprintf(fp, "%d %d ", key, count);
}

void hashtable_saver(void *arg, const char *word, void *counter){
    FILE *fp = arg; //cast arg as fp, counter as counters 
    counters_t *counters = counter;
    fprintf(fp, "%s ", word); //print the word, then iterate through counters as print docID count pairs
    counters_iterate(counters, fp, (*counters_saver));
    fprintf(fp, "\n");
}

/**************** global module functions ****************/

//using the function naming convention that we've been using for other modules, not camelCase :(
index_t *index_new(int numSlots){
    index_t *index = malloc(sizeof(index_t)); //allocate memory for index
    index->hashTable = hashtable_new(numSlots); //allocate memory for hashtable
    index->numSlots = numSlots;
    return index;
}

bool index_add(index_t *index, char *word, int docID){
    if (index != NULL && word != NULL && docID >= 0){ //validate inputs
    word = normalizeWord(word);
        counters_t *currCounter = hashtable_find(index->hashTable, word); //check if entry for word exists 
        if (currCounter == NULL){ //if not found, create new counter
            counters_t *newCounter = counters_new();
            counters_add(newCounter, docID);
            hashtable_insert(index->hashTable, word, newCounter);
            return true;
        } else{
            counters_add(currCounter, docID);
            return true;
        }
    } //if invalid inputs given
    return false;
}

counters_t *index_get(index_t *index, const char *word){
    counters_t *counter = hashtable_find(index->hashTable, word);
    return counter;
}

void index_delete(index_t *index){
    hashtable_delete(index->hashTable, (*delete_counters));
    free(index);
}


bool index_save(index_t *index, FILE *fp){
    if (fp != NULL && index != NULL){ //make sure inputs are valid
        hashtable_iterate(index->hashTable, fp, (*hashtable_saver));
        return true;
    } else{ //if invalid inputs, return false
        return false;
    }
}

index_t *index_load(char *indexFilename){
    //open file for reading
    FILE *fp = fopen(indexFilename, "r");
    if (fp != NULL) {
        //initialize vars for the new index
        index_t *index = index_new(900);
        char *word = NULL;
        int docID = 0;
        int count = 0;
        int fscanfCheck;
        int i = 0;
        while ((word = file_readWord(fp)) != NULL) { //read word, get docIDs and counts
            while ((fscanfCheck = fscanf(fp, "%d %d ", &docID, &count)) > 0) { //fscanf looks for docID count pairs until, returns 0 if its not found
                for(i=0; i<count; i++){ //add until count is matched in new index
                    index_add(index, normalizeWord(word), docID);
                }
            }
            free(word);
        }
        return index;
    } //if file failed to open, return NULL
    return NULL;
}

