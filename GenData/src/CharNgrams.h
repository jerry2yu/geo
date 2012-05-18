#ifndef _CHAR_NGRAMS_H_
#define _CHAR_NGRAMS_H_

#include "ngrams.h"

/**
* this class implements all character ngram related operations
* Revisions:
* April 23, 2006. Jerry Yu
* Initial implementation
*/

class CharNgrams : public Ngrams
{

public:

  CharNgrams( int newNgramN );

  virtual ~CharNgrams();

  void addTokens( Vector<IToken*> * token );

	/**
	* sort ngrams by frequency/ngram/or both, then output
	*/

	void output();

private:

	/**
	* Generate ngrams when queue has NGRAM_N - 1 tokens.
	* the token queue need to be processed specially for the first NGRAM_N - 1 tokens,
	* also need to be called if less than NGRAM_N tokens in the whole input text.
	* @param	count - total items in the queue
	*/
	void preParse( int count );

	/**
	* Once the queue is full, it will start to pop out ngrams
	* for each new token pushing in
	*/

	void parse();


};
#endif
