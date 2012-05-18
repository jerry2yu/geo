#ifndef GEOTOKEN_H_
#define GEOTOKEN_H_

#include "IToken.h"
#define TOTAL_PUNCTUATIONS       23; /* total number of puctuations */
class GeoToken : public IToken
{
  enum{ GEO_START=0, GEO_MID, GEO_END, GEO_OTHER };
 public:

  GeoToken();

  /**
   * Constructor
   * @param newToken - token string
   * @param newTag - tag of the token, such as Punctuation, Phone, GEO, etc.
   * @param newUS - indicates that token is a US state name, 0: false, 1: true
   * @param newSD - indicates that token is a street direction, 0: false, 1: true
   * @param newSS - indicates that token is a street suffix, 0: false, 1: true 
   * @param newSUD - indicates that token is a secondary unit designator ( apartment, buildig ), 0: false, 1: true
   */
  GeoToken( const char * newToken, int newTag, int newLineStatus, bool newUS=false, bool newSD=false, bool newSS=false, bool newSUD=false, bool newStreet=false, bool newCity=false, bool newState=false, bool newZip = false, 
	    int newGeoBoundaryStart=GEO_OTHER,
	    int newGeocoderBoundaryFlag =GEO_OTHER,
	    int newReBoundaryFlag = GEO_OTHER
);

  /**
   * @param newPos - part of speech tag
   */

  GeoToken( const char * newToken, int newTag, int newLineStatus, int newGeo, const char * newPos, int newGeocoderTag,
	    bool newUS, bool newSD, bool newSS, bool newSUD,bool newStreet=false, bool newCity=false, bool newState=false, bool newZip=false, 
	    int geoBoundaryStart=GEO_OTHER,
	    int newGeocoderBoundaryFlag =GEO_OTHER,
	    int newReBoundaryFlag = GEO_OTHER

);

  virtual ~GeoToken();
  
  /**
   * get the string representation of the token
  * @return string
   */
  virtual string toString();

  /**
   * gets the token string that the token represents
   * @return token string
   */
  virtual string getTokenString()
  {
    return token;
  }

  /**
   * get part of speech tag
   * @return POS string pointer
   */
  virtual const char * getPos()
  {
    return pos;
  }
  
  /**
   * set part of speech tag
   * @param newPos - new POS to set
   */
  virtual void setPos( const char * newPos )
  {
    if ( pos )
      {
	free( pos );
      }
    size_t len = strlen( newPos ) + 1;
    pos = (char*)malloc( len );
    memcpy( pos, newPos, len );
  }

  /**
   * Clones one token, however with new token string
   * @param tokenStr - cloned token will have this token string 
   * @return pointer to the new token, caller client needs to release memory of the token 
   */
  virtual IToken * clone( const string & tokenStr )
  {
    return new GeoToken( tokenStr.c_str(), tag, lineStatus, geo, pos, geocoderTag, us, sd, ss, sud, street, city, state,zip, 
			 geoBoundaryFlag, geocoderBoundaryFlag, reBoundaryFlag );
			 //geocoderStart, geocoderMid, geocoderEnd, geocoderOther );
  }

  /**
   * set flag for current token to be the first one in the line
   */
  void setLineStart()
  {
    lineStatus = 0;
  }

  /**
   * whether current token is the first one in the line
   * @return true if the token is the first in the line, otherwise false
   */
  bool isLineStart()
  {
    return 0 == lineStatus;
  }

  /**
   * set flag for current token to  be in the middle of the line
   */
  void setLineMid()
  {
    lineStatus = 1;
  }

  /**
   * whether current token is in the middle of a line
   * @return true if the token is in the middle of a line
   */
  bool isLineMid()
  {
    return 1 == lineStatus;
  }
  
  /**
   * set flag for current token to  be the last one in the line
   */
  void setLineEnd()
  {
    lineStatus = 2;
  }

  /**
   * whether current token is the last one in the line
   * @return true if the token is the last one in the line, otherwise false
   */
  bool isLineEnd()
  {
    return 2 == lineStatus;
  }

  bool setLineJustOneToken()
  {
    lineStatus = 3;
	return true;
  }

  /**
   * whether current line has only one token
   */
  bool isLineJustOneToken()
  {
    return lineStatus == 3;
  }

  /**
   * gets geographical flag, which identify whether it is an address
   * @return the geo flag
   */
  int getGeo()
  {
    return geo;
  }

  /**
   * sets the geographical flag
   * @param newGeo - flag to be set
   */
  void setGeo( int newGeo )
  {
    geo = newGeo;
  }

  bool isGeocoderAddress()
  {
    return 1 == geocoderTag;
  }

  void setGeocoderAddress( int newGeocoderTag = 1 )
  {
    geocoderTag = newGeocoderTag;
  }
  /**
   * Test whether this token is a US state name
   * Such as AL, Florida
   * @return true if this token is a US state name
   */
  bool isUSState()
  {
    return us;
  }
  void setUSState( bool newValue )
  {
    us = newValue;
  }

  /**
   * Test whether this token is a street direction
   * Such as South, Northeast
   * @return true if this token is a street direction
   */
  bool isStreetDirection()
  {
    return sd;
  }
  bool setStreetDirection( bool newValue )
  {
    sd = newValue;
	return true;
  }

  /**
   * Test whether this token is a Street Suffix
   * Such as Ave, Stree
   * @return true if str is a street suffix
   */
  bool isStreetSuffix()
  {
    return ss;
  }
  bool setStreetSuffix( bool newValue )
  {
    ss = newValue;
	return true;
  }

