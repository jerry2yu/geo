
#ifndef __Geo_Address_H
#define __GEO_Address_H
#include "cvector.h"

typedef struct {
	char* address;
	int start; //start position
	int end; //end position
	int country;
} Address;

/* set the scan string that will be scanned by flex  */
void setString(char* s);

/**
 * extract address from source string s
 * and save extracted address, position, 
 * and country into addressVector
 * addressVector must be allocated and disposed 
 * by the caller program
 * @param s - string that addresses are to be extracted from
 * @param position - array of the address positions.
 * @param addressVector - vector of extracted addresses
 */

void extractAddress(char* s, int* positions, cvector* addressVector);


#endif
