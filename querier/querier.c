/*
* querier.c - CS50 indexer module
*
* Ben Pable
*
* CS50 S24
*
* see implementation for function details
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>
#include <ctype.h>
#include "file.h"
#include "hashtable.h"
#include "word.h"
#include "counters.h"
#include "webpage.h"
#include "index.h"

/**************** local types ********************/

typedef struct dual_holder { //local type to pass two objects as one parameter
    counters_t *comp;
    counters_t *new;
} dual_holder;

typedef struct sort_holder { //local type to pass two objects as one parameter
    int* array;
    int* CurrNum;
} sort_holder;

/**************** helper function declaration ********************/

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


/**************** main() ********************/
int main(int argc, char** argv)
{
    if(argc != 3){ //check num args
        fprintf(stderr, "usage: ./querier pageDirectory indexFilename\n");
        exit(1);
    }
    char* pageDir = argv[1];
    char* indexFilename = argv[2];

    char testStr[strlen(pageDir)+10]; //check crawler directory
	strcpy(testStr, pageDir);
	strcat(testStr, "/.crawler");
    FILE *testFP = fopen(testStr, "r");
    if(testFP == NULL){
        fprintf(stderr, "failure to validate page directory: %s\n", pageDir);
        exit(2);
    }
    fclose(testFP);

    index_t *pageIndex = index_load(indexFilename); //check index
    if(pageIndex==NULL){
        fprintf(stderr, "failure to load index from: %s", indexFilename);
        exit(3);
    }


    fprintf(stdout, "query? "); //initial user input
    char* currLine = file_readLine(stdin);
    char** tkInputLine;
    int count = parseInput(&tkInputLine, currLine);
    while(count==-1){ //if invalid user input given, repeat until valid input found
        fprintf(stdout, "query? ");
        currLine = file_readLine(stdin);
        count = parseInput(&tkInputLine, currLine);        
    }
    if(count==0){ //if stdin issue or EOF, finish
        index_delete(pageIndex);
        exit(4);
    }
    while(tkInputLine != NULL){
        counters_t* resSet = runQuery(tkInputLine, pageIndex, count); //run query

        int counts[count*2]; //create array of ints for sorting
        for(int countsItr2 = 0; countsItr2<count*2; countsItr2+=1){
            counts[countsItr2]=0;}
        int tempVal = 0;
        struct sort_holder sortHolder = {counts, &tempVal};
        counters_iterate(resSet, &sortHolder, &sortRes); //sort the results

        counters_delete(resSet);
        free(currLine);
        free(tkInputLine);
        printRes(counts, tempVal, pageDir); //print query results

        fprintf(stdout, "\nquery? "); //reinitialize user input
        currLine = file_readLine(stdin);
        fprintf(stdout, "\n");
        count = parseInput(&tkInputLine, currLine);
        while(count==-1){
            fprintf(stdout, "query? ");
            currLine = file_readLine(stdin);
            count = parseInput(&tkInputLine, currLine);        
        }
        if(count==0){index_delete(pageIndex);exit(0);}
    }
    index_delete(pageIndex);
    exit(6); //should never arrive here
}

/**************** helper functions ********************/

static char* fileNameGenerator(char* pageDir, int docID){
	//short helper function to generate the filenames for pulling info
	char idBuffer[12];
	sprintf(idBuffer, "%d", docID);
	char* res = malloc(sizeof(char)*(strlen(pageDir)+strlen(idBuffer)+2));
	strcpy(res, pageDir);
	strcat(res, "/");
	strcat(res, idBuffer);
	return res;
}

//function to take line from stdin and parse, returns the number of words processed for querying
int parseInput(char*** output, char* line){
    if(line == NULL){
        fprintf(stderr, "stdin completed or read error\n");
        return 0;
    }
    int spaceCtr = 0; //space counter
    for(int i = 0; i < strlen(line); i++){ //validate input line
        if(isspace(line[i])){
            spaceCtr++;
        }
        if(!isalpha(line[i]) && !isspace(line[i])){
            fprintf(stderr, "invalid input character at most recent line, pos: %d\n", i);
            free(line);
            return -1;
        }}
    fprintf(stdout, "query: ");
    int wordCtr = 0; //word counter
    int size = strlen(line);
    char** words = (char**)malloc(sizeof(char*)*(spaceCtr+2));
    char* currWord;
    char currChar;
	for (int i = 0; i < size; i++){ //iterate through line
		currChar = line[i];
		if (isalpha(currChar)){ //if letter
			currWord = &line[i]; //set beginning of word to that spot in line
			words[wordCtr] = normalizeWord(currWord);
			wordCtr++;
		   	int j = i; //looking ahead until space is found
		   	char v = line[j];
            while (isalpha(v)){
                j++;
                v = line[j];
            }
		 	line[j] = '\0';
            printf("%s ",words[wordCtr-1]);
		  	i = j; //set next spot for after the current section
		}
	}
    *output = words; //tokenized input
    return wordCtr;
}

//function that takes query words, index, and num of words as args, returns query res
counters_t* runQuery(char** queryRes, index_t* index, int count){
    counters_t* resCounters = counters_new(); //make resulting Counters
    if(resCounters==NULL){
        fprintf(stderr, "out of memory\n");
        return NULL;
    }
    queryHelper(index, resCounters, queryRes, count);
    return resCounters;
}

