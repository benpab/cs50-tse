/*
* indexer.c - CS50 indexer module
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
#include "file.h"
#include "hashtable.h"
#include "webpage.h"
#include "index.h"


static void indexBuild(char *pageDir, char *indexFilename);

static void indexPage(index_t* index, FILE *fp, int id);

static char* fileNameGenerator(char* pageDir, int docID);

/**************** main() ********************/

int main(int argc, char* argv[])
{
	if (argc != 3){ //check for num args
		fprintf(stderr, "usage: ./indexer pageDir filename\n");
		exit(1);
	}

	char* pageDir= argv[1];	//check for valid pageDir
    char *checkDir = malloc(sizeof(char)*((strlen(pageDir)+strlen("/.crawler")+1)));
	checkDir[0] = '\0';
    strcat(checkDir, pageDir);
    strcat(checkDir, "/.crawler");
    FILE *fpTest = fopen(checkDir, "r");
    free(checkDir);
    if(fpTest == NULL){
		fprintf(stderr, "unable to find ./crawler file in given pageDir, exiting...\n");
        exit(2);
    }
    fclose(fpTest);


	char* filename = argv[2];	//check file to make sure it can be written to
	FILE *fp = fopen(filename, "w");
	if (fp == NULL){
		fprintf(stderr, "filename is not acceptable for writing\n");
		exit(3);
	}else{
		fclose(fp);
	}

	indexBuild(pageDir, filename); //create index
	exit(0);
}

/************* helper functions ***************/

static void indexBuild(char* pageDir, char* indexFilename){
	index_t *indexName = index_new(900); //create index
	char* filename;
	int id = 1;
	filename = fileNameGenerator(pageDir, id); //create initial filename
	if (filename == NULL) { //if fileNameGen fails
		free(filename);
		exit(4);
	}
	FILE *fp;
	fp = fopen(filename, "r");
	while (fp != NULL){ //while the requested file exists
		indexPage(indexName, fp, id);
		id++; //update id for next configuration
		free(filename); 
		filename = fileNameGenerator(pageDir, id);
		fp = fopen(filename, "r");
	}
	free(filename); //fileNameGen. alloc's memory so need to free

	FILE *fpOut = fopen(indexFilename, "w"); //create output file
	if(fpOut==NULL){
		fprintf(stderr, "unable to write index file");
		exit(6);
	}
	bool saveRes = index_save(indexName, fpOut);
	if(saveRes==false){
		fprintf(stderr, "unable to save index file");
		fclose(fpOut);
		index_delete(indexName);
		exit(5);
	}
	fclose(fpOut);
	//once index is completed, 
	index_delete(indexName);
}


void indexPage(index_t* index, FILE *fp, int id){
	char *url = file_readLine(fp);
	char *depthStr = file_readLine(fp);
	free(depthStr); //depth is unnecessary
	char *html = file_readFile(fp);
	fclose(fp);
	//create temp webpage to parse HTML
	webpage_t *page = webpage_new(url, 0, html);
	int pos = 0;
	char *currWord;
	while ((currWord = webpage_getNextWord(page, &pos)) != NULL){ //while words left
		if (strlen(currWord) > 2){ //check to see if word is long enough
			index_add(index, currWord, id);
		}
		free(currWord); //free for next currWord
	}
	//delete everything
	webpage_delete(page);
 }

static char* fileNameGenerator(char* pageDir, int docID){
	//short helper function to generate the filenames for indexBuild
	char idBuffer[12];
	sprintf(idBuffer, "%d", docID);
	char* res = malloc(sizeof(char)*(strlen(pageDir)+strlen(idBuffer)+2));
	strcpy(res, pageDir);
	strcat(res, "/");
	strcat(res, idBuffer);
	return res;
}