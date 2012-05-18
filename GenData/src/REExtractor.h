#ifndef REEXTRACTOR_H_
#define REEXTRACTOR_H_

/**
 * The meat of Rule Based Regular Expression method implementation
 */
#include "vector.h"
#include "GeoToken.h"

class REExtractor
{
 public:
  REExtractor()
    {
    }
  ~REExtractor()
    {
    }

  struct Label
  {
    Label( int newStart, int newEnd ) : start(newStart), end(newEnd)
    {
    }
    int start;
    int end;
  };
  /**
   * extract and return the start and end postions of addresses text segment
   * @param labels - contains all the start and end position of extracted address
   */
  bool run( Vector<IToken*> & tokens, Vector<Label*> & newLabels );
 private:
  void scanSource( const char * sourceText );/* set the scan source for flex */
  Vector<int> positions;/* vector to store string postion in the source text of each token */
  Vector<Label*> * labels;
  void flexMatch();
  /**
   * save the start and end position of an address
   */
  void saveAddress( int start, int end );
  
};

#endif