//does the dirty work of querying
void queryHelper(index_t* pageIndex, counters_t* result, char** words, int count){
    index_t* index = pageIndex;
    counters_t* resSet = result;
    char* word = NULL;
    for (int i = 0; i < count; i++){ //iterate through words
		word = words[i];
        if(word==NULL){ //shouldn't happen, just being defensive
            continue;
        }
		if (!isValidWord(word)){ //if not "and" or "or"
			counters_t *tempCounter = index_get(index, word); //temp counter to start new scorekeeping
			counters_t *scores = counters_new(); //scorekeeper
			if (tempCounter != NULL){	//duplicate temp into scorekeeper
				counters_iterate(tempCounter, scores, scoreInit);
			}
			int j = i + 1; //looking ahead, intersect until an "or" is found
            if(j<count){
			    char* nextWord;
			    nextWord = words[j];
                while ((j < count) && (strcmp(nextWord, "or") != 0)){ //while not finished w current section
                    if (!isValidWord(nextWord)){ //check again for valid word
                        counters_t *tempCtrs = index_get(index, nextWord); //get result for this word 
                        if (tempCtrs == NULL) {
                            tempCtrs = counters_new(); //if nothing found, clear using intersection
                            scoreIntersect(&scores, tempCtrs); //intersect old scores w new findings
                            free(tempCtrs);
                        } else{
                    scoreIntersect(&scores, tempCtrs);} //intersect old scores w new findings
                    }
                    j++;
                    if (j < count) {
                        nextWord = words[j];
                    }
                }
            } //once "or" found or finished, merge with results
			i = j;
            counters_iterate(scores, resSet, scoreMerger); //merge with results
            counters_delete(scores);
    }}
    return;
}

//short check to eliminate any extraneous words
bool isValidWord(char* word){
	if ((strcmp(word, "and") == 0) || (strcmp(word, "or") == 0)) {
		return true;
	} 
	return false;
}

//pulls values from index into scores to initialize items
static void scoreInit(void *arg, const int key, int count){
	counters_t* scoreCtrs = arg; //basically just copy values from first item into score counter
	counters_set(scoreCtrs, key, count);
} 

//function to take the intersection of two counter sets
static void scoreIntersect(counters_t** main, counters_t* comp){
    counters_t* intRes;
    intRes = counters_new();
    if(intRes == NULL){
        fprintf(stderr, "out of memory error\n");
        exit(5);
    }
    struct dual_holder holder = {comp, intRes};
    counters_iterate(*main, &holder, scoreIntersectHelper);
    counters_delete(*main);
    *main = intRes;
}

//does the dirty work of scoreIntersect
static void scoreIntersectHelper(void *arg, int key, int count){
    dual_holder* holder = arg;
    int newCount = counters_get(holder->comp, key);
    if(newCount!=0){
        if(newCount<count){
            counters_set(holder->new, key, newCount);
        } else {
            counters_set(holder->new, key, count);
        }
    }
}

//merges the scores of two counter sets, to be used with counters_iterate
static void scoreMerger(void *arg, const int key, int count){
	counters_t* resScores = arg;
    int currScore = counters_get(resScores, key);
	int sum = count + currScore;
	counters_set(resScores, key, sum);
}

//sorts the resulting words and scores for ranking
void sortRes(void* arg, int key, int count){
    //array goes {key,count,key,count...}
    sort_holder *sortHolder = (sort_holder*) arg;
    if(*sortHolder->CurrNum==0){
        sortHolder->array[0] = key;
        sortHolder->array[1] = count;
        *sortHolder->CurrNum = 2;
    } else { //sort
        for(int i=*sortHolder->CurrNum; i>0; i-=2){ //increment by 2
            if(sortHolder->array[i-1]<count){ //if previous entry smaller, move this entry up
                sortHolder->array[i] = sortHolder->array[i-2];
                sortHolder->array[i+1] = sortHolder->array[i-1];
                sortHolder->array[i-1] = count;
                sortHolder->array[i-2] = key;
            } else{ //otherwise add in place
                sortHolder->array[i] = key;
                sortHolder->array[i+1] = count;
                i = 0;
            }
        }
        *sortHolder->CurrNum = *sortHolder->CurrNum + 2;
    }
}

//prints the score, docID, and link for each result from query, ranked
void printRes(int* sortedRes, int numEntries, char* pageDir){
    int spaces;
    fprintf(stdout, "\nMatches %d documents (ranked):", numEntries);
    for(int i = 0; i<numEntries; i+=2){
        fprintf(stdout,"\nscore ");
        char scoreStr[8];
        sprintf(scoreStr, "%d doc", sortedRes[i+1]);
        spaces = 7-strlen(scoreStr); //getting req. num spaces
        for(int j = 0; j<spaces; j++){
            fprintf(stdout, " ");
        }
        fprintf(stdout, "%s", scoreStr);
        char docIDStr[6];
        sprintf(docIDStr, "%d: ", sortedRes[i]);
        spaces = 5-strlen(docIDStr); //req num spaces
        for(int j = 0; j<spaces; j++){
            fprintf(stdout, " ");
        }
        fprintf(stdout, "%s", docIDStr);
        char* fileLoc = fileNameGenerator(pageDir, sortedRes[i]);
        FILE* fp = fopen(fileLoc, "r"); //pull first line from file, the URL
        free(fileLoc);
        char* link = file_readLine(fp);
        fprintf(stdout, "%s", link);
        fclose(fp);
        free(link);
    }
    fprintf(stdout, "\n");
}