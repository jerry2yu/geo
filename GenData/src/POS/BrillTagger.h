#ifndef BRILLTAGGER_H_
#define BRILLTAGGER_H_
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <malloc.h>
#include "../vector.h"
#include "HashSet.h"
#include "../GeoToken.h"

#define LEXICON    "POS/Bin_and_Data/LEXICON"
#define BIGRAMS    "POS/Bin_and_Data/BIGRAMS"
#define LRULEFILE  "POS/Bin_and_Data/LEXICALRULEFILE"
#define CRULEFILE  "POS/Bin_and_Data/CONTEXTUALRULEFILE"

#define MAXLINELEN 5000
#define MAXTAGLEN 256  /* max char length of pos tags */
#define MAXWORDLEN 256 /* max char length of words */
#define MAXAFFIXLEN 5  /* max length of affixes being considered */
#define RESTRICT_MOVE 1   /* if this is set to 1, then a rule "change a tag */
/* from x to y" will only apply to a word if:
a) the word was not in the training set or
b) the word was tagged with y at least once in
the training set  
When training on a very small corpus, better
performance might be obtained by setting this to
0, but for most uses it should be set to 1 */




#include <stdlib.h> /* for atoi */



class BrillTagger
{
public:

	BrillTagger( const char * lexiconFileName = LEXICON,
		     const char * bigramFileName =  BIGRAMS, 
		     const char * lRuleFileName = LRULEFILE,
		     const char * cRuleFileName = CRULEFILE );
	~BrillTagger();

	/**
         * Tag the passed tokens
         * @param tokens - vector of tokens that need to be part of speech tagged
         */
	void runTagger( Vector<IToken*> & tokens );



	struct Element
	{
		char * key;
		char * value;
		Element( char * newKey, char* newValue ) : key( newKey ), value( newValue )
		{
		}
		~Element()
		{
		}
	};

	static int comparefn(const void * a, const void * b)
	{
		/*printf("comparing %s - %s.\n", ((Element*)a)->key, ((Element*)b)->key ); */
		return strcmp( ((Element*)a)->key, ((Element*)b)->key );
	}

	static int hashfn_string( const void *elemAddr )
	{
		const char * str = ((Element*)elemAddr)->key;
		unsigned int hash = 0; 
		for ( ; *str; ++str)
			hash = 5 * hash + *str;

		return hash;   	
	}


private:

	/**
         * Reading all data files
         */
	void init ( const char * lexiconFileName, const char * bigramFileName, const char * lRuleFileName, const char * cRuleFileName );

	/**
	* clear all the vectors and hashsets
	*/
	void clear();

	/**
	 * star stage to tag tokens
         */
	void startStateTagger( Vector<IToken*> & tokens );

	/**
	 * end stage tagger
	 * @param tokens - vector of tokens that have been tagged by start state tagger 
	 * @return true if succeeded, otherwise false
	 */
	void finalStateTagger(Vector<IToken*> & tokens );

	/**
	* find lexicon in the lexicon hashset
	* @return NULL if not found, otherwise return pointer to the found element
	*/
	Element * getLexicon( const char *  key )
	{
	  Element elem( (char*)key, 0 );
		return (Element*)lexiconHash.get( &elem );
	}

	/**
	* find good right rule in the hashset
	* @return NULL if not found, otherwise return pointer to the found element
	*/
	Element * getGoodRight( char *  key )
	{
		Element elem( key, 0 );
		return (Element*)goodRightHash.get( &elem );
	}

	/**
	* find seen taggin in the hashset
	* @return NULL if not found, otherwise return pointer to the found element
	*/
	Element * getSeenTagging( char *  key )
	{
		Element elem( key, 0 );
		return (Element*)seenTaggingHash.get( &elem );
	}

	/**
	* find good left rule in the hashset
	* @return NULL if not found, otherwise return pointer to the found element
	*/
	Element * getGoodLeft( char *  key )
	{
		Element elem( key, 0 );
		return (Element*)goodLeftHash.get( &elem );
	}

	/*	void SetTagVal (Darray tagVal, Darray freeptrs, int count, char* val, 
	int* first)
	{
	// Benjamin Han: add val for later destruction
	if (*first) 
	{
	Darray_addh(freeptrs,val);
	*first=0;
	}
	Darray_set(tagVal,count,val);
	}
	*/


	HashSet lexiconHash, goodRightHash, goodLeftHash, seenTaggingHash;

	Vector<char*> lRules, cRules, bigrams;

	void change_the_tag(char **theentry,char * thetag,int theposition)

	{
		char            staart[] = "STAART";
		char* oldTag=theentry[theposition];

		if (oldTag!=staart) free(oldTag);
		theentry[theposition] = mystrdup(thetag);
	}

	char **perl_split( char *buf )
	{
		char **return_buf;
		int cntr = 0;
		char *temp,*temp2;

		temp2 = (char *)malloc(sizeof(char)*(1+strlen(buf)));
		while(*buf == ' ' || *buf == '\t') ++buf;
		strcpy(temp2,buf);
		return_buf = (char **) malloc(sizeof(char *) * ((numspaces(temp2)+1) + 2));
		return_buf[cntr++] = (char *)strtok(temp2," \t");
		while ( ( temp = strtok(NULL," \t") ) ) 
			if (temp != NULL) {
				return_buf[cntr] = temp;
				++cntr;}
			return_buf[cntr] = NULL;
			return(return_buf); 
	}


	// THIS IS REALLY STRANGE! Brill allocates two additional blocks, and
	// use one of them to store NULL?
	char **perl_split_independent( char * buf)
	{
		char **return_buf;
		int cntr = 0;
		char *temp;

		while(*buf == ' ' || *buf == '\t') ++buf;
		return_buf = (char **) malloc(sizeof(char *) * (numspaces(buf)+3));

		return_buf[cntr++] = (char *)mystrdup((char *)strtok(buf," \t"));
		while ((temp = (char *)strtok(NULL,"\t ")) != NULL) {
			return_buf[cntr] =(char *)mystrdup(temp);
			++cntr;
		}
		return_buf[cntr] = NULL;

		return(return_buf); 
	}


	int numspaces(char* buf)
	{
		size_t count;
		int tot = 0;
		for (count = 0; count < strlen(buf); ++count) 
			if (buf[count]==' ')
				++tot;
		return tot; 
	}

	char *mystrdup( const char * thestr )
	{
		size_t len = strlen( thestr ) + 1;
		char * dupStr = (char*) malloc( len );
		memcpy( dupStr, thestr, len );
		return dupStr;
		// return( (char *)strcpy((char *)malloc((strlen(thestr)+1)*(sizeof(char))),thestr));
	}

	int not_just_blank( char * thestr )
		/* make sure not just processing a no-character line */
	{ 
		char *thestr2;
		thestr2 = thestr;
		while(*thestr2 != '\0') {
			if (*thestr2 != ' ' && *thestr2 != '\t' && *thestr2 != '\n'){
				return(1); }
			++thestr2;
		}
		return(0);
	}


	int num_words( char * thestr )
	{
		int count,returncount;

		returncount=0;
		count=0;
		while (thestr[count] != '\0' && (thestr[count] == ' ' 
			|| thestr[count] == '\t')) ++count;
		while (thestr[count++] != '\0') {
			if (thestr[count-1] == ' ' || thestr[count-1] == '\t') {
				++returncount;
				while (thestr[count] == ' ' || thestr[count] == '\t')
					++count;
				if (thestr[count] == '\0') --returncount;
			}
		}
		return(returncount);
	}

};

#endif

