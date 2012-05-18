#include "RE.h"
RE::RE()
{
}

RE::~RE()
{
}

bool RE::run( Vector<IToken*> & tokens )
{

    bool ret = true;

    int count = tokens.count();
    for ( int i=0;i<count;i++)
      {
	GeoToken * token = (GeoToken*)(tokens[i]);
	//	token->setREOther();
	//printf("Token %s\n", token->getTokenString().c_str() );
      }
    /* get a vector of address start and end positions */
    extractor.run( tokens, labels );
    count = labels.count();
    for ( int i=0; i<count; i++ )
      {/* set every extracted address */
	assert ( labels[i]->start< tokens.count() && labels[i]->end < tokens.count() );
	int startIndex = labels[i]->start;
	int endIndex = labels[i]->end;
	//printf("%s-%s\n", tokens[ startIndex ]->getTokenString().c_str(), tokens[ endIndex ]->getTokenString().c_str() );

	/* set start end mid, other RE flag */
	for ( int j=startIndex; j<=endIndex; j++)
	  {
	    GeoToken * token = (GeoToken*)(tokens[ j ]);

	    if ( j == startIndex )
	      {
		token->setREStart();
		//printf("set %s to start value %s\n", token->getTokenString().c_str(), token->isREStart()?"START":"NO");
	      }
	    else if ( j == endIndex)
	      {
		token->setREEnd();
		//printf("set %s to END\n", token->getTokenString().c_str() );
	      }
	    else
	      {
		//printf("set %s to MID\n", token->getTokenString().c_str() );
		token->setREMid();
	      }


	  }

	delete labels[i];
      }
    labels.clear();
    return ret;
}

