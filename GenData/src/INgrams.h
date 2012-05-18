
#ifndef _INGRAMS_H_
#define _INGRAMS_H_
#include "IToken.h"
/**
* Define the interface for ngram classes
* Revisions:
* April 22, 2006. Jerry Yu
* Initial implementation
*/
class INgrams
{
public:

	/**
	* constructor
	*/
	INgrams( );

	/**
	* destructor
	*/
	virtual ~INgrams();

	/**
	* feed a token in, the token will be processed internally to generating ngram
	*
	* for word ngram, the token will be a token with word string, 
	* for character ngram, the token will be a token with string of the character.
	*/

	virtual void addToken ( IToken * token )=0;

	/**
	* sort ngrams by frequency/ngram/or both, then output
	*/
	virtual void output() =0 ;

};


#endif
