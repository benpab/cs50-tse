/*
* word - CS50 word module
*
* see word.h for more detailed information
*
* Ben Pable - CS50 S24
*/

#include <stdlib.h>
#include <ctype.h>
#include "word.h"

/**************** Global Functions ****************/

char *normalizeWord (char *word){
//make sure word isn't null
  if (word != NULL) { //for all characters, make lowercase
    for (char *cChar = word; *cChar != '\0'; cChar++)
      *cChar = tolower(*cChar);
  }
  return word;
}