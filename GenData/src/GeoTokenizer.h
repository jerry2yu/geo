#ifndef GEOTOKENIZER_H_
#define GEOTOKENIZER_H_

#include "ITokenizer.h"
#include "GeoToken.h"

/**
 * Tokenizes text with geographical features
 * April 15, 2006. Zheyuan Yu
 * Initial Implementation
 */
class GeoTokenizer : public ITokenizer
{
  enum LineStatus { LINE_START_PENDING, LINE_START, LINE_END };
 public:

  GeoTokenizer();
  virtual ~GeoTokenizer();
  /**
   * Parses text into tokens with geographical features
   * @param fileName - name of the text file to be parsed
   * @param tokens - pointer to the vector of parsed tokens
   * @return true if parsing succeeded, otherwise false
   */
  virtual bool parse( const char * fileName, vector<IToken*> * tokens );

 private:
  enum{ GEO_START, GEO_MID, GEO_END, GEO_OTHER };

  /**
   * uses flex to parse tokens, and stores them into given token vector
   * @param tokens - pointer to the vector of parsed tokens
   * @return true if succeeded, otherwise false
   */
  bool flexMatch( vector<IToken*> * tokens );

  /**
   * sets the flag for "line start". 
   * The flag means current text is the start of a line
   */ 
  void setLineStart( )
  {
    lineStatus = 0;
  }

  /**
   * sets the flag for "line end"
   * The flag means current text is the end of a line
   */
  void setLineEnd()
  {
    lineStatus = 2;
  }

  /**
   * sets the flag for "line middle"
   * The flag means current text is the middle of a line
   */
  void setLineMid()
  {
    lineStatus = 1;
  }
  
  /**
   * whether current processing token is the first one in current line
   * @return true if it is the first token, otherwise false
   */
  bool isLineStart()
  {
    return lineStatus == 0;
  }
  /**
   * whether current processing token is the last one in current line
   * @return true if it is the last one, otherwise false
   */
  bool isLineEnd()
  {
    return lineStatus == 2;
  }

  /**
   * whether current processing token is in the middle of current line
   * @return true if it is in the middle of line, otherwise false
   */
  bool isLineMid()
  {
    return lineStatus == 1;
  }

  /**
   * whether current token is stop word ( word should be discarded )
   * @return true if it is stop word
   */  
  bool isStopWord()
  {
    return stopWord;
  }
  
  /**
   * set the flag of stop word to be the given flag
   * @param flag - true to indicate current token is stop word, otherwise false
   */
  void setStopWord ( bool flag )
  {
    stopWord = flag;
  }

  /**
   * set the geographical flag to be the given flag
   * @param flag - true to indicate current token is in the <GEO></GEO> block, otherwise false
   */
  void setGeo( bool flag )
  {
    geo = flag;
  }
  
  /**
   * gets flag of geo flag
   * @return true if current token in the <GEO></GEO> block, otherwise false
   */
  bool isGeo()
  {
    return geo;
  }

  /**
   * sets the flag of script block
   * @param flag - true if current in <script></script> block
   */
  void setScript( bool flag )
  {
    script = flag;
  }
  /**
   * gets the flag of script block
   * @return true if current in <script></script> block, otherwise false
   */
  bool isScript( )
  {
    return script;
  }
  
  /**
   * sets the flag of style block
   * @param flag - true if current in <style></style> block
   */
  void setStyle( bool flag )
  {
    style = flag;
  }
  /**
   * get the flag of style block
   * @return true if current in <style></style> block, otherwise false
   */
  bool isStyle()
  {
    return style;
  }
  
  /**
   * sets the flag of trec doc header
   * @param flag - true if current in <docHeader></docHeader> block
   */
  void setDocHeader( bool flag )
  {
    docHeader = flag;
  }
  /**
   * gets the flag of doc header block
   * @return true if current in <docHeader></docHeader> block
   */
  bool isDocHeader()
  {
    return docHeader;
  }

  /**
   * sets the flag of <head></head> block
   * @param flag true if current in <head></head> block
   */
  void setHead( bool flag )
  {
    head = flag;
  }
  /**
   * gets the flag of <head></head> block
   * @return true if current in <head></head> block, otherwise false
   */
  bool isHead()
  {
    return head;
  }

  /**
   * sets the flag of <!-- --> block
   * @param flag - true to indicates current in <!-- --> block, otherwise false
   */
  void setComment( bool flag )
  {
    comment = flag;
  }
  /**
   * gets the flag of <!-- --> block
   * @return true if current in <!-- --> block, otherwise false
   */
  bool isComment( )
  {
    return comment;
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
  int geoBoundaryFlag;/* flag indicates current token is start mid, end of an address */

  char * token;
  int lineStatus;       /* whether token is the first/middle/last( value would be 0, 1, 2 ) one in the line */
  bool stopWord;        /* flag to indicates current word is stop word if true, or word should be discarded, such as word in a comment block or <sript> block */
  bool geo;               /* flag to indicate current token is geographical token (in the block of <GEO></GEO> */
  bool script;          /* flag to indicate current token in the block of <script></script> */
  bool style;           /* flag to indicate current token is in the block of <style></style> */
  bool docHeader;       /* flag to indicate current token is in the block of trec <DOCHead></DocHead> */
  bool head;            /* flag to indicate current token is in the block of <head></head> */
  bool comment;         /* flag to indicate current token is in the block of <!--  and --> */
  bool geoStart, geoEnd;/* flag to indicate current token is start/end of geo segment */


  


};



#endif
