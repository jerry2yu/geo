#ifndef __GEO_DISPLAYHTML_H
#define __GEO_DISPLAYHTML_H
#include <string.h>
#include "config.h"
#include "cgiparser.h"

/**
 * Function: displayHtmlAbsoluteURL 
 * --------------------
 * scan html file and convert all relative url into absolute url path, by append given url to relative html
 * and output the html
 * Update: Feb 2, 2005 by Zheyuan Yu
 */

 void displayHtmlAbsoluteURL (char* html, char* url);


#endif
