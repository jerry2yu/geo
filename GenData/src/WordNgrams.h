#ifndef _WORD_NGRAMS_H_
#define _WORD_NGRAMS_H_

#include "ngrams.h"
/**
* class for all word ngrams related operations
*
* Revisions:
* April 22, 2006. Jerry Yu
* Initial implementation
*/
class WordNgrams : public Ngrams
{

public:

	WordNgrams( int newNgramN );

	/**
	 * feed in all tokens.
         * @param tokens - vector to hold all the tokens
	 */

	virtual bool addTokens(   Vector<IToken*> * tokens );

	/**
	* feed a token in, the token will be processed internally to generating ngram
	*
	* the token will be word string, 
	*/
	void addToken ( IToken * token );

	virtual ~WordNgrams();

	/**
	* output ngrams
	*/
	virtual void output();

private:

	/**
	* Generate ngrams when queue has NGRAM_N - 1 tokens.
	* the token queue need to be processed specially for the first NGRAM_N - 1 tokens
	*/
	void preParse( int count );

	/**
	* Once the queue is full, it will start to pop out ngrams
	* for each new token pushing in
	*/

	void parse();


};
#endif
