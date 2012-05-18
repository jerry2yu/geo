#include "dic.hpp"
SimpleDic::SimpleDic(int tbsz,const char* file):SimpleHash(tbsz)
{
    const int DIC_LINE_MAX_LENGTH=250;
    const char* DICTIONARY=file;
    char buffer[DIC_LINE_MAX_LENGTH];
    ifstream from (DICTIONARY);
    if (! from.is_open()){
	cout<<"Error opening file "<<DICTIONARY<<", please check it is existed"<<endl;
	exit(1);
    }
    //cout<<"open "<<DICTIONARY<<endl;
    while (!from.eof() )
    {
	// read in each line and extract url and file id
	from.getline (buffer,DIC_LINE_MAX_LENGTH);
	if (*buffer==0)
           continue;
	//cout<<" buffer "<<buffer<<endl;
	insert(buffer);
    }
    if (from.is_open())
    from.close();

}//tbsz should be prime number for the size of hash table, file: the input dictionary file

SimpleDic::~SimpleDic()
{
};

bool SimpleDic::look(const char* s)
{
        name* n;
		// convert to uppercase. dictionary lookup is case insensitive
		int len=strlen(s);
		char* token=(char*)malloc(len+1);
		for (int i=0;i<len;i++)
			token[i] = toupper(s[i]);
		token[len]='\0';

        n=SimpleHash::look(token,0);

		free (token);
        if (n==NULL)
        {
        //cout<<"not found"<<endl;
          return false;
        }
        //cout<<"found "<<n->string<<endl;
        return true;
}


