#include "HashSet.h"

HashSet::HashSet( HashFunction hashFunction, CompareFunction compareFunction,
            unsigned int initialCapacity, float newLoadFactor )
    : count(0), hashfn( hashFunction ), comparefn( compareFunction )  
{         
   this->bucketCount = initialCapacity == 0 ? DEFAULT_INITIALCAPACITY : initialCapacity;
   this->loadFactor = newLoadFactor < 0.0 ? DEFAULT_LOADFACTOR : newLoadFactor;
   this->buckets = ( HashSetEntry ** ) calloc( this->bucketCount, sizeof ( HashSetEntry * ) );
   this->threshold = ( unsigned int )( this->bucketCount * this->loadFactor );
}

HashSet::~HashSet()
{
   this->clear();
   free( this->buckets );
}

const void * HashSet::get( void * item )
{
   unsigned int hash = this->hashfn( item );
   unsigned int index = hash % this->bucketCount;
   HashSetEntry * entry;
     
   for ( entry = buckets[index]; entry; entry = entry->next )
   {   
      if ( entry->hash == hash && comparefn( entry->item, item ) == 0)
         return ( entry->item );
   }
   return (NULL);
}

const void * HashSet::add( const void * item )
{
   unsigned int hash = hashfn( item );
   unsigned index = hash % bucketCount;
   HashSetEntry *entry;
      
   /* search the item in the set*/
   for ( entry = buckets[index]; entry; entry = entry->next )
      if ( entry->hash == hash && comparefn ( entry->item, item ) == 0 )
         return item;
    
   /* if count exceeds threshold, rehash all the items */
   if ( count >= threshold ) 
   {
      this->rehash();
      index = hash % this->bucketCount;
   }
      
   /* save the new item */ 
   entry = ( HashSetEntry * )malloc( sizeof( HashSetEntry ) );
   entry->hash = hash;
   entry->item = (void *) item;
   entry->next = buckets[index];
   this->buckets[index] = entry;
   ++this->count;
   return NULL;
}   

void HashSet::rehash()
{
   unsigned int newBucketCount = this->bucketCount + this->bucketCount + 1;   /* double the number of buckets */
   HashSetEntry ** newBuckets = ( HashSetEntry** ) calloc( newBucketCount, sizeof( HashSetEntry * ) );
   unsigned int index = this->bucketCount;
   while ( index-- ) 
   {                   
      HashSetEntry *entry, *next=NULL;
      for ( entry = this->buckets[index]; entry; entry = next )
      {
         unsigned newIndex = entry->hash % newBucketCount;
         next = ( HashSetEntry * )entry->next;
         entry->next = ( HashSetEntry * ) newBuckets[newIndex];
         newBuckets[newIndex] = entry;
      }
   }
        
   this->threshold = ( unsigned int )( newBucketCount * this->loadFactor );
   this->bucketCount = newBucketCount;
   free( this->buckets );
   this->buckets = newBuckets;
}

const void * HashSet::remove( const void * item )
{
   unsigned int hash = hashfn( item );
   unsigned int indx = hash % bucketCount;
   HashSetEntry *entry, *prev;
     
   for ( entry = this->buckets[indx], prev = NULL; entry; prev = entry, entry = entry->next ) 
   {
      if ( entry->hash == hash && comparefn( entry->item, item )==0 ) 
      {
         item = entry->item;
         if ( prev )
            prev->next = entry->next;
         else
            this->buckets[indx] = entry->next;
         free( entry );
         --this->count;
         return ( item );
      }
   }
     
   return (NULL);
}

void HashSet::clear()
{
   HashSetEntry * entry, * tmp;
   for ( unsigned int i=0; i< this->bucketCount; i++)
   {
      entry = this->buckets[i];
      while ( entry )
      {
         tmp = entry;
         entry = entry->next;
         free( tmp );
      }
   }   
}

const void * HashSetIterator::next()
{
   const void *item;
   while ( entry == NULL )
   {
      if ( this->bucketIndex-- == 0 )
         return (NULL);
      entry = buckets[ this->bucketIndex ];
   }
        
   item = entry->item;
   entry = entry->next;
     
   return item;
}

/**
 * Following codes are for testing only
 */
#if 0
#include "stdio.h"
#include <string.h>
struct Element
{
   char * key;
   int value;
   Element( const char * newKey, int newValue )
   {
      int len = strlen( newKey ) + 1;
      key = (char*) malloc( len );
      memcpy( key, newKey, len );
      value = newValue;
   }
   ~Element()
   {
      if( key)
         free( key );
   }
};

int compare (const void * a, const void * b)
{
   /*printf("comparing %s - %s.\n", ((Element*)a)->key, ((Element*)b)->key ); */
    return strcmp( ((Element*)a)->key, ((Element*)b)->key );
}

int hashfn_string( const void *elemAddr )
{
   const char * str = ((Element*)elemAddr)->key;
    unsigned int hash = 0; 
     for ( ; *str; ++str)
       hash = 5 * hash + *str;
  
   return hash;      
}

void testHashSet()
{
   HashSet h( hashfn_string, compare );
   /* test put method */
   if ( NULL == h.add( new Element( "Sunday", 0 ) ) )
      printf("element inserted, table count %d.\n", h.size() );
   else
      printf("element already existed.\n");
      
   Element elem( "Sunday", 999 );
   Element * e =(Element*)h.get( &elem );
   if ( e )
      printf("found key %s, value %d.\n", e->key, e->value );
   else
      printf("error, elem is not found.\n");
   /* test remove */
   e = (Element *)h.remove( &elem );
   if ( e )
      delete e;
   /* test put and remove */
   h.add( new Element( "Monday", 1 ) );
   h.add( new Element( "Tuesday", 2 ) );
   h.add( new Element( "Sunday", 0 ) );
   h.add( new Element( "Wednesday", 3 ) );
   h.add( new Element( "Thursday", 4 ) );
   h.add( new Element( "Friday", 5 ) );
   h.add( new Element( "Saturday", 6 ) );
   
   e = ( Element * ) h.remove( &elem ); /* remove Sunday */
   if ( e )
      delete e;
   
   /* test iterator */
   HashSetIterator iter( h );
   while ( e=(Element * ) iter.next() )
   {
      printf("key %s, value %d.\n", e->key, e->value );
      delete e;
   }
   printf("table count %d.\n", h.size() );
}

int main ()
{
   printf("Testing hash set.\n");

   for ( int i=0; i< 1;i++)
      testHashSet();   
   
   return 0;
}
#endif

