#ifndef ITOKEN_H_
#define ITOKEN_H_

#include <string.h>
#include "mystring.h"

/**
 * Interface of all token class
 * A token contains token string and its features
 * April 15, 2006. Zheyuan Yu
 */
class IToken
{
 public:
  IToken();
  virtual ~IToken();

  /**
   * gets formated printable string representing the token.
   */
  virtual string toString() = 0;

  /**
   * gets the token string that the token represents
   * @return token string
   */
  virtual string getTokenString() = 0;

  /**
   * Clones one token, however with new token string
   * @param tokenStr - cloned token will have this token string 
   * @return pointer to the new token, caller client needs to release memory of the token 
   */
  virtual IToken * clone( const string & tokenStr ) = 0;

};



#endif
