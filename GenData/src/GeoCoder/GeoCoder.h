#ifndef GEOCODER_H_
#define GEOCODER_H_

#include "../vector.h"
#include "../GeoToken.h"
#include "geo.h"
#include "io.h"


class GeoCoder
{
public:
	GeoCoder();
	~GeoCoder();
	/**
	* tag the address segments in the tokens, by tagging the start and end token.   
	*/
	void run( Vector<IToken*> & tokens );
	void runFIPS( Vector<IToken*> & tokens );

private:
	/**
	* get the address end poistion
	* @param tokens - tokens in the text to be geo tagged
	* @param start - first token for the potential address
	* @return -1 if it is not an valid address, otherwise return the position of last token in the address
	*/ 
	int getAddressEnd( Vector<IToken*> & tokens, int start );
	/**
	* @param buffer string to be geo tagged
	* @return the physical memory address of the last token in the address, -1 if the string is not an address
	*/
	int getAddressEnd( const char * buffer );
	/**
	* Set flag to token to indicate tokens belong to an address
	* @tokens - vector to hold all tokens in a text
	* @startIndex - index of first token in the address
	* @endIndex - index of last token in the address
	*/
	void setAddressTag( Vector<IToken*> & tokens, int startIndex, int endIndex );
	/**
	* The strcasecmp() function shall compare, while ignoring differences in case, 
	* the string pointed to by s1 to the string pointed to by s2. 
	* The strncasecmp() function shall compare, while ignoring differences in case, 
	* not more than n bytes from the string pointed to by s1 to the string pointed to by s2.
	*
	* The strcmp subroutine then returns a value that is:
	* Less than 0 if the value of string String1 is lexicographically less than string String2.
	* Equal to 0 if the value of string String1 is lexicographically equal to string String2.
	* Greater than 0 if the value of string String1 is lexicographically greater than string String2.
	*/
	int strncasecmp(const char *s1, const char *s2, size_t n);

	/**
	*get length of the name stored in map data file
	* @param len - sizeof the string
	*/
	int getNameLengthInDatabase( const char * name, int len );

	/**
	* Recursively binary search the map file to find given name.
	* @param	begin - start search position of the file
	* @param	end - end search position of the file
	* @param	index - map file
	* @param	name - name to search
	* @param	name_len - length of the name string
	*/
	int find_name( int begin,int end, char type,const char *name,int name_len );

	/**
	 * mark the token with the GEO features
	 */
	void tagToken( Vector<IToken*> & tokens, int start, int end, char type );

	/**
	* lookup the map data, tag names with city, street, state or country 
	* @return index of next token to be tagged
	*/
	int getNames( Vector<IToken*> & tokens, int index );

	/**
	* lookup alias database, replace it if alias found
	* for example, "New York" will be replace by "NY"
	*/
	int getName( String & name );

	/**
	 * lookup database for the give name and type
	 * @type	'C':city, 'S':state 'A' Alias 'O''E': street
	 * @name	name to lookup
	 * @partialMatch	return 1 if current name match partial in the database
	 * @return 1 if name found in database, otherwise 0
	 */
	int getName( char type, String & name, bool & partialMatch );

	/**
	 * Whether current token is a valid ZIP
	 * @return	0 if current is not valid ZIP, otherwise 1
	 */
	int getZip( String str );

	/**
	 * Whether current token is a valid ZIP
	 * @return	0 if current is not valid ZIP, otherwise 1
	 */
	int isValidZip();

	int streetNo;
	struct { int begin,end; } range[40];
	int rangeCount;
	struct io_file *index; /* geo map index */
	int lastNamePos; /* position of last name found */
	int lastNumberPos; /* position of last found number */
	struct geo_location * out;

};

#endif
