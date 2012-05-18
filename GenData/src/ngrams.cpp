#include "ngrams.h"

Ngrams::Ngrams( int newNgramN ) :
ngramN(newNgramN)
{
	// initial queue
	head = tail = 0;
	tokenCount = 0;
}

Ngrams::~Ngrams()
{
  releaseQueue();

  /* free memory of pointers of ngramVectors, each ngramVector represents a ngram */
  int count = (int)(ngrams.count());
  for ( int i=0; i<count; i++ )
    {
       delete (Vector<IToken*>*)(ngrams[i]);
    }

  /* free memory of tokens */
  count = (int)(tokens.count());
  for ( int i=0; i<count; i++ )
  {
    delete (IToken*)(tokens[i]);
  }

}

void Ngrams::addToken ( IToken * token )
{
  //  printf("add token %s.\n", ((IToken*)token)->toString().c_str() );
  	int count = pushQueue( token );
	if ( count == this->ngramN )
	{
		parse();
		popQueue();
	} 
	else if ( count == this->ngramN - 1 )
	{
		preParse( count );
	} 

}

void Ngrams::addNgram( Vector<IToken*> * ngramVector )
{
  assert ( ngramVector );
  ngrams.add( ngramVector );
#if 0
  printf("%d-gram ", ngramVector->count() );
  for ( int j=0;j<ngramVector->count();j++)
    {
      printf("%s_", ((IToken*)(ngramVector->get(j)))->getTokenString().c_str() );
    }
  printf("\n");
#endif

}

int Ngrams::pushQueue( IToken * token )
{
  IToken * newToken = token->clone( token->getTokenString() );
  tokens.add( newToken );
  TokenNode * tokenNode = new TokenNode( newToken );
  if ( !head )
    {
      head = tokenNode;
    }
  else
    {
      this->tail->next = tokenNode;
    }
  this->tail = tokenNode;

  return ++tokenCount;
  
}

void Ngrams::popQueue()
{
  if ( head )
  {
	if ( head == tail )
	{
	  tail = 0;
	}
	TokenNode * p = head->next;
	delete head;
	head = p;
	--tokenCount;
  }
}
