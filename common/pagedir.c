/*
* pagedir - CS50 pagedir module
*
* see pagedir.h for more detailed information
*
* Ben Pable - CS50 S24
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "webpage.h"
#include "pagedir.h"

bool pagedir_init(char* pageDirectory){
    //validate directory
    if(pageDirectory==NULL){
        fprintf(stderr, "null pageDirectory provided");
        return false;
    }
    //create .crawler file
    if((pageDirectory[strlen(pageDirectory)-1])=='/'){ //if trailing slash given, remove
        pageDirectory[strlen(pageDirectory)-1] = '\0';
    }
    char* crwlrStr = malloc(sizeof(char)*(strlen(pageDirectory)+1+strlen("/.crawler")));
    if(crwlrStr==NULL){
        fprintf(stderr, "memory alloc. error\n");
        return false;
    }
    strcpy(crwlrStr, pageDirectory);
    strcat(crwlrStr, "/.crawler");
    FILE *fp = fopen(crwlrStr, "w");
    if(fp==NULL){
        fprintf(stderr, "invalid pageDirectory\n");
        free(crwlrStr);
        return false;
    }
    fclose(fp);
    free(crwlrStr);
    return true;
}

void pagedir_save(char* pageDirectory, webpage_t *page, int* fileCtr){
    //create filename
    if(*fileCtr<1){
        fprintf(stderr, "invalid file id provided for page: %s\n", webpage_getURL(page));
        exit(5);
    }
    char filename[12];
    sprintf(filename, "/%d", *fileCtr);
    char* fullFilename = malloc(sizeof(char)*(strlen(filename)+strlen(pageDirectory)+1));
    if(fullFilename==NULL){
        fprintf(stderr, "memory allocation error\n");
        exit(6); 
    }
    strcpy(fullFilename, pageDirectory);
    strcat(fullFilename,filename);
    //write file
    FILE *fp = fopen(fullFilename, "w");
    free(fullFilename);
    if(fp==NULL){
        fprintf(stderr, "unable to write file: %s\n", fullFilename);
        exit(7);
    }
    //print all required elements to the file
    fprintf(fp,"%s\n%d\n%s\n", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));
    fclose(fp);
    return;
}