#include "GeoWordNgrams.h"

GeoWordNgrams::GeoWordNgrams( int newNgramN ) : WordNgrams( newNgramN )
{
}

GeoWordNgrams::~GeoWordNgrams()
{
}

void GeoWordNgrams::output()
{
  int count = (int)(ngrams.count());

  for ( int i=0; i<count; i++ )
    {
      Vector<IToken*> * ngramVector = (Vector<IToken*> * )ngrams[i];
      // printf("%d-geogram ", ngramVector->count() );
      for ( int j=0;j<(int)(ngramVector->count());j++)
	{
	  GeoToken * token = (GeoToken*) ngramVector->get(j);
	  
	  printf("%s,", token->getTokenString().c_str() );
	    printf("%d,", token->getGeo()  );
	  if ( token->isLineStart() )
	    printf("LineStart," );
	  if ( token->isLineEnd() )
	    printf("LineEnd" );
	  printf( "%s, ", token->getPos() ? token->getPos() : "NOPOS"  );
	  printf("%s, ", token->isGeocoderAddress() ? "true":"false" );
	  
	}
      for ( int j=0;j<(int)(ngramVector->count());j++)
	{
	  printf(" %s", ngramVector->get(j)->getTokenString().c_str() );
	}

      printf("\n");
    }
}