  /**
   * Test whether this token is a Secondary Unit Designator
   * such as Apartment, Building
   * @return true if str is a SUD
   */
  bool isSecondaryUnitDesignator()
  {
    return sud;
  }
  bool setSecondaryUnitDesignator( bool newValue )
  {
    sud = newValue;
	return true;
  }

  bool isStreet()
  {
	  return street;
  }
  void setStreet( bool newValue=true )
  {
	  street = newValue;
  }
  bool isCity()
  {
	  return city;
  }
  void setCity( bool newValue=true )
  {
	  city = newValue;
  }
  bool isState()
  {
	  return state;
  }
  void setState( bool newValue=true )
  {
	  state = newValue;
  }
  void setZip ( bool newValue=true )
  {
	  zip = newValue;
  }
  bool isZip()
  {
	  return zip;
  }

  /**
   * Get the local feature
   */
  int getLocalFeature()
  {
    return tag;
  }
  /**
   * get the description of local feature
   */
  string getLocalFeatureName();

  /**
   * get the id represents punctuation that the token belongs, if return TOTAL_PUNCTUATIONS, that means this token is not punctuation
   * @return id represents punctuation.
   */
  int getPunctuationId();

  /**
   * Get the name of the puctuation that the token belongs to.
   * If this token is not a punctuation, return NOTPUNC
   * @return the name of the punctuation
   */
  String getPunctuationName();

  /* whether it a number or number contain '-' and ' ' */
  bool isNumber()
  {
	  bool ret = false;
	  char * p = token;
	  while ( *p != 0 )
	  {
		  if ( isdigit(*p) )
			  ret = true;
		  else if ( '-' != *p && ' ' != *p )
		  {	/* not digit and not hyphone */
			  ret = false;
			  break;
		  }
		  ++p;
	  }
	  return ret;
  }

  bool isGeoStart()
  {
    return GEO_START == geoBoundaryFlag;
  }

  void  setGeoStart()
  {
    geoBoundaryFlag = GEO_START;
  }

  bool isGeoEnd()
  {
    return GEO_END == geoBoundaryFlag;
  }

  void setGeoEnd( )
  {
    geoBoundaryFlag = GEO_END;
  }
  bool isGeoMid()
  {
    return GEO_MID == geoBoundaryFlag;
  }
  void setGeoMid()
  {
    geoBoundaryFlag = GEO_MID;
  }
  bool isGeoOther()
  {
    return GEO_OTHER == geoBoundaryFlag;
  }
  void setGeoOther()
  {
    geoBoundaryFlag = GEO_OTHER;
  }


  /* geocoder boundary methods */
  void setGeocoderStart()
  { 
    geocoderBoundaryFlag = GEO_START;
  }
  bool isGeocoderStart()
  {
    return GEO_START == geocoderBoundaryFlag;
  }

  void setGeocoderMid()
  {
    geocoderBoundaryFlag = GEO_MID;
  }
  bool isGeocoderMid()
  {
    return GEO_MID == geocoderBoundaryFlag;
  }

  void setGeocoderEnd()
  {
    geocoderBoundaryFlag = GEO_END;
  }

  bool isGeocoderEnd()
  {
    return GEO_END == geocoderBoundaryFlag;
  }

  void setGeocoderOther()
  {
    geocoderBoundaryFlag = GEO_OTHER;
  }
  bool isGeocoderOther()
  {
    return GEO_OTHER == geocoderBoundaryFlag;
  }






  bool isREStart()
  {
    return GEO_START == reBoundaryFlag;
  }

  void  setREStart()
  {
    reBoundaryFlag = GEO_START;
  }

  bool isREEnd()
  {
    return GEO_END == reBoundaryFlag;
  }

  void setREEnd( )
  {
    reBoundaryFlag = GEO_END;
  }
  bool isREMid()
  {
    return GEO_MID == reBoundaryFlag;
  }
  void setREMid()
  {
    reBoundaryFlag = GEO_MID;
  }
  bool isREOther()
  {
    return GEO_OTHER == reBoundaryFlag;
  }
  void setREOther()
  {
    reBoundaryFlag = GEO_OTHER;
  }



  
private:


  char * token;
  int tag; /* tag of the token, local feautre */
  int lineStatus; /* flag for start/end/middle(value of 0,1,2 ) of a line */
  int geo; /* geographical flag, default to 0 */
  char * pos; /* part of speech tag */
  int geocoderTag; /* tag by running Dan's Geocoder, 1: token belongs an address otherwise 0 */
  bool us, sd, ss, sud;
  bool street, city, state, zip;
  int geoBoundaryFlag;/* flag indicates current token is start mid, end of an address */
  int geocoderBoundaryFlag;
  int reBoundaryFlag;
  //  bool geocoderStart, geocoderMid, geocoderEnd, geocoderOther;
  /* name of features */
  
  static const char * localFeatureName[20];
  static const char * punctuationName[ 24 ]; /* should be TOTAL_PUNCTUATIONS + 1 */

  /*
  char* name_layout[20] = {"LINE_START","LINE_IN","LINE_END"};
  char* name_label[4] = {"out","begin","in","end"};
  char* name_punctuation[14]= {"COMMA","PERIOD","EXCLAMATION","QUESTION","SEMICOLON","COLON","PLUS","MINUS","APOSTROPHE","LEFT_PARENTHESIS","RIGHT_PARENTHESIS","DOUBLE_QUOTE","SPACE","NOTPUNCTUATION"};
  
  #define TOTAL_GEO_TAGS 23
  char name_label_tag[TOTAL_GEO_TAGS][20] = {"NOTGEO","GEO","SN","NS","UN","UNIT","ST","STREET","SD","FL","CITY","STATE","COUNTRY","POB","PON","POS","POSN","ZIP","NAME","C","BUILDING","BN","OTHER"};

  */


};

#endif
