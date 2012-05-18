/* weight a potential address, 
if return 0 or negative number, not an us address, 
otherwise return the likely address weight.
*/
#include "SimpleExtract.h"
#include <iostream> 
#include "config.h"

SimpleDic SimpleExtract::usstates (US_STATES_DATA,59);
SimpleDic SimpleExtract::sud (SUD_DATA, 59);
SimpleDic SimpleExtract::ss (STREET_SUFFIXES_DATA, 701);
SimpleDic SimpleExtract::others(OTHER_DATA, 59);
SimpleDic SimpleExtract::sd(STREET_DIRECTIONS_DATA, 31);

float SimpleExtract::is_us_address(const char* s,int len)
{
//    cout<<"Searching ~"<<s<<"~"<<endl;
    char token [200];
    int weight=0;
    bool has_state=false, has_sud=false, has_ss=false, has_others=false, has_sd=false;
    /*flag for 
      has_state: contain state, 
      has_sud:  contain secondary unit designator, 
      has_ss: street suffixes 
      has_others: others such as po or box
      has_sd: street directions
      */

    char *t;
    int c=0;
    if (len<=0) return -1;//empty, not address
    //tokenize the potential address
    while (1)
    {
      t=token;
      while (*s&&isspace(*s)) {
     // cout<<"isspace ~"<<*s<<"~"<<endl;
      s++;c++;}
      while (*s&&!isspace(*(s)))
      {
      //  cout<<"not space ~"<<*s<<"~"<<endl;
        *(t++)=toupper(*(s++));
        c++;
      }
      *(t)='\0';
      //cout<<"token ~"<<token<<"~"<<endl;
      //lookup the token in dictionary
      if (usstates.lookup(token))
      {
        has_state=true;
        //cout<<token<<" is state"<<endl;
        }
      //look up the sud dictionary
      if (sud.lookup(token))
      {
        has_sud=true;
        //cout<<token<<" is sud"<<endl;
      }
      // look up the dictionary for Street Suffixes
      if (ss.lookup(token))
      {
        has_ss=true;
        //cout<<token<<" is ss"<<endl;
      }
      if (sd.lookup(token))
      {
        has_sd=true;
        //cout<<token<<" is street direction"<<endl;
      }
      //look up the others dictionary
      if (others.lookup(token))
      {
        has_others=true;
        //cout<<token<<" is others"<<endl;
      }
        //cout<<"c "<<c<<" len "<<len<<endl;
      if (c>=len)
        break;
    }
    if (has_state) weight++;
  //  else return 0;
    if (has_sud) weight++;
    if (has_ss) weight++;
    if (has_sd) weight++;
    if (has_others) weight++;
    /*if (weight>0)
      cout<<s<<" is a us address"<<endl;
    else
      cout<<s<<" not a us address"<<endl;
    */
    return weight;

}
/**
 * Function is_state 
 * Usage: is_state ("WA")
 * lookup state dictionary to check whether token is US State name
 * Update: Jan 13, 2005 by Zheyuan Yu
 */
bool SimpleExtract::is_usstate(const char* token)
{
	  if (usstates.lookup(token))
		  return true;
	  else
		  return false;
}

/**
 * Function: is_sud
 * Usage: is_sud ("apartment")
 * lookup dictionary to check whether a token is Secondary Unit Designator, such as APT, FLOOR
 * Update: Jan 13, 2005 by Zheyuan Yu
 */
bool SimpleExtract::is_sud(const char* token)
{
	  if (sud.lookup(token))
		  return true;
	  else
		  return false;
}

/**
 * Function: is_ss
 * Usage: is_ss ("Street")
 * lookup dictionary to check whether a token is 
 * Street Suffixes, such as AVE, Road
 * Update: Jan 13, 2005 by Zheyuan Yu
 */
bool SimpleExtract::is_ss(const char* token)
{
	  if (ss.lookup(token))
		  return true;
	  else
		  return false;
}

/**
 * Function: is_sd
 * Usage: is_sud ("South")
 * lookup dictionary to check whether a token is Street Direction, such North, south
 * Update: Jan 13, 2005 by Zheyuan Yu
 */

bool SimpleExtract::is_sd(const char* token)
{
	  if (sd.lookup(token))
		  return true;
	  else
		  return false;
}

/**
 * Function: is_others
 * Usage: is_sud ("University")
 * lookup dictionary to check whether a token is Other word, such University, POB ...
 * Update: Jan 13, 2005 by Zheyuan Yu
 */

bool SimpleExtract::is_others(const char* token)
{
	  if (others.lookup(token))
		  return true;
	  else
		  return false;
}


