#include "WekaHandler.h"

WekaHandler::WekaHandler()
{
}

WekaHandler::~WekaHandler()
{
}

void WekaHandler::outputWordNgrams( Vector< Vector< IToken* > * > * newNgrams, int newNumOfNgrams, bool outputHeader )
{
	ngrams = newNgrams;
	numOfNgrams = newNumOfNgrams;
	if ( outputHeader )
		outputARFFHeader( );
	outputARFFData();
}

void WekaHandler::outputARFFHeader()
{
	printf("%% GEO Tagger Dataset\n%%\n" );
	printf("%% Author: Zheyuan Yu\n%%\n" );
	printf("@relation geo\n");
	for ( int i=0; i<numOfNgrams; i++) 
	{

#if 0
		printf("@attribute punctuation%d {COMMA,PERIOD,EXCLAMATION,QUESTION,SEMICOLON,COLON,PLUS,MINUS,APOSTROPHE,LEFT_PARENTHESIS,RIGHT_PARENTHESIS,DOUBLE_QUOTE,LEFT_BOX_BRACKET,RIGH_BOX_BRACKET,LEFT_CURLY_BRACKET,RIGHT_CURLY_BRACKET,EQUAL,UNDERSCORE,SLASH,BACK_SLASH,NUMBER_SIGN,LEFT_ANGLE_BRACKET,RIGHT_ANGLE_BRACKET,NOTPUNC }\n", i );

		/*Local feature */
		//printf("@attribute wordFormat%d {UNKNOWN,EMAIL,INITCAP,ALLCAPS,CONTAINDIGITS,ALLDIGITS,ACRONYM,WORD,PUNCTUATION,TIME,URL,CONTRACTION, LONELYINITIAL,UPPERCASECHAR,LOWERCASECHAR,CONTAINDASH,PHONE,USZIP,POBOX}\n", i );  
		printf("@attribute wordFormat%d {UNKNOWN,EMAIL,INITCAP,ALLCAPS,CONTAINDIGITS,ALLDIGITS,ACRONYM,WORD,PUNCTUATION,TIME,URL,CONTRACTION, LONELYINITIAL,UPPERCASECHAR,LOWERCASECHAR,CONTAINDASH,PHONE}\n", i );  

		/* part of speech tagger feature */
		printf("@attribute pos%d {' ',',',:,.,'','\"',(,),#,CC,CD,DT,EX,FW,IN,JJ,JJR,JJS,MD,NN,NNP,NNPS,NNS,PDT,POS,PRP,PRP$,RB,RBR,RBS,RP,SYM,TO,UH,VB,VBD,VBG,VBN,VBP,VBZ,WDT,WP,WP$,WRB, PRP|MD,NNP|VBN}\n", i );

		/* layout feature */
		printf("@attribute layout%d {Start,Mid,End,OneToken}\n", i);
#endif
#if 0
		/* geocoder feature */
		printf("@attribute geocoder%d {START,MID,END,OTHER}\n", i );
#endif

		/* regular expression extractor feature */
		printf("@attribute re%d {START,MID,END,OTHER}\n", i);

		/*
		printf("@attribute is_usstate%d {US,NO}\n", i);
		printf("@attribute is_sd%d {SD,NO}\n", i);
		printf("@attribute is_ss%d {SS,NO}\n", i);
		printf("@attribute is_sud%d {SU,NO}\n", i);
		*/
#if 0
		/* geographical feature */
		printf("@attribute dictionary%d {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}\n", i );
	      	printf("@attribute geo%d {USZIP,POBOX,OTHER}\n", i );
#endif
		//printf("@attribute is_others%d {0,1}\n", i);

#if 0
		/*FIPS feature */
		/*		
      		printf("@attribute street%d {STREET,NO}\n", i);
		printf("@attribute city%d {CITY,NO}\n", i);
		printf("@attribute state%d {STATE,NO}\n",i);
		printf("@attribute zip%d {ZIP, NUMBER, NO}\n",i);
		*/
		printf("@attribute fips%d {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31}\n", i );
#endif
		/* token line feature */
		//printf("@attribute len%d NUMERIC\n", i); // length of the token

	}
	printf("@attribute label {START,MID,END,OTHER}\n");
	printf("@data\n");
}

void WekaHandler::outputARFFData()
{
	int count = (int)(ngrams->count());/* total number of ngrams */
	for ( int i=0; i<count; i++ )
	{
		outputNgramFeatures( ngrams->get(i) );
		printf(",");
		outputNgramClass( ngrams->get(i) );
		/* output comment of instance */
		// printf(" %%");
		// outputNgramDesc( ngrams->get(i) );

		printf("\n");
	}
}

