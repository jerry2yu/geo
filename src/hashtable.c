                                                                                                                                                                                                                                                                                                                                                                                                                                     #include "hashtable.h"
void HashSetNew(hashset *h, int elemSize, int numBuckets, 
		       HashSetHashFunction hashfn, HashSetCompareFunction comparefn,
				 HashSetFreeFunction freefn)
{
	int i;
	assert(hashfn&&comparefn);
	assert(elemSize>0);
	h->ItemSize=elemSize;
	h->BucketsCount=numBuckets;
	h->hashfn=hashfn;
	h->comparefn=comparefn;
	h->freefn=freefn;
	h->ItemsCount=0;
	h->Buckets=(cvector*)malloc(sizeof(cvector)*numBuckets);
	assert(h->Buckets);
	for (i=0;i<numBuckets;i++)
		//create bucket
		VectorNew(&h->Buckets[i],h->ItemSize,h->freefn,1);
}
void HashSetDispose(hashset *h)
{
	int i;
	for (i=0;i<h->BucketsCount;i++)
	{
	
		VectorDispose(&(h->Buckets[i]));
	}
	free(h->Buckets);

}
void HashSetEnter(hashset *h, const void *elemAddr)
{
	int key,position;
	assert(h&&elemAddr);
	assert(h->Buckets);
	key=h->hashfn(elemAddr,h->BucketsCount);
	assert((key>=0)&&(key<=h->BucketsCount));
	position=VectorSearch(&h->Buckets[key],elemAddr,h->comparefn,0,false);
	if (position>=0)//element exists, replace element
		VectorReplace(&h->Buckets[key],elemAddr,position);
	else//insert element.
		VectorAppend(&h->Buckets[key],elemAddr);
	h->ItemsCount++;
}
int HashSetCount(const hashset *h)
{
	assert(h);
	return h->ItemsCount;
}
void *HashSetLookup(hashset *h, const void *elemAddr)
{
	int key,position;
	assert(h&&elemAddr);
	key=h->hashfn(elemAddr,h->BucketsCount);
	assert((key>=0)&&(key<h->BucketsCount));
	position=VectorSearch(&(h->Buckets[key]),elemAddr,h->comparefn,0,false);
	if (position>=0)
		return VectorNth(&(h->Buckets[key]),position);
	else 
		return NULL;
}

/* following structure and functions are for hash table iterator */
HashSetIterator* HashSetIteratorNew(hashset* h)
{
	HashSetIterator* iter=(HashSetIterator*)malloc(sizeof(HashSetIterator));
	assert (iter);
	assert (h);
	iter->HashSet=h;
	iter->BucketIndex=0;
	iter->ArrayIndex=-1; //first call of HashSetIteratorNext will move it to first element
	iter->Elem=NULL;
	return iter;
}


bool HashSetIteratorNext(HashSetIterator* iter)
{
	assert (iter);
	iter->ArrayIndex++;
	while (iter->BucketIndex<iter->HashSet->BucketsCount)
	{
		//if array index exceed the elements in the current bucket, move to next bucket
		if (iter->ArrayIndex<iter->HashSet->Buckets[iter->BucketIndex].ItemsCount) 
		{
			iter->Elem=VectorNth(&(iter->HashSet->Buckets[iter->BucketIndex]),iter->ArrayIndex);
			assert(iter->Elem);
			return true;

		}
		else

		{
			iter->ArrayIndex=0;
			iter->BucketIndex++;
			iter->Elem=NULL;
		}
	}
	return false;



}

void HashSetIteratorFree(HashSetIterator* iter)
{
	assert(iter);
	free(iter);
}






