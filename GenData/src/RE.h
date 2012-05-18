/**
 * This classes represents the rule based extration method
 */
#ifndef RE_H_
#define RE_H_

#include "vector.h"
#include "GeoToken.h"
#include "REExtractor.h"

class RE
{
 public:

  RE();

  ~RE();

  /**
   * extract and label token as START, MID, END, OTHER class labels
   */
  bool run( Vector<IToken*> & tokens );

private:

  Vector<IToken*> tokens;

  Vector<REExtractor::Label*> labels;

  /**
   * extract and label token as START, MID, END, OTHER class labels
   */
  void run( Vector<IToken*> & tokens, Vector<REExtractor::Label*> & labels )
  {
  }

  REExtractor extractor;
  
};

#endif
