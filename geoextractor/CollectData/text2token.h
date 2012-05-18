#ifndef __GEO_TEXT2TOKEN_H
#define __GEO_TEXT2TOKEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SimpleExtract.h"
#include "cvector.h"
#include "token.h"
#define MY_PATH_MAX 1024 //max length of path
#define DEF_TBSZ 104729//size of ngram hash table




/**
 * usage: getTokens ("/home2/zyu/text2ngram/address.html",tokenVector);
 * -------------------------------
 * read in file stream, and 
 * parse the html file, get all tokens into tokenVector
 * update: Feb 2, 2005 by Jerry Yu
 */

 void getTokens (char* fileName, cvector *tokens);

/**
 * parse the html string, get all tokens into tokenVector
 * update: Feb 2, 2005 by Jerry Yu
 */

 void getTokensFromText (const char* text, cvector *tokensVector) ;

#endif
