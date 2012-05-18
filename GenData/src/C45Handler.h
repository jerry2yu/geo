#ifndef C45HANDLER_H_
#define C45HANDLER_H_
#include "GeoToken.h"
#include "vector.h"
/**
 * Handler class for output Weka data files
 */
class C45Handler
{
 public:
  C45Handler( );
  ~C45Handler();
  /**
   * Output C45 data file with word ngram features
   */
  void outputWordNgrams( Vector< Vector< IToken* > * > * newNgrams, int newNumOfNgrams, bool outputHeader );

  void outputCharNgrams()
  {
  }
  
    /**
     * Output C45 header
     */
  void outputC45Header();
   
  /**
   * output data section of C45 file
   */
  void outputC45Data();

  /**
   * output features for given ngram
   * @param ngramVector - vector represents a ngram
   */
  void outputNgramFeatures( Vector< IToken * > * ngramVector );

  /**
   * output ngram description for given ngram ( one ngram contains n tokens )
   * @param ngramVector - vector represents a ngram 
   */
  void outputNgramDesc( Vector< IToken * > * ngramVector );

  /**
   * output class( GEO/NOTGEO-address/notaddress ) for the given ngram
   * @param ngramVector - vector represents a ngram
   */
  void outputNgramClass( Vector< IToken * > * ngramVector );

  /**
   * Output features for given token
   * @param token - one token
   */

  inline void outputTokenFeatures( GeoToken * token );

  inline void outputFipsFeatures( GeoToken * token );
  inline void outputGeographicalFeatures( GeoToken * token );

 private:
  Vector< Vector< IToken* > * > * ngrams;
  int numOfNgrams;
  //  int slidingWindowSize;
  FILE * fp;    /* file for output header or data files */
  
};
#endif
