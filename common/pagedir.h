/*
* Header file for pagedir.c module
* 
* Ben Pable - CS50 S24
*/

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include "webpage.h"

/**************** functions ****************/

// checks a directory and marks it for use by crawler
bool pagedir_init(char* pageDirectory);

// creates a file storing the basic info and contents for a webpage
void pagedir_save(char* pageDirectory, webpage_t *page, int* fileCtr);

#endif // __PAGEDIR_H