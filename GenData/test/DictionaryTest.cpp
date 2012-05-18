#include "DictionaryTest.h"

DictionaryTest::DictionaryTest()
{
}

DictionaryTest::~DictionaryTest()
{
}

bool DictionaryTest::unitTest()
{
	Dictionary dic;
	bool ret = true;
	if ( !( dic.isUSState( "FL" ) && dic.isUSState( "AL" ) 
	&& dic.isUSState( "WA" ) && dic.isUSState( "CA" ) ) )
	{
		ret = false;
	}
	
	if ( !( dic.isSUD( "FLOOR" ) && dic.isSUD( "APT" ) && dic.isSUD( "ROOM" )
	&& dic.isSUD( "BUILDING" ) ) )
	{
		ret = false;
	}
	
	if ( !( dic.isSS( "AVE" ) && dic.isSS( "DRIVE" ) && dic.isSS( "AVENUE" )
	&& dic.isSUD( "SUITE" ) ) )
	{
		ret = false;
	}
	
	if ( !( dic.isSD( "EAST" ) && dic.isSD( "S" ) && dic.isSD( "SOUTHEAST" )
	&& dic.isSD( "NORTHEAST" ) ) )
	{
		ret = false;
	}
	
	if ( !( dic.isOthers( "CORPORATION" ) && dic.isOthers( "UNIVERSITY" ) ) )
	{
		ret = false;
	}
	
	return ret;
}

