
#ifndef __Geo_Extract_H
#define __GEO_Extract_H


#include <ctype.h>
#include "dic.hpp"

/* dictionary class */

class SimpleExtract
{
      public:
      static SimpleDic usstates;
      static SimpleDic sud;
      static SimpleDic ss;
      static SimpleDic others;
      static SimpleDic sd;
      SimpleExtract(){};
      ~SimpleExtract(){};
	  /* weight a potential address, 
	  if return -1, not an us address, 
	  otherwise return the likely address weight.
	  */
      float is_us_address(const char* s,int len);
	  
	  /**
	  * Function is_state 
	  * Usage: is_state ("WA")
	  * lookup state dictionary to check whether token is US State name
	  * Update: Jan 13, 2005 by Zheyuan Yu
	  */

	  bool SimpleExtract::is_usstate(const char* token);
	  
	  /**
	  * Function: is_sud
	  * Usage: is_sud ("apartment")
	  * lookup dictionary to check whether a token is Secondary Unit Designator, such as APT, FLOOR
	  * Update: Jan 13, 2005 by Zheyuan Yu
	  */
	  
	  bool SimpleExtract::is_sud(const char* token);
	  
	  /**
	  * Function: is_ss
	  * Usage: is_ss ("Street")
	  * lookup dictionary to check whether a token is 
	  * Street Suffixes, such as AVE, Road
	  * Update: Jan 13, 2005 by Zheyuan Yu
	  */
	  
	  bool SimpleExtract::is_ss(const char* token);
	  
	  /**
	  * Function: is_sd
	  * Usage: is_sud ("South")
	  * lookup dictionary to check whether a token is Street Direction, such North, south
	  * Update: Jan 13, 2005 by Zheyuan Yu
	  */
	  
	  bool SimpleExtract::is_sd(const char* token);
	  
	  /**
	  * Function: is_others
	  * Usage: is_sud ("University")
	  * lookup dictionary to check whether a token is Other word, such University, POB ...
	  * Update: Jan 13, 2005 by Zheyuan Yu
	  */
	  
	  bool SimpleExtract::is_others(const char* token);



};

#endif
