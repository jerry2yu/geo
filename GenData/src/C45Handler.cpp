#include "C45Handler.h"

C45Handler::C45Handler() : fp(NULL)
{
}

C45Handler::~C45Handler()
{
  
}

void C45Handler::outputWordNgrams( Vector< Vector< IToken* > * > * newNgrams, int newNumOfNgrams, bool outputHeader )
{
    ngrams = newNgrams;
    numOfNgrams = newNumOfNgrams;
    const char * headerFileName = "c45.names";
    const char * dataFileName = "c45.data";

    
    if ( outputHeader )
      {
	fp = fopen( headerFileName, "w+" );
	if ( fp )
	  {
	    outputC45Header( );
	    fclose( fp );
	    fp = fopen( dataFileName, "w+" );/* clear data file */
	  }
      }
    if ( !fp )
      fp = fopen( dataFileName, "a");

    if ( fp )
      {
	outputC45Data();
	fclose( fp );
      }

    fp = NULL;
}

void C45Handler::outputC45Header()
{

  fprintf( fp, "| GEO Tagger Dataset\n|\n" );
  fprintf( fp, "| Author: Zheyuan Yu\n|\n" );
  fprintf( fp, "START,MID,END,OTHER.\n\n");
  for ( int i=0; i<numOfNgrams; i++) 
  {

    /*Local feature */
    fprintf( fp, "wordFormat%d: UNKNOWN,EMAIL,INITCAP,ALLCAPS,CONTAINDIGITS,ALLDIGITS,ACRONYM,WORD,PUNCTUATION,TIME,URL,CONTRACTION, LONELYINITIAL,UPPERCASECHAR,LOWERCASECHAR,CONTAINDASH,PHONE.\n", i );  


    /* geographical feature */
    fprintf( fp, "dictionary%d: 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15\n", i );
    fprintf( fp, "geo%d: USZIP,POBOX,OTHER\n", i );
		//fprintf( fp,"@attribute is_others%d {0,1}\n", i);


    /* part of speech tagger feature */

    fprintf( fp, "pos%d: UNKNOWN,PUNC,CC,CD,DT,EX,FW,IN,JJ,JJR,JJS,MD,NN,NNP,NNPS,NNS,PDT,POS,PRP,PRP$,RB,RBR,RBS,RP,SYM,TO,UH,VB,VBD,VBG,VBN,VBP,VBZ,WDT,WP,WP$,WRB,PRP_MD,NNP_VBN\n", i );

    /* punctuation feature */

   fprintf( fp, "punctuation%d: COMMA,PERIOD,EXCLAMATION,QUESTION,SEMICOLON,COLON,PLUS,MINUS,APOSTROPHE,LEFT_PARENTHESIS,RIGHT_PARENTHESIS,DOUBLE_QUOTE,LEFT_BOX_BRACKET,RIGH_BOX_BRACKET,LEFT_CURLY_BRACKET,RIGHT_CURLY_BRACKET,EQUAL,UNDERSCORE,SLASH,BACK_SLASH,NUMBER_SIGN,LEFT_ANGLE_BRACKET,RIGHT_ANGLE_BRACKET,NOTPUNC\n", i );

    /* layout feature */
    fprintf( fp, "layout%d: Start,Mid,End,OneToken\n", i);
#if 0

    /* regular expression extractor feature */
    fprintf( fp, "re%d: START,MID,END,OTHER.\n", i);



    /* geocoder feature */
    fprintf( fp, "geocoder%d: START,MID,END,OTHER\n", i );

#endif

		/*
		fprintf( fp, "@attribute is_usstate%d {US,NO}\n", i);
		fprintf( fp, "@attribute is_sd%d {SD,NO}\n", i);
		fprintf( fp, "@attribute is_ss%d {SS,NO}\n", i);
		fprintf( fp, "@attribute is_sud%d {SU,NO}\n", i);
		*/



		/*FIPS feature */
		/*		
      		fprintf( fp, "@attribute street%d {STREET,NO}\n", i);
		fprintf( fp, "@attribute city%d {CITY,NO}\n", i);
		fprintf( fp, "@attribute state%d {STATE,NO}\n",i);
		fprintf( fp, "@attribute zip%d {ZIP, NUMBER, NO}\n",i);
		*/
    fprintf( fp, "fips%d: 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31\n", i );

		/* token line feature */
		//printf("@attribute len%d NUMERIC\n", i); // length of the token

	}



}

void C45Handler::outputC45Data()
{
	int count = (int)(ngrams->count());/* total number of ngrams */
	for ( int i=0; i<count; i++ )
	{
		outputNgramFeatures( ngrams->get(i) );
		fprintf( fp, ",");
		outputNgramClass( ngrams->get(i) );
		/* output comment of instance */
		//fprintf( fp, " %%");
		// outputNgramDesc( ngrams->get(i) );

		fprintf( fp, "\n");
	}

}

