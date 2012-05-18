#ifndef _Ngrams_h_
#define _Ngrams_h_

#include "INgrams.h"
#include "vector.h"

/**
* class for common ngram operations
* Revisions:
* April 22, 2006. Jerry Yu
* Initial implementation
*/

class Ngrams : public INgrams
{
public:

	Ngrams( int newNgramN );

	~Ngrams();

	/**
	* feed a token in, the token will be processed internally to generating ngram
	*
	* for word ngram, the token will contain word string, 
	* for character ngram, the token will contain a string of the character.
	*/

	virtual void addToken ( IToken * token );


	int getN()
	{
		return ngramN;
	}


protected:


	struct TokenNode
	{
		IToken * token;
		TokenNode * next;
		TokenNode ( IToken * newToken ) : token( newToken ), next( 0 )
		{
		}
	};

	TokenNode * head, * tail;

	int ngramN;	// default number of ngrams

	/**
	* add a ngram to the ngram list.
	* @param ngramVector - a vector representing a ngram
	*/

	void addNgram( Vector<IToken *> * ngramVector );


protected:
	Vector<IToken*> tokens;                /* vector of parsed tokens */
	Vector< Vector<IToken*> * > ngrams;    /* vector of ngrams, each ngrams is represented by a vector of tokens */
 private:
	int tokenCount; //used for counting when parsing text

	/**
	* add token to the queue. The queue will be used to generate ngram
	* @param	token - token to be added to the queue.
	* @return	total number of tokens in the queue
	*/
	int pushQueue( IToken * token );


	void popQueue();

	/**
	* Generate ngrams when queue has NGRAM_N - 1 tokens.
	* the token queue need to be processed specially for the first NGRAM_N - 1 tokens,
	* also need to be called if less than NGRAM_N tokens in the whole input text.
	* @param	count - total items in the queue
	*/
	virtual void preParse( int count )=0;

	/**
	* Once the queue is full, it will start to pop out ngrams
	* for each new token pushing in
	*/
	virtual void parse(){};

	// release queue memory
	void releaseQueue()
	{
		while ( tokenCount > 0 )
		{
			popQueue();
		}
	}
};

#endif
