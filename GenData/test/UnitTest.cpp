#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "DictionaryTest.h"

/**
 * method to test one component
 * @param	componet - pointer to the unit test componet
 * @param	msgOk - message to show when testing succeeded.
 * @param	msgFail - message to show when testing failed.
 */
void unitTestComponent( IUnitTest * component, const char* msgOk, const char * msgFail )
{
	printf("************************************************\n");
	assert( component && msgOk && msgFail );
	if ( component->unitTest() )
	{
		printf( "%s.\n", msgOk );
	}
	else
	{	
		printf( "%s.\n", msgFail );
		exit( 0 );
	}
}

/* main function for unit testing */
int main()
{
	for (int i=0;i<1;i++){
	DictionaryTest * dictionaryTest = new DictionaryTest();
	unitTestComponent( dictionaryTest, "Dictionary testing ok", "Dictionary testing failed" );
	delete dictionaryTest;

	}

	return 0;
}
