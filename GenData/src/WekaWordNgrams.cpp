#include "WekaWordNgrams.h"

bool WekaWordNgrams::outputHeader = true;

WekaWordNgrams::WekaWordNgrams( int newNgramN ) :
  GeoWordNgrams( newNgramN )
{
}

WekaWordNgrams::~WekaWordNgrams()
{
}
/**
 * feed in all tokens.
 * @param tokens - vector to hold all the tokens
 */

bool WekaWordNgrams::addTokens(   Vector<IToken*> * tokens )
{
  assert( tokens );
  int dummyCount = getN()/2; /* ngram order */
  for ( int i=0; i<dummyCount;i++)
    {
      tokens->insert( new GeoToken("DUMMY",0,0), 0 );      
    }
  for ( int i=0; i<dummyCount;i++)
    {
      tokens->add( new GeoToken("DUMMY",0,0) );      
    }
  GeoWordNgrams::addTokens( tokens );
}

void WekaWordNgrams::output()
{
  int count = (int)(ngrams.count());
  //  int numNgrams = 3;
  int slidingWindowSize = 1;
  Vector< Vector<IToken*> * >  newNgrams;
  /* keep only n grams, filter out ngrams with other num of n */
  for ( int i=0; i<count; i++ )
    {
      Vector<IToken*> * ngramVector = ngrams.get(i);
      if ( getN() == ngramVector->count() )
	{
	  newNgrams.add( ngramVector );
	}
    }
  //  printf("new vector count %d.\n", newNgrams.count() );
  wekaHandler.outputWordNgrams( &newNgrams, getN(), outputHeader );
  outputHeader = false;
}
