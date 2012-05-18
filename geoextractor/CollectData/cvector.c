#include "cvector.h"

/*void call_n_times( int times, void(*callback)(void*) ) {
    int i;
    for( i = 0; i != times; i++ ) {
        callback(i);
    }
}

// define the callback
void my_callback( void *arg ) {
    char *mystr = arg;
    printf( "%s\n", mystr );
}
void int_callback( int i ) {
    printf( "%d\n", i );
}
*/
void VectorNew(struct cvector *v, int elemSize, 
			   VectorFreeFunction freefn, int initialAllocation)
{
	if (initialAllocation==0)
		initialAllocation=DEFAULT_VECTOR_SIZE;
	assert(initialAllocation>=0);
	v->InitCount=initialAllocation;
	v->ItemsCount=0;
	v->ItemSize=elemSize;
	v->ArrayCount=initialAllocation;
	v->Items=(void*)malloc(elemSize*(v->ArrayCount));
	v->ItemFreeFunction=freefn;


}
void VectorDispose(cvector *v)
{
	int i;
	if (!v)
		return;
	assert(v->ItemsCount>=0);
	if (v->ItemFreeFunction!=NULL)
		//call free function for each item.
		for (i=0;i<(v->ItemsCount);i++)
			v->ItemFreeFunction(VectorNth(v,i));
	//free items
	free(v->Items);
	//free vector
	v->InitCount=0;
	v->ItemsCount=0;
	v->ArrayCount=0;
	v->ItemSize=0;
	v->ItemFreeFunction=NULL;

}
int VectorLength(const cvector *v)
{
	return v->ItemsCount;
}

void *VectorNth(const cvector *v, int position)
{
	assert(position>=0);
	//assert(position<v->ItemsCount);
	return (char*)(v->Items)+position*v->ItemSize;
}
void VectorInsert(cvector *v, const void *elemAddr, int position)
{
	assert(v);
	assert(elemAddr);
	if (position<0)
	{
		printf("vector position must be non-negative\n");
		return;
	}
	if (v->ItemsCount>=v->ArrayCount)
	{
		v->ArrayCount+=v->InitCount;
		v->Items=realloc(v->Items,v->ArrayCount*v->ItemSize);
		if (!v->Items)
		{
			printf("Out of memory\n");
			return;
		}
	}

	if (position>v->ItemsCount)//append to the end
		position=v->ItemsCount;
	/*memcpy isn’t guaranteed to work when 
	the source and destination regions overlap, (memmove makes that guarantee.)
	*/
	memmove(VectorNth(v,position+1),VectorNth(v,position),(v->ItemsCount-position)*v->ItemSize);
	memcpy(VectorNth(v,position),elemAddr,v->ItemSize);
	v->ItemsCount++;
}
void VectorAppend(cvector *v, const void *elemAddr)
{
	VectorInsert(v,elemAddr,v->ItemsCount);
}


void VectorReplace(cvector *v, const void *elemAddr, int position)
{
	// char* p=(char*)(v->Items);
	int i=0;
	if ((position>=v->ItemsCount)||(position<0))
	{
		printf("vector postion out of range\n");
		return;
	}
	for (i=0;i<v->ItemSize;i++)
		*((char*)(v->Items)+i)=*((char*)(v->Items)+position*v->ItemSize+i);
}

void VectorDelete(cvector *v, int position)
{
	if ((position<0)||(position>=v->ItemsCount))
	{
		printf("Vector position out of range\n");
		return;
	}
	if (v->ItemFreeFunction!=NULL)
		v->ItemFreeFunction(VectorNth(v,position));
	memmove(VectorNth(v,position),VectorNth(v,position+1),(v->ItemsCount-position-1)*v->ItemSize);
	v->ItemsCount--;
}
int VectorSearch(const cvector *v, const void *key, VectorCompareFunction searchfn, 
			     int startIndex, bool isSorted)
{
	int i;
	void * found;
	assert(v);
	assert(searchfn);
	if (v->ItemsCount<=0) //empty vector
		return -1;
	if ((startIndex<0)||(startIndex>=v->ItemsCount))
	{
		printf("Start Index out out range\n");
		return -1;
	}
	if (!searchfn)
	{
		printf("Please specify the compare function\n");
		return -1;
	}
	if (isSorted)//sorted, do quick sort
	{
		found = bsearch (key, VectorNth(v,startIndex), v->ItemsCount-startIndex, v->ItemSize, searchfn);
		if(found)
		return ((char*)found-(char*)v->Items)/v->ItemSize;
	}
	else
	{
		for (i=0;i<v->ItemsCount;i++)
		{
			if (searchfn(key,VectorNth(v,i))==0)
				return i;
		}
	}
	return -1; //not found
}

void VectorSort(cvector *v, VectorCompareFunction comparefn)
{
	assert(comparefn);
	qsort( v->Items, v->ItemsCount, v->ItemSize, comparefn );
}

void VectorMap(cvector *v, VectorMapFunction mapfn, void *auxData)
{
	int i;
	assert(v&&mapfn&&auxData);
	for (i=0;i<v->ItemsCount;i++)
		mapfn(VectorNth(v,i), auxData);
}