void WekaHandler::outputNgramFeatures( Vector< IToken * > * ngramVector )
{
	int count = (int)(ngramVector->count()); /* num of tokens in the ngram */
	for ( int i=0; i<count; i++ )
	{ /* output each token in the ngram */
		GeoToken * token = (GeoToken*) ngramVector->get(i);
		outputTokenFeatures( token ); /* output features for the token */
		if ( count -1 != i )
		{/* not last token in the ngram */
			printf(",");
		}
	}
}

void WekaHandler::outputNgramDesc( Vector< IToken * > * ngramVector )
{
	int count = (int)(ngramVector->count()); /* num of tokens in the ngram */
	for ( int i=0; i<count; i++ )
	{ /* output each token in the ngram */
		GeoToken * token = (GeoToken*) ngramVector->get(i);
		printf("%s", token->getTokenString().c_str() );
		if ( count -1 != i )
		{/* not last token in the ngram */
			printf("_");
		}

	}
}

void WekaHandler::outputNgramClass( Vector< IToken * > * ngramVector )
{
	int count = (int)(ngramVector->count()); /* num of tokens in the ngram */
	assert( 1 == count % 2 && count >= 1 );
	int index = count/2;
        GeoToken * token = (GeoToken*)(ngramVector->get(index));
	printf("%s", token->isGeoStart()?"START": token->isGeoMid()?"MID": token->isGeoEnd()?"END": token->isGeoOther()?"OTHER":"UNKNOWN" );
	//printf("%s", ((GeoToken*)(ngramVector->get(index)))->getGeo() ? "GEO" : "NOTGEO" );
}


#define STREET 0x0001
#define CITY   0x0002
#define STATE  0x0004
#define ZIP    0x0008
#define NUMBER 0x0010

void WekaHandler::outputFipsFeatures( GeoToken * token )
{
  assert( token );

  //  printf("%s,%s,%s,%s,%s", token->isStreet()?"STREET":"NO", token->isCity()?"CITY":"NO", token->isState()?"STATE":"NO", token->isZip()?"ZIP": token->isNumber() ? "NUMBER" : "NO" );
  unsigned short ret = 0;
  if ( token->isStreet() )
    ret |= STREET;
  if ( token->isCity() )
    ret |= CITY;
  if ( token->isState() )
    ret |= STATE;
  if ( token->isZip() )
    ret |= ZIP;
  if ( token->isNumber() )
    ret |= NUMBER;
  printf(",%u", ret );
}

#define USSTATE 0x0001
#define SD      0x0002
#define SS      0x0004
#define SUD     0x0008
#define USZIP   17
#define POBOX   18
void WekaHandler::outputGeographicalFeatures( GeoToken * token )
{
  assert( token );
  /*  printf(",%s,%s,%s,%s", 
		token->isUSState()?"US":"NO", token->isStreetDirection()?"SD":"NO",
		token->isStreetSuffix()?"SS":"NO", token->isSecondaryUnitDesignator()?"SU":"NO" );
  */
  unsigned short ret = 0;
  if ( token->isUSState() )
    ret |= USSTATE;
  if ( token->isStreetDirection() )
    ret |= SD;
  if ( token->isStreetSuffix() )
    ret |= SS;
  if ( token->isSecondaryUnitDesignator() )
    ret |= SUD;
    printf(",%u", ret );
    printf(",%s", token->getLocalFeature()== USZIP ?"USZIP":token->getLocalFeature()==POBOX?"POBOX":"OTHER");
  
       
}

void WekaHandler::outputTokenFeatures( GeoToken * token )
{
#if 0

	/* punctuation feature */
  	printf("%s", token->getPunctuationName().c_str() );

	/* word format feature */
	printf(",%s", token->getLocalFeatureName().c_str() );


	/* part of speech tagging feature */
	const char * pos = token->getPos();
	pos[0] == '"' || pos[0] == ',' ? printf(",'%s'", pos ) : strlen(pos) == 0 ? printf(",' '") : printf( ",%s", pos );
	//pos[0] == '"' || pos[0] == ',' ? printf("'%s'", pos ) : strlen(pos) == 0 ? printf("' '") : printf( "%s", pos );


	/* layout feature */
	printf(",%s", token->isLineMid()?"Mid":token->isLineStart()?"Start":token->isLineEnd()?"End":token->isLineJustOneToken()?"OneToken":"Other");
#endif

#if 0
	/* geocoder feature */
	printf("%s", token->isGeocoderStart()?"START":token->isGeocoderMid()?"MID":token->isGeocoderEnd()?"END":token->isGeocoderOther()?"OTHER":"UNKNOWN");
#endif

	/* regular expression extractor feature */
	printf("%s", token->isREStart()?"START":token->isREMid()? "MID" : token->isREEnd()?"END":token->isREOther()?"OTHER":"UNKNOWN" );


#if 0
	/* geographical feature */
	outputGeographicalFeatures( token );
#endif

#if 0
	/* FIPS feature */
	outputFipsFeatures( token );

#endif
	/* token line feature */
	//printf(",%d", token->getTokenString().length() );

}