void C45Handler::outputNgramFeatures( Vector< IToken * > * ngramVector )
{
	int count = (int)(ngramVector->count()); /* num of tokens in the ngram */
	for ( int i=0; i<count; i++ )
	{ /* output each token in the ngram */
		GeoToken * token = (GeoToken*) ngramVector->get(i);
		outputTokenFeatures( token ); /* output features for the token */
		if ( count -1 != i )
		{/* not last token in the ngram */
		  fprintf( fp, ",");
		}
	}
}

void C45Handler::outputNgramDesc( Vector< IToken * > * ngramVector )
{
	int count = (int)(ngramVector->count()); /* num of tokens in the ngram */
	for ( int i=0; i<count; i++ )
	{ /* output each token in the ngram */
		GeoToken * token = (GeoToken*) ngramVector->get(i);
		fprintf( fp, "%s", token->getTokenString().c_str() );
		if ( count -1 != i )
		{/* not last token in the ngram */
		  fprintf( fp, "_");
		}

	}
}

void C45Handler::outputNgramClass( Vector< IToken * > * ngramVector )
{
	int count = (int)(ngramVector->count()); /* num of tokens in the ngram */
	assert( 1 == count % 2 && count >= 1 );
	int index = count/2;
        GeoToken * token = (GeoToken*)(ngramVector->get(index));
	fprintf( fp, "%s", token->isGeoStart()?"START": token->isGeoMid()?"MID": token->isGeoEnd()?"END": token->isGeoOther()?"OTHER":"UNKNOWN" );
	//fprintf(fp, "%s", ((GeoToken*)(ngramVector->get(index)))->getGeo() ? "GEO" : "NOTGEO" );
}


#define STREET 0x0001
#define CITY   0x0002
#define STATE  0x0004
#define ZIP    0x0008
#define NUMBER 0x0010

void C45Handler::outputFipsFeatures( GeoToken * token )
{
  assert( token );

  //  fprintf( fp,"%s,%s,%s,%s,%s", token->isStreet()?"STREET":"NO", token->isCity()?"CITY":"NO", token->isState()?"STATE":"NO", token->isZip()?"ZIP": token->isNumber() ? "NUMBER" : "NO" );
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
  fprintf( fp, ",%u", ret );
}

#define USSTATE 0x0001
#define SD      0x0002
#define SS      0x0004
#define SUD     0x0008
#define USZIP   17
#define POBOX   18
void C45Handler::outputGeographicalFeatures( GeoToken * token )
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
  fprintf( fp, ",%u", ret );
  fprintf( fp, ",%s", token->getLocalFeature()== USZIP ?"USZIP":token->getLocalFeature()==POBOX?"POBOX":"OTHER");
  
       
}

void C45Handler::outputTokenFeatures( GeoToken * token )
{


	/* word format feature */
  fprintf( fp, "%s", token->getLocalFeatureName().c_str() );

	/* geographical feature */
	outputGeographicalFeatures( token );


	/* part of speech tagging feature */
	const char * pos = token->getPos();
	assert ( pos );

	pos[0] == 0 ? fprintf( fp, ",UNKNOWN" ) : 
	  pos[0] == '"' || pos[0] == ',' || pos[0] == '.' || pos[0] == ':' || pos[0] == '\'' 
	  || pos[0] == '(' || pos[0] == ')' || pos[0]=='#'  ? fprintf( fp, ",%s", "PUNC" ) :
	  strcmp( pos, "PRP|MD" ) == 0 ?   fprintf( fp, ",%s", "PRP_MD" ) :  
	  strcmp( pos, "NNP|VBN" ) == 0 ? fprintf( fp, ",%s", "NNP_VBN" ) : fprintf( fp, ",%s", pos );

#if 0
	pos[0] == 0 ? fprintf( fp, "UNKNOWN" ) : 
	  pos[0] == '"' || pos[0] == ',' || pos[0] == '.' || pos[0] == ':' || pos[0] == '\'' 
	  || pos[0] == '(' || pos[0] == ')' || pos[0]=='#'  ? fprintf( fp, "%s", "PUNC" ) :
	  strcmp( pos, "PRP|MD" ) == 0 ?   fprintf( fp, "%s", "PRP_MD" ) :  
	  strcmp( pos, "NNP|VBN" ) == 0 ? fprintf( fp, "%s", "NNP_VBN" ) : fprintf( fp, "%s", pos );
#endif




	/* punctuation feature */
  fprintf( fp, ",%s", token->getPunctuationName().c_str() );

	/* layout feature */
	fprintf( fp, ",%s", token->isLineMid()?"Mid":token->isLineStart()?"Start":token->isLineEnd()?"End":token->isLineJustOneToken()?"OneToken":"Other");

#if 0

	/* regular expression extractor feature */
	fprintf( fp, ",%s", token->isREStart()?"START":token->isREMid()? "MID" : token->isREEnd()?"END":token->isREOther()?"OTHER":"UNKNOWN" );




	/* geocoder feature */
	fprintf( fp, ",%s", token->isGeocoderStart()?"START":token->isGeocoderMid()?"MID":token->isGeocoderEnd()?"END":token->isGeocoderOther()?"OTHER":"UNKNOWN");
#endif


	/* FIPS feature */
	outputFipsFeatures( token );

	/* token line feature */
	//fprintf( fp, ",%d", token->getTokenString().length() );

}
