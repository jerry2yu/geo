#ifndef C45WORDNGRAMS_H_
#define C45WORDNGRAMS_H_

#include "GeoWordNgrams.h"
#include "C45Handler.h"

class C45WordNgrams : public GeoWordNgrams
{
 public:
  /**
   * @param newNgramN - num of ngrams 
   */
  C45WordNgrams( int newNgramN );
  virtual ~C45WordNgrams();
  /**
  * feed in all tokens.
  * @param tokens - vector to hold all the tokens
  */
  virtual bool addTokens(   Vector<IToken*> * tokens );
  virtual void output();
  static bool outputHeader;
 private:
  C45Handler c45Handler;
};

#endif
