#ifndef _IUNITTEST_H_
#define _IUNITTEST_H_
#include <stdio.h>

/**
 * Interface for all Unit Test classes
 */
class IUnitTest
{
public:
	/**
	 * Contructor
	 */
	 IUnitTest();
	
	/**
	 * Destructor
	 */
	 virtual ~IUnitTest();
	
	/**
	 * Test the component
	 * @return	true if succeeded, otherwise false
	 */
	virtual bool unitTest() = 0;
};

#endif
