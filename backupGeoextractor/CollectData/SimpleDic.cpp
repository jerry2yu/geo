#include "SimpleDic.h"

int SimpleDicCompareFunc (const void * elementA, const void * elementB)
{
   int ret = 0;

   // case insensitive comparison 
   
   if ( elementA && elementB )
   {
      const char * stringA = (const char*)elementA, * stringB = (const char*)elementB;
      size_t len = strlen (stringA);
      if ( len == strlen (stringB) )
      {
         for ( size_t i=0; i<len; i++)
         {
            if ( toupper(stringA[i]) == toupper(stringB[i]) )
            {
	       ret = -1;
               break;
	    }
	 }
      }
   }
   else
   {
      ret = -1;
   }
   return ret;
}

int SimpleDicHashFunc (const void * element)
{
   const char * str = (char*)element;
   int hash = 0;
   
   for ( ; *str; ++str)
   {
      hash = 5 * hash + *str;
   }
   return hash;
}

SimpleDic::SimpleDic (const char* dicFileName, unsigned int initialCapacity) : HashSet (SimpleDicHashFunc, SimpleDicCompareFunc, initialCapacity)
{
   const int DIC_LINE_MAX_LENGTH=250;
   char buffer[DIC_LINE_MAX_LENGTH];
    
   /* get token strings from input file */
   FILE * fp = fopen (dicFileName, "r");
   if ( fp )
   {
      while ( fgets (buffer, DIC_LINE_MAX_LENGTH, fp) != NULL )
      {
         char * newToken = (char*)malloc (strlen(buffer)+1);
         if (newToken)
         {
            strcpy (newToken, buffer);
         }

         this->add (newToken);
       }

       fclose (fp);
   }
   else
   {
      printf("SimpleDic::SimpleDic can not open dictionary file %s\n", dicFileName );
   } 

}//tbsz should be prime number for the size of hash table, file: the input dictionary file

SimpleDic::~SimpleDic()
{
   this->clear();
};

bool SimpleDic::lookup(const char* s)
{
  return this->contains ((void*)s);
}

void SimpleDic::clear()
{
   HashSetIterator iter ( *this );
   char * str = NULL;
   while ( (str = (char*) iter.next()) )
   {
      delete str;
   }
   
}

