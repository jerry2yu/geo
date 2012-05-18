#ifndef _GEO_GETADDRESS_
#define _GEO_GETADDRESS_

#define HIGHLIGHT_TAG0 "<B style='color:black;background-color:#ffff66'>"
#define HIGHLIGHT_TAG1 "<B style='color:black;background-color:#A0FFFF'>"
//#define HIGHLIGHT_TAG2 "<B style='color:black;background-color:#99ff99'>"
//#define HIGHLIGHT_TAG3 "<B style='color:black;background-color:#ff9999'>"
#define HIGHLIGHT_TAG_END "</B>"
#define GEO_TAG_START_US "<GEO TYPE=\"US\">"
#define GEO_TAG_START_CA "<GEO TYPE=\"CA\">"
#define GEO_TAG_START_UK "<GEO TYPE=\"UK\">"
#define GEO_TAG_END "</GEO>"

#include <stdio.h>
#include "cgiparser.h"
int getAddress (char* url);
/**
 * Function: saveAddress
 * ---------------------
 * save tagged text to file
 * will saved to two files, and save original file
 * 1. url
 * format: id	right/wrong:(0/1) numRight(5 digits), numTotal(5 digits) url
 * 2 files 
 * name format (f0000, f00001, ... f000id)
 * 3 original file
 * name format g00000, g00001, ... g000id
 * check url file, to see whether this url already in the list
 * if not append it to the file, and save tagged webpage to file f0000newid
 * if exists, update the url file: id right/wrong @numRight @numWrong url
 * @numRight: total number of correctly extracted addresses
 * @numExtracted: total number of addressed extracted by rule base algrithm
 * @numWrong: total number of addresses in the webpage
 * exists, update the url file: id right/wrong url
 */

void saveTaggedText(char* url, char* originalText, char* taggedText, int rightOrWrong, int numRight, int numExtracted, int numTotal);
#endif

