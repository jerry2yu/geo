#ifndef __Geo_Simple_Hash_H
#define __Geo_Simple_Hash_H
#include <iostream>
#include <fstream>

using namespace std;

struct name {
  char* string;
  name* next;
  double value;
};

class SimpleHash
{
	int TBLSZ;
	int no_of_errors;

	name** table;
	double error(const char* s1,const char* s2="" )
	{
		cerr<<"error: "<<s1<<s2<<'\n';
		no_of_errors++;
		return 1;
	}
	void SimpleHash::FreeTable(name* n);
  public:
    SimpleHash(int tbsz);
    ~SimpleHash();
	name* look(const char* p, int ins);
	name* insert(const char* s) {return look(s,1);}
	bool is_us_state(char* s);
};


// int dictionary(SimpleHash* h);
 void test(int intrun);
#endif
