#include "WordNgrams.h"

WordNgrams::WordNgrams( int newNgramN ) :
Ngrams( newNgramN )
{
	
}

WordNgrams::~WordNgrams()
{
}

bool WordNgrams::addTokens(  Vector<IToken*> * tokens )
{
  bool ret = false;
  if ( tokens )
    {
      int count = (int)(tokens->count());
      for ( int i=0; i<count; i++ )
	{
	  addToken( tokens->get( i ) );
	}
      ret = true;
    }
	return true;
  
}

void WordNgrams::addToken( IToken * token )
{
  this->Ngrams::addToken( token );
}

void WordNgrams::preParse( int count )
{

	TokenNode * p, * newHead;
	p = newHead = head;
	while ( newHead )
	{
	  p = newHead;

	  Vector<IToken*> * ngramVector = 0;
	  while( p )
	  {
	    if ( !ngramVector )
	      {
		ngramVector = new Vector<IToken*>;
	      }
	    ngramVector->add( p->token );
	    
	    p = p->next;			

	  }
	  newHead = newHead->next;
	  if ( ngramVector )
	    {
	      this->addNgram( ngramVector );
	    }

	}
}

void WordNgrams::parse()
{
 	TokenNode * p, * newHead;
	p = newHead = head;

	while ( newHead )
	{
	  unsigned short n = 0;
	  p = newHead;

	  Vector<IToken*> * ngramVector = 0;
	  while ( p )
	    {
	      if ( !ngramVector )
	      {
		ngramVector = new Vector<IToken*>();
	      }
	      ngramVector->add( p->token );
	
	      //printf("Token %s.\n", p->token->getTokenString().c_str() );
	      ++n;
	      p = p->next;
	      
	    }
	  /* save the ngram to ngram vector */
	   //printf("%d ngram %s.\n", n, ngram.c_str() );
	    if ( n > 0 && ngramVector )
	    {
	      this->addNgram( ngramVector );
	    }
	    
	    newHead = newHead->next;
	}
}


void WordNgrams::output()
{
  int count = (int)(ngrams.count());
  for ( int i=0; i<count; i++ )
    {
      Vector<IToken*> * ngramVector = (Vector<IToken*> * )ngrams[i];
      printf("%zu-gram ", ngramVector->count() );
      for ( int j=0;j<(int)(ngramVector->count());j++)
	{
	  printf("%s_", ((IToken*)(ngramVector->get(j)))->getTokenString().c_str() );
	}
      printf("\n");
    }

}
