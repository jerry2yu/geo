#ifndef WEKAWORDNGRAMS_H_
#define WEKAWORDNGRAMS_H_

#include "GeoWordNgrams.h"
#include "WekaHandler.h"

class WekaWordNgrams : public GeoWordNgrams
{
 public:
  /**
   * @param newNgramN - num of ngrams 
   */
  WekaWordNgrams( int newNgramN );
  virtual ~WekaWordNgrams();
  /**
  * feed in all tokens.
  * @param tokens - vector to hold all the tokens
  */
  virtual bool addTokens(   Vector<IToken*> * tokens );
  virtual void output();
  static bool outputHeader;
 private:
  WekaHandler wekaHandler;
};

#endif
