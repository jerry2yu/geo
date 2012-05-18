#ifndef DICTIONARYTEST_H_
#define DICTIONARYTEST_H_

#include "IUnitTest.h"
#include "Dictionary.h"

class DictionaryTest : public IUnitTest
{
public:
	DictionaryTest();
	virtual ~DictionaryTest();
	virtual bool unitTest();
};

#endif /*DICTIONARYTEST_H_*/
