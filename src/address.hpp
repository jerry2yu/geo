
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
void set_string(char* s);
/*scan */
void flex_match();

/*********************************
* extract address from source string
* get address start and end position from vector positions
* and save extracted address, position, 
* and country into addressVector
* addressVector must be allocated and disposed 
* by the caller program
**********************************/

void extract_address(char* s, int* positions, cvector* addressVector);


#endif
