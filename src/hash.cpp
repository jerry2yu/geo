#include "hash.hpp"
/* input: size of the hash table 
   best to be a prime number */
SimpleHash::SimpleHash(int tbsz)
{
  TBLSZ=tbsz;
  table = new name*[TBLSZ];
  for (int i=0;i<TBLSZ;i++)
  table[i]=NULL;

}
void SimpleHash::FreeTable(name* n)
{
name* nn=n;
name* nnn=n;
while (nn!=NULL)
{
  nnn=nn->next;
//  cout<<", "<<nn->string;
  delete[] nn->string;
  delete nn;
  nn=nnn;
}
}
SimpleHash::~SimpleHash()
{
   for (int i=0;i<TBLSZ;i++)
   {
     if (table[i]==NULL)
        continue;
 //    cout<<endl<<"delete table "<<i<<": ";
     FreeTable(table[i]);
   }
delete[]  table;
}
name* SimpleHash::look(const char* p, int ins=0)
{
	int ii=0, a=127;
	const char* pp=p;
	//while (*pp) (ii==ii<<1^*pp++;
        for (;*pp!=0;pp++)
           ii=(a*ii+*pp)%TBLSZ;
	if (ii<0) ii=-ii;
	ii%=TBLSZ;

	for (name* n=table[ii]; n; n=n->next) //search
		if (strcmp(p,n->string)==0) return n;
	
	if (ins==0){
	   //error(p," name not found");
	   return NULL;

	}
        //insert 
	name* nn= new name;
	nn->string=new char[strlen(p)+1];
	strcpy(nn->string,p);
	nn->value=1;
	nn->next=table[ii];
	table[ii]=nn;
	//cout<<endl<<"token "<<p<<" inserted at "<<ii;
	return nn;
}



/*void test(int intrun)
{
        //initialize the hash table to size of a prime number, e.g. 541,7919
     //   cout<<"start run "<<intrun<<endl;
        SimpleHash h(3);
        //for (int i=0;i<100000;i++)
	dictionary(&h);
//	for (int i=0;i<1000;i++)
//	h.look("CAN");
	
//	cout<<h.look("CHN")->value<<endl;
	// insert pre-defined names;
//	h.insert ("pi")->value=3.14;
//	h.insert ("e")->value=2.71;

//	cout<<h.look("pi")->value<<endl;
//	cout<<h.look("e")->value<<endl;
	if(h.look("WA")==NULL)
	  cout<<"WA not found"<<endl;
	  else
	  cout<<"WA found"<<endl;


}*/

