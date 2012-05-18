#ifndef _HASHSET_H_
#define _HASHSET_H_

#define   DEFAULT_INITIALCAPACITY      17
#define   DEFAULT_LOADFACTOR         0.75

#include <stdlib.h>

/**
 *  Entry to hold a hash element
 */
typedef struct HASHSETENTRY
{
   unsigned int hash;   /* Hash value */
   const void *item;   /* Pointer to a item */
   struct HASHSETENTRY * next;
} HashSetEntry;

/**
 * Hash function to map item to a hash code. Ideally, the hash function would
 * distribute items as uniformly over the [0, numBuckets] range as possible.
 */
typedef int (*HashFunction)( const void *itemAddr );

/**
 * Fuction to compare two itemss
 * @return
 * - negative, if item 1 at address itemAddr1 less than item 2 at address itemAddr2.
 * - zero, equal
 * - positive, if item 1 > item 2nction
 */
typedef int (*CompareFunction)( const void *itemAddr1, const void *itemAddr);

/**
 * HashSet is hash table managing pointers to a set of items.
 * It does not store actual items but only pointers to the items, also it
 * does not release item memory, it is up to the client to manage item memory.
 * 
 * Duplicate items are not allowed, only one item will be stored. Two items are 
 * considered equal if the result of compareFunction( &item1, &item2 )==0. 
 * Hash collisions are resolved with linked lists.
 * 
 * The function hashFunction( &item ) is used to compute the hash values of items;
 * 
 * Number of buckets will be doubled each time when the total number of items in the set surpasses
 * the threshold, which is toal number of bunckets times load factor.
 *
 * Use HashSetIterator to iterate through the entire set. Use method next() to get new item in each iteration.
 * 
 * HashSet offers constant time performance for the basic operations,
 * such as add - ( may take longer, since it involves rehashing), remove, contains and size, 
 * assuming the hash function disperses the elements properly among the buckets. 
 * Iterating over this set requires time proportional to the sum of the HashSet instance's size 
 * (the number of items) plus the "capacity" (the number of buckets). 
 * Thus, it's very important not to set the initial capacity too high (or the load factor too low) if iteration performance is important.
 * 
 * Mar 12, 2006. Jerry Yu
 * Initial implementation
 */
class HashSet
{
   friend class HashSetIterator;

public:

   /**
    * Constructor
    */
   HashSet()
   {
   }

   /**
    * Constructs a new, empty set; the default is initial capacity (17) and load factor (0.75).
    * @param   hashFunction - function to get hash code for items
    * @param   compareFunction - function to compare items
    * @param   initialCapacity - initial bucket numbers
    * @param   newLoadFactor   - load factor
    */
   HashSet( HashFunction hashFunction, CompareFunction compareFunction,
      unsigned int initialCapacity = DEFAULT_INITIALCAPACITY, 
      float newLoadFactor = DEFAULT_LOADFACTOR );

   /**
    * Clear the set and free memory
    */
   ~HashSet();

   /**
    * Returns true if this set contains the specified item.
    * @param   item - item to be found
    */
   bool contains( void * item )
   {
      return get( item );
   }

   /**
    * Return a pointer to a matching item
    * @param   item - item to be found
    * @return   pointer the matching item, if item is not found, return NULL
    */
   const void * get( void * item );

   /**
    * Adds the specified item to this set if it is not already present.
    * @item   item to be added, can not be NULL
    * @return   - if there is already an "equal" item then the new item will not
    *            be stored and the function returns a pointer to an existing key
    *           - otherwise the function stores pointer to the new key and return NULL
    */
   const void * add( const void * item );

   /**
    *  Rehash (grow) HASHSET
    *  - called when loadFactor exceeds threshold
    *  - new size is 2*old_size+1
    */
   void rehash();     

   /**
    *  Removes the specified element from this set if it is present.
    *  @return   return NULL if there is no "equal" item in the set
    *          otherwise the function returns a pointer to the deleted item.
    */
   const void * remove( const void * item );

   /**
    * Removes all of the items from this set.
    */
   void clear();

   /**
    * Returns the number of elements in this set (its cardinality).
    */
   int size() const
   {
      return count;
   }

   /**
    * Returns true if this set contains no items.
    */
   bool isEmpty() const
   {
      return count == 0;
   }

private:
   
   /**
    * Pointer to an array of buckets
    */
   HashSetEntry **buckets;
   
   /**
    * Total number of buckets
    */
   unsigned int bucketCount;
   
   /**
    * count of items
    */
   unsigned int count;
   
   /**
    * loadFactor threshold
    */
   unsigned int threshold;
   
   /**
    * Current loadFactor (count/bucketCount)
    */
   float  loadFactor;
   
   /**
    * Hash function
    */
   HashFunction hashfn;
   
   /**
    * Compare function
    */
   CompareFunction comparefn;

};

/**
 * Hash Set Iterator class
 */
class HashSetIterator
{
public:

   /**
    * Cconstructor
    * @param hashSet - hash set
    */
   HashSetIterator( HashSet & hashSet ) : buckets( hashSet.buckets ), entry(0), bucketIndex( hashSet.bucketCount )
   {

   }
   
   /**
    * Destructor
    */
   ~HashSetIterator()
   {
   }
   
   /**
    * Returns the next item in the iteration.
    */
   const void * next();

private:

   /**
    * Buckets of the hash set
    */
   HashSetEntry ** buckets;

   /**
    * Hash set entry
    */
   HashSetEntry * entry;
   
   /**
    * index of the bucket
    */
   int bucketIndex;
};

#endif
