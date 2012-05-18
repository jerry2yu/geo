#include "GenData.h"
int totalTokenCount = 0;
int totalGeoTokenCount = 0;
bool currentIsGeocoderAddress=false;/*current token belongs to a geocoder recognized address */
int totalAddressCount = 0;
int totalGeocoderAddressCount = 0; /* total number of addresses the recognized by geo coder */
int geocoderTruePositive = 0; /* number of address corrected extracted by geocoder */
bool geocoderStartMatch = false;

bool outputC45Header = false;
//#define SHOW_STATISTIC
//#define SHOW_TEXT
GenData::GenData()
{
}

GenData::~GenData()
{
}

bool GenData::process( const char * fileName, int ngramNumber )
{
  assert( fileName );
  //  printf( "processing file %s.\n", fileName );
  /* tokenize the file */
  GeoTokenizer geoTokenizer;
  tokens.clear();
  if ( !geoTokenizer.parse( fileName, &tokens ) )
  {
	  fprintf( stderr, "Can not find file %s.\n", fileName );
	  return false;
  }
  //printf("total tokens returned %d.\n", tokens.count() );

  /* lookup geo dictionary, and tag the tokens */
  dic.lookup( tokens );
  
  /* Part of tagging of the text */
  brillTagger.runTagger( tokens );
  
  /* Tag the addresses in the text with Dan's geocoder */
  //geoCoder.run( tokens );
  geoCoder.runFIPS( tokens );

  /* Rule Based Extraction based Regular Expression */
  //  re.run( tokens );
  
  /*  WekaWordNgrams wekaWordNgrams( ngramNumber );
  wekaWordNgrams.addTokens( &tokens );
  wekaWordNgrams.output();
  */
  C45WordNgrams c45WordNgrams( ngramNumber );
  c45WordNgrams.addTokens( &tokens );
  c45WordNgrams.output();
  
  /*GeoCharNgrams geoCharNgrams( ngramNumber );
  geoCharNgrams.addTokens( &tokens );
  geoCharNgrams.output();
  */

  clear();
  return true;

}

void GenData::clear()
{
 int count = (int)(tokens.count());
 totalTokenCount+=count;
 for ( int i=0; i<count; i++)
  {
    GeoToken * token = (GeoToken*)tokens[i];

#ifdef SHOW_STATISTIC
    /* statistics */
    if (token->getGeo() )
      {
	++totalGeoTokenCount;
      }
    if ( token->isGeoStart() )
      {
	++totalAddressCount;
      }
	
    if (token->isGeocoderAddress() )
      {
	if ( !currentIsGeocoderAddress )
	  {
	    currentIsGeocoderAddress = true;
	    ++totalGeocoderAddressCount;
	    //  printf("\n");
	  }
	//printf("%s ", token->getTokenString().c_str() );
      }
    else
      { 
        currentIsGeocoderAddress = false;
      }

    /* get geocoder true positive */
    if ( token->isGeoStart() )
      {
	geocoderStartMatch = token->isGeocoderStart();
      }
    if ( token->isGeoEnd() )
      {
	
	if ( token->isGeocoderEnd() )
	  {
	    ++geocoderTruePositive;
	  }
	geocoderStartMatch = false;
      }

#endif
    //printf("%s/%s/%s ", token->getTokenString().c_str(), token->getPos(),token->isGeocoderAddress()?"GC":"NA" );
      /* printf("%s %s %s %s %s %s\n", token->getTokenString().c_str(), token->getLocalFeatureName().c_str(),
	   token->isUSState()?"US":"NO", token->isStreetDirection()?"SD":"NO",
	   token->isStreetSuffix()?"SS":"NO", token->isSecondaryUnitDesignator()?"SU":"NO" );
      */
#ifdef SHOW_TEXT
    //printf("%s\n", token->getTokenString().c_str()?token->getTokenString().c_str() : "NULL" );
    if ( token->isGeoStart() )
      printf("\n");
    if ( token->getGeo() )
      printf("%s ", token->getTokenString().c_str() );

    //    printf("%d - %s - %s %s\n", i, token->getTokenString().c_str(), token->isGeoOther()?"OTHER":token->isGeoMid()?"MID":token->isGeoStart()?"START":token->isGeoEnd()?"END":"UNKNOWN", token->getGeo() ? "GEO":"NO" );
     /*    if ( token->isLineEnd() )
      {
	printf("\n\n");
      }
     */
#endif

   delete token;
 

   }
 
}
#include "Config.h"
//#include <fstream>
int main( int argc, char ** argv )
{
    char fileName[ MY_PATH_MAX ];
    if (argc < 2) 
    {
      fprintf( stderr, "\nOOPS!  This program takes one argument which is the file list\n");
      fprintf( stderr, "Usage: %s fileList n header|data\n", argv[0] ); 
      return 0;
    } 

    int ngramNumber = NGRAM_NUM_DEFAULT;
    if (argc>2)
    {
	ngramNumber = atoi(argv[2]);
    }
    if ( argc>3 )
      {
	outputC45Header = strcmp( argv[3], "header" ) == 0 ? true : false;
      }


	/* reading list of file names */
	FILE * fileList = fopen( argv[1],"r");
	if ( NULL != fileList )
    {
      GenData genData;
	  /* read in each line */
	  while( fgets( fileName, MY_PATH_MAX, fileList ) != NULL )
	  {

	    if ( strlen( fileName ) > 0 )
	    {
	      genData.process( String(fileName).trimEnd("\n").c_str(), ngramNumber );
	    }
       }
       fclose( fileList );
    }
    else
    {
      fprintf( stderr, "Can not open file %s.\n", argv[1] );
    }
#ifdef SHOW_STATISTIC
    printf("Total token count %d\n", totalTokenCount);
    printf("Total geo token count %d\n", totalGeoTokenCount );
    printf("Total number of addresses %d\n", totalAddressCount );
    float precision = (float)geocoderTruePositive / (float) totalGeocoderAddressCount;
    float recall = (float)geocoderTruePositive / (float) totalAddressCount;
    float fmeasure = 2.0 * precision * recall / ( precision + recall );
    printf("Total number of addresses recognized by geocoder %d, True Positive %d, Precision %f Recall %f F-Measure %f\n", totalGeocoderAddressCount, geocoderTruePositive, precision, recall, fmeasure );
#endif
    return 0;
} 
