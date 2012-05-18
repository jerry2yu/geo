#include "C45WordNgrams.h"

bool C45WordNgrams::outputHeader = true;

C45WordNgrams::C45WordNgrams( int newNgramN ) :
  GeoWordNgrams( newNgramN )
{
}

C45WordNgrams::~C45WordNgrams()
{
}
/**
 * feed in all tokens.
 * @param tokens - vector to hold all the tokens
 */

bool C45WordNgrams::addTokens(   Vector<IToken*> * tokens )
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

void C45WordNgrams::output()
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
  c45Handler.outputWordNgrams( &newNgrams, getN(), outputHeader );
  outputHeader = false;
}
