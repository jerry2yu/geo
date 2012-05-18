#ifndef DICTIONARY_H_
#define DICTIONARY_H_
#include "ternarySearchTree.h"
#include "GeoToken.h"
#include "Config.h"

#define DIC_LINE_MAX_LENGTH 1024
class Dictionary
{
 public:
  Dictionary();
  ~Dictionary();

  /**
   * Test whether a given string is a US state name
   * Such as AL, Florida
   * @param str - string to be tested
   * @return true if str is a US state name
   */
  bool isUSState( const char * str );

  /**
   * Test whether a given string is a Street Direction
   * Such as Ave, Stree
   * @param str - string to be tested
   * @return true if str is a street direction
   */
  bool isStreetDirection( const char * str );

  /**
   * Test whether a given string is a Street Suffix
   * Such as Ave, Stree
   * @param str - string to be tested
   * @return true if str is a street suffix
   */
  bool isStreetSuffix( const char * str );

  /**
   * Test whether a given string is a Secondary Unit Designator
   * such as Apartment, Building
   * @param str - string to be tested
   * @return true if str is a SUD
   */
  bool isSecondaryUnitDesignator( const char * str );

  /**
   * Test whether a given string is a "other" string that may help identify address
   * such as university, labortary
   * idea: construct "hot geo word dictionary" from websites, by extracting addresses from webpages,then sort the tokens by frequency
   */
  bool isOtherGeoToken( const char * str );

  void lookup( Vector<IToken*> & tokens );

 private:
  /**
   * Readin dictionary file into ternary search tree
   * @param dic - dictionary ternary serach tree
   * @param fileName - file name of the dictioary
   * @return true if operation succeeded, otherwise false
   */
  bool openDictionary( TernarySearchTree<char> &dic, const char * fileName );

  /* dictionary files */
  TernarySearchTree<char> us; /* US state name */
  TernarySearchTree<char> sd; /* street direction */
  TernarySearchTree<char> ss; /* street suffix */
  TernarySearchTree<char> sud; /* street unit disignator */
  TernarySearchTree<char> other;

  
};
#endif
