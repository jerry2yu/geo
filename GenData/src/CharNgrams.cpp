#include "CharNgrams.h"

CharNgrams::CharNgrams( int newNgramN ) :
Ngrams( newNgramN )
{

}

CharNgrams::~CharNgrams()
{
}

void CharNgrams::addTokens( Vector<IToken*> * tokens )
{
  bool ret = false;
  if ( tokens )
    {
      int count = (int)(tokens->count());
      for ( int i=0; i<count; i++ )
	{
	  IToken * token = tokens->get(i);
	  string str = token->getTokenString();
	  int len = (int)(str.length());
	  for ( int j=0;j<len; j++ )
	  {
	    IToken * newToken = token->clone( str[j] );
	    addToken( newToken );
	    delete newToken;
	  }
	  printf("addTokens %s.\n", tokens->get(i)->getTokenString().c_str() );
	  //addToken( tokens->get( i ) );
	}
      ret = true;
    }

}

void CharNgrams::preParse( int count )
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
void CharNgrams::parse()
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

void CharNgrams::output()
{
  int count = (int)(ngrams.count());
  for ( int i=0; i<count; i++ )
    {
      Vector<IToken*> * ngramVector = (Vector<IToken*> * )ngrams[i];
      printf("%zu-char gram ", ngramVector->count() );
      for ( int j=0;j<(int)(ngramVector->count());j++)
	{
	  printf("%s_", ((IToken*)(ngramVector->get(j)))->getTokenString().c_str() );
	}
      printf("\n");
    }

}
