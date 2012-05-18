#include "GeoToken.h"
//char * GeoToken::localFeatureName[20] = {"UNKNOWN","EMAIL","INITCAP","ALLCAPS","CONTAINDIGITS","ALLDIGITS","ACRONYM","WORD","PUNCTUATION", "TIME", "URL", "CONTRACTION", "LONELYINITIAL", "UPPERCASECHAR", "LOWERCASECHAR", "CONTAINDASH","PHONE", "USZIP", "POBOX"};  
/* Move USZIP and POBOX to be geographical features */
const char * GeoToken::localFeatureName[20] = {"UNKNOWN","EMAIL","INITCAP","ALLCAPS","CONTAINDIGITS","ALLDIGITS","ACRONYM","WORD","PUNCTUATION", "TIME", "URL", "CONTRACTION", "LONELYINITIAL", "UPPERCASECHAR", "LOWERCASECHAR", "CONTAINDASH","PHONE", "ALLDIGITS", "WORD"};  

const char * GeoToken::punctuationName[24] = {"NOTPUNC","COMMA","PERIOD","EXCLAMATION","QUESTION","SEMICOLON","COLON","PLUS","MINUS","APOSTROPHE","LEFT_PARENTHESIS","RIGHT_PARENTHESIS","DOUBLE_QUOTE","LEFT_BOX_BRACKET","RIGH_BOX_BRACKET","LEFT_CURLY_BRACKET","RIGHT_CURLY_BRACKET","EQUAL","UNDERSCORE","SLASH","BACK_SLASH","NUMBER_SIGN","LEFT_ANGLE_BRACKET","RIGHT_ANGLE_BRACKET"};

GeoToken::GeoToken() : 
  token(0), tag(0), lineStatus(0), geo(0), pos(0), geocoderTag(0),
  us(false), sd(false), ss(false), sud(false),street(false), city(false), state(false),zip(false),
  geoBoundaryFlag(GEO_OTHER),geocoderBoundaryFlag(GEO_OTHER),reBoundaryFlag(GEO_OTHER)
		  //  geocoderStart(false),geocoderMid(false),geocoderEnd(false),geocoderOther(false)
{
}

GeoToken::GeoToken( const char * newToken, int newTag, int newLineStatus, bool newUS, bool newSD, bool newSS, bool newSUD,
		    bool newStreet, bool newCity, bool newState, bool newZip, 
		    int newGeoBoundaryFlag,
		    //bool newGeocoderStart, bool newGeocoderMid, bool newGeocoderEnd, bool newGeocoderOther
		    int newGeocoderBoundaryFlag,
		    int newReBoundaryFlag

) 
: lineStatus(0),geo(0),pos(0), geocoderTag(0), us(newUS),sd(newSD), ss(newSS), sud(newSUD),street(newStreet), city(newCity), state(newState), zip(newZip),
  geoBoundaryFlag(newGeoBoundaryFlag), geocoderBoundaryFlag(newGeocoderBoundaryFlag), reBoundaryFlag(newReBoundaryFlag)
//  geocoderStart(newGeocoderStart), geocoderMid(newGeocoderMid), geocoderEnd(newGeocoderEnd), geocoderOther(newGeocoderOther)
{  
  assert ( newToken && newTag >= 0 );

  size_t len = strlen( newToken ) + 1;
  token = new char[ len ];
  memcpy( token, newToken, len );  
  tag = newTag;
  lineStatus = newLineStatus;
  //  printf("%s %s.\n", token, nameLocal[ tag ] );
 }

GeoToken::GeoToken( const char * newToken, int newTag, int newLineStatus, int newGeo, const char * newPos, int newGeocoderTag, bool newUS, bool newSD, bool newSS, bool newSUD, bool newStreet, bool newCity, bool newState, bool newZip, 
		    int newGeoBoundaryFlag,
		    int newGeocoderBoundaryFlag,
		    int newReBoundaryFlag
) : pos(0)

{
  assert ( newToken && newTag >= 0 );
  size_t len = strlen( newToken ) + 1;
  token = new char[ len ];
  memcpy( token, newToken, len );  
  tag = newTag;

  lineStatus = newLineStatus;
  geo = newGeo;
  if ( !newPos ) newPos=""; /* set to defaut " " */

  len = strlen( newPos )+1;
  pos = (char*)malloc( len );
  memcpy( pos, newPos, len );


  geocoderTag = newGeocoderTag;

  /* dictionary features */
  us = newUS;
  sd = newSD;
  ss = newSS;
  sud = newSUD;
  /* FIPS dictionary feature */
  street = newStreet;
  city = newCity;
  state = newState;
  zip = newZip;
  geoBoundaryFlag = newGeoBoundaryFlag;
  geocoderBoundaryFlag = newGeocoderBoundaryFlag;
  reBoundaryFlag = newReBoundaryFlag;

  /*  geocoderStart = newGeocoderStart;
  geocoderMid = newGeocoderMid;
  geocoderEnd = newGeocoderEnd;
  geocoderOther = newGeocoderOther;
  */
}


GeoToken::~GeoToken()
{
  if ( token )
  {
    delete[] token;
  }
  if ( pos )
  {
    free( pos );
  }
}

string GeoToken::toString()
{
  string str( token ? token : "" );
  
  if ( getGeo() > 0 )
    {
      str = string("<GEO>") + str;
    }
  /*  if ( isLineStart() )
    {
      str = string("\n") + str;
    }
  */
  return str;
}

string GeoToken::getLocalFeatureName()
{
  //  printf(" %d ", getLocalFeature() );
  //return "";
  return localFeatureName[ getLocalFeature() ];
}

int GeoToken::getPunctuationId()
{ 
    int ret = 0;/* default to be non punctuation */
    if ( 1 == strlen( token ) )
      {
	switch ( token[0] )
	  {
	  case ',':
	    ret = 1;
	    break;
	  case '.':
	    ret = 2;
	    break;
	  case '!':
	    ret = 3;
	    break;
	  case '?':
	    ret = 4;
	    break;
	  case ';':
	    ret = 5;
	    break;
	  case ':':
	    ret = 6;
	    break;
	  case '+':
	    ret = 7;
	    break;
	  case '-':
	    ret = 8;
	    break;
	  case '\'':
	    ret = 9;
	    break;
	  case '(':
	    ret = 10;
	    break;
	  case ')':
	    ret = 11;
	    break;
	  case '"':
	    ret = 12;
	    break;
	  case '[':
	    ret = 13;
	    break;
	  case ']':
	    ret = 14;
	    break;
	  case '{':
	    ret = 15;
	    break;
	  case '}':
	    ret = 16;
	    break;
	  case '=':
	    ret = 17;
	    break;
	  case '_':
	    ret = 18;
	    break;
	  case '/':
	    ret = 19;
	    break;
	  case '\\':
	    ret = 20;
	    break;
	  case '#':
	    ret = 21;
	    break;
	  case '<':
	    ret = 22;
	    break;
	  case '>':
	    ret = 23;
	    break;
	}
      }
    return ret;
}

string GeoToken::getPunctuationName()
{
  return punctuationName[ getPunctuationId() ];
}
