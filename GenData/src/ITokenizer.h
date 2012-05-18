#ifndef ITOKENIZER_H_
#define ITOKENIZER_H_

#include "IToken.h"
#include "vector.h"

/**
 * Interface of tokenizers
 * April 15, 2006. Zheyuan Yu
 */
class ITokenizer
{
 public:
  ITokenizer();
  virtual ~ITokenizer();
  /**
   * Parses a text file into tokens, and save them into vector of pointers of tokens
   * @param fileName - name of the text file to be parsed
   * @param tokens - pointer to the vector of parsed token pointers
   * @return true if parsing succeeded, otherwise false
   */
  virtual bool parse( const char * fileName, vector<IToken*> * tokens ) = 0;

};
#endif
