/*
* crawler.c - CS50 crawler module
*
* Ben Pable
*
* CS50 S24
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>
#include "bag.h"
#include "hashtable.h"
#include "pagedir.h"
#include "webpage.h"

/**************** local functions ****************/

static void crawler(char* seedURL, char* pageDirectory, int maxDepth);

static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);


/**************** main ****************/

int main(int argc, char** argv)
{
    //initialize vars and parse
    char* seedURL = argv[1];
    char* pageDirectory = argv[2];
    int maxDepth = -1; //proxy value of -1

    //parse args
    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
    //run crawler
    crawler(seedURL, pageDirectory, maxDepth);
    
    return 0;
    }

static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth){

    if(argc!=4){ //check num args
    fprintf(stderr, "improper # of parameters, usage: ./crawler seedURL pageDirectory maxDepth\n");
    exit(1);
    }
    //validate seedURL
    char *normalizedURL = normalizeURL(*seedURL);
    if(normalizedURL==NULL){
        fprintf(stderr, "seedURL normalization error, check for valid URL\n");
        exit(1);
    }
    strcpy(*seedURL, normalizedURL);
    free(normalizedURL);
    if(isInternalURL(*seedURL)==false){
        printf("%s\n",*seedURL);
        fprintf(stderr, "seedURL not internal\n");
        exit(2);
    }
    //validate directory
    if(!pagedir_init(*pageDirectory)){
        fprintf(stderr, "page directory initialization failed\n");
        exit(3);
    }
    int scanRes = sscanf(argv[3], "%d", maxDepth); //scan for max depth and check for non-int input
    if(scanRes==0){
        fprintf(stderr, "improper maxDepth, use int [0,10]");
        exit(4);
    }
    if(*maxDepth < 0 || *maxDepth > 10){
        fprintf(stderr, "improper maxDepth, must be int [0,10]");
        exit(4);
    }
    
    return;
}

static void crawler(char* seedURL, char* pageDirectory, int maxDepth){
    //initiailize internal structs
    bag_t *pageBag = bag_new();
    hashtable_t *seenPages = hashtable_new(10);
    if(pageBag==NULL || seenPages==NULL){
        fprintf(stderr, "error allocating data structures\n");
        exit(8);
    }
    //initialize seedPage
    char* _seedURL = malloc(sizeof(char)*(1+strlen(seedURL)));
    strcpy(_seedURL, seedURL);
    webpage_t *seedPage = webpage_new(_seedURL, 0, NULL);
    if(seedPage==NULL){
        fprintf(stderr, "error allocating seedPage\n");
        exit(9);
    }
    //begin crawling
    bag_insert(pageBag, seedPage);
    bool initInsert;
    if(!(initInsert = hashtable_insert(seenPages, seedURL, "1"))){
        fprintf(stderr, "failed to insert seedPage\n");
        exit(10);
    }

    webpage_t *currPage = bag_extract(pageBag);
    int id = 1;
    int pageDepth;
    while(currPage != NULL){//while there are pages left to explore
        //create page file
        if(webpage_fetch(currPage)==false){
            fprintf(stderr, "page: %s not found\n", webpage_getURL(currPage));
            webpage_delete(currPage);
            currPage = bag_extract(pageBag);
            continue;
        }
        fprintf(stdout, "Fetched: %s\n", webpage_getURL(currPage));
        pagedir_save(pageDirectory, currPage, &id);
        id++;
        //check depth and scan
        pageDepth = webpage_getDepth(currPage);
        if(pageDepth < maxDepth){
            pageScan(currPage, pageBag, seenPages);
        }
        webpage_delete(currPage);
        currPage = bag_extract(pageBag);
    }
    //free memory and return
    bag_delete(pageBag, webpage_delete);
    hashtable_delete(seenPages, NULL);
    return;
}

static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen){
    //init vars for scanning
    int pos = 0;
    char* res;
    char* dummyItem = "1";
    int depth = webpage_getDepth(page);
    fprintf(stdout, "Scanning: %s\n", webpage_getURL(page));
    while((res = webpage_getNextURL(page, &pos))!=NULL){ //while URLs are found
        fprintf(stdout, "Found: %s\n", res);
        char* normalizedRes = normalizeURL(res);
        if(normalizedRes==NULL){
            fprintf(stderr, "failed to normalize: %s\n", res);
            continue;
        }
        if(isInternalURL(normalizedRes)){ //if internal URL, add to pageBag
            if(hashtable_insert(pagesSeen, normalizedRes, dummyItem)){
                webpage_t* newPage = webpage_new(normalizedRes, depth+1, NULL);
                if(newPage==NULL){
                    fprintf(stderr, "failed to make new page for URL: %s\n", res);
                    free(normalizedRes);
                } else{
                fprintf(stdout, "Added: %s\n", normalizedRes);
    			bag_insert(pagesToCrawl, newPage);
                }
            } else{
                fprintf(stdout, "IgnDupl: %s\n", normalizedRes);
                free(normalizedRes); //if failed, free normlaizedURL
            }
        } else{
            fprintf(stdout, "IgnExtrn: %s\n", normalizedRes);
            free(normalizedRes);
        } //free res
        free(res);
    }
}
