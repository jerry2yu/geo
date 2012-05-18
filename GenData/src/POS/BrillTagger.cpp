#include "BrillTagger.h"

BrillTagger::BrillTagger( const char * lexiconFileName, const char * bigramFileName, const char * lRuleFileName, const char * cRuleFileName ) : lexiconHash( hashfn_string, comparefn ),
goodRightHash( hashfn_string, comparefn ), goodLeftHash( hashfn_string, comparefn ),
seenTaggingHash( hashfn_string, comparefn )
{
	init( lexiconFileName, bigramFileName, lRuleFileName, cRuleFileName );

}

BrillTagger::~BrillTagger()
{
	clear();
}

void BrillTagger::init ( const char * lexiconFileName, const char * bigramFileName, const char * lRuleFileName, const char * cRuleFileName )
{
	//  printf("opening %s.\n%s\n%s\n%s\n", lexiconFileName, bigramFileName, lRuleFileName, cRuleFileName );
	FILE * lexicon = fopen( lexiconFileName,"r");
	FILE * bigramFile = fopen( bigramFileName, "r" );
	FILE * lRuleFile = fopen( lRuleFileName, "r" );
	FILE * cRuleFile = fopen( cRuleFileName, "r" );
	if ( !lexicon || !bigramFile || !lRuleFile || !cRuleFile )
	{
		return;
	}


	char line[MAXLINELEN];
	char space[500];
	/*	char COMMAND[1024]; */
	char word[MAXWORDLEN],tag[MAXTAGLEN];
	/*	char bigram1[MAXWORDLEN],bigram2[MAXWORDLEN];*/
	char **perl_split_ptr,**perl_split_ptr2,*atempstr,**temp_perl_split_ptr;
	/*	char bigram_space[MAXWORDLEN*2]; */


	/* lexicon hash stores the most likely tag for all known words.
	we can have a separate wordlist and lexicon file because unsupervised
	learning    can add to wordlist, while not adding to lexicon.  For
	example, if a big    untagged corpus is about to be tagged, the wordlist
	can be extended to    include words in that corpus, while the lexicon
	remains static.    Lexicon is file of form: 
	word t1 t2 ... tn 
	where t1 is the most likely tag for the word, and t2...tn are alternate
	tags, in no particular order. */
	/* read through once to get size */
	/** 
	* Jerry Yu. April 29, 2006. we do not need to know total number of 
	* lexicons in advance, since hash set grows by itself
	*/


	/* just need word and most likely tag from lexicon (first tag entry) */
	/* Benjamin Han 100400: originally it's hinted by the # of lines in lexicon
	file */
	while( fgets(line,sizeof(line),lexicon ) != NULL) 
	{
		size_t len = strlen( line );   /* remove dos \r\n */
		if ( len > 0 && line[len - 1]=='\n' )
		{
			line[len-1]='\0';
		}
		if ( len > 1 && line[len - 2]=='\r' )
		{
			line[len-2]='\0';
		}
		if (not_just_blank(line))
		{

			sscanf(line,"%s%s",word,tag);
			//printf("lexicon word %s, tag %s.\n", word, tag );
			//Registry_add(*lexicon_hash,(char *)mystrdup(word), (char *)mystrdup(tag));
			Element * e = new Element( mystrdup(word), mystrdup(tag) );
			if ( lexiconHash.add( e ) ) /* the lexicon already existed in the table */
			{
				free( e->key );
				free( e->value );
				delete e;    
			}

		}
	}


	/* read in lexical rule file */
	while(fgets(line,sizeof(line),lRuleFile) != NULL) 
	{
		size_t len = strlen( line );   /* remove dos \r\n */
		if ( len > 0 && line[len - 1]=='\n' )
		{
			line[len-1]='\0';
		}

		if ( len > 1 && line[len - 2]=='\r' )
		{
			line[len-2]='\0';
		}
		if (not_just_blank(line))
		{


			lRules.add( mystrdup( line ) );
			//Darray_addh(*lRuleArray,mystrdup(line));
			perl_split_ptr = perl_split(line);
			temp_perl_split_ptr = perl_split_ptr;
			Element * e = 0;
			const void * addRet = 0; // return value of table adding operation
			if (strcmp(perl_split_ptr[1],"goodright") == 0) 
			{
				e = new Element( mystrdup(perl_split_ptr[0]), (char*)1 );
				addRet = goodRightHash.add( e );
			}
			else if (strcmp(perl_split_ptr[2],"fgoodright") == 0) 
			{
				e = new Element( mystrdup(perl_split_ptr[1]), (char*)1 );
				addRet =goodRightHash.add( e );
			}
			else if (strcmp(perl_split_ptr[1],"goodleft") == 0) 
			{
				e = new Element( mystrdup(perl_split_ptr[0]), (char*)1 );
				addRet = goodLeftHash.add( e );
			}
			else if (strcmp(perl_split_ptr[2],"fgoodleft") == 0) 
			{
				e = new Element( mystrdup(perl_split_ptr[1]), (char*)1 );
				addRet = goodLeftHash.add( e ); 
			}
			if ( addRet && e )    /* item already existed in the table, free the memory */
			{
				free( e->key );
				delete e;
			}


			free(*perl_split_ptr);
			free(perl_split_ptr);
		}
	}

	/* read in bigram file */
	// Benjamin Han 100400: I store the contents in bigramArray so
	// we don't have to do file IO everytime the start-state-tagger is
	// invoked.

	while(fgets(line,sizeof(line),bigramFile) != NULL)
	{
		size_t len = strlen( line );   /* remove dos \r\n */
		if ( len > 0 && line[len - 1]=='\n' )
		{
			line[len-1]='\0';
		}

		if ( len > 1 && line[len - 2]=='\r' )
		{
			line[len-2]='\0';
		}


		if(not_just_blank(line)) 
		{
			//line[strlen(line) - 1] = '\0'; /* Jerry Yu. comment: seems no use */ 
			size_t len = strlen( line ) + 1;
			atempstr = (char *)malloc( len );
			memcpy( atempstr, line, len );
			//printf("%s.\n", atempstr );
			//Darray_addh(*bigramArray,atempstr);
			bigrams.add( atempstr );

		}
	}

	fseek(lexicon, (long)0 , SEEK_SET);

	/* read in the lexicon for the final-state-tagger */
	//Registry_size_hint(*seenTagging,numLexiconEntries);

	// Benjamin Han 100500: MISSING RESTRICT_MOVE section?
	// Answer: Brill used registry WORDS while I use lexicon_hash to replace
	//         his WORDS (see POST::Run) - the only difference is in WORDS 
	//         every value is 1 while in lexicon_hash a values is the first 
	//         tag following the word in the lexicon file.

	while(fgets(line,sizeof(line),lexicon) != NULL) 
	{
		size_t len = strlen( line );   /* remove dos \r\n */
		if ( len > 0 && line[len - 1]=='\n' )
		{
			line[len-1]='\0';
		}

		if ( len > 1 && line[len - 2]=='\r' )
		{
			line[len-2]='\0';
		}

		if (not_just_blank(line)) {

			perl_split_ptr = perl_split(line);
			perl_split_ptr2 = perl_split_ptr;
			++perl_split_ptr;
			while(*perl_split_ptr != NULL) 
			{
				//printf("line %s.\n", line );
				sprintf(space,"%s %s",*perl_split_ptr2,*perl_split_ptr);
				//printf( "Space %s End Space\n", space );
				Element * e = new Element( mystrdup( space ), (char*) 1 );
				if ( seenTaggingHash.add( e ) )
				{  /* item already existed in the table */
					free( e->key );
					delete e;
				}

				//Registry_add(*seenTagging,mystrdup(space),(char *)1);
				++perl_split_ptr; 
			}
			free(*perl_split_ptr2);
			free(perl_split_ptr2);
		}
	}


	// read in contextual rule
	while(fgets(line, sizeof(line), cRuleFile) != NULL)
	{
		size_t len = strlen( line );   /* remove dos \r\n */
		if ( len > 0 && line[len - 1]=='\n' )
		{
			line[len-1]='\0';
		}

		if ( len > 1 && line[len - 2]=='\r' )
		{
			line[len-2]='\0';
		}
		if (not_just_blank(line)) 
		{
			cRules.add( mystrdup( line ) );
			//Darray_addh(*cRuleArray,mystrdup(line));
		}
	}

	fclose( lexicon );
	fclose( bigramFile );
	fclose( lRuleFile );
	fclose( cRuleFile );




}


void BrillTagger::clear()
{
	int count = (int)( lRules.count() );
	for ( int i=0; i<count; i++ )
	{
		//printf("lRule %s.\n", lRules[i] );
		free( lRules[i] );
	}
	count = (int)(cRules.count());
	for ( int i=0; i<count; i++ )
	{
		//printf("cRule %s.\n", cRules[i] );
		free( cRules[i] );
	}
	count = int( bigrams.count() );
	for ( int i=0; i<count; i++ )
	{
		//printf("bigram %s.\n", bigrams[i] );
		free( bigrams[i] );
	}


	/* test iterator */
	Element * e;
	HashSetIterator iter( lexiconHash );
	//printf("table count %d.\n", lexiconHash.size() );
	int i=0;
	while ( e=(Element * ) iter.next() )
	{
		//printf("key %s, value %s.\n", e->key, e->value );
		free(e->key);
		free(e->value);
		delete e;
	}

	HashSetIterator goodRightIter( goodRightHash );
	//printf("good right rule count %d.\n", goodRightHash.size() );
	while ( e=(Element * ) goodRightIter.next() )
	{
		//printf("key %s, value %s.\n", e->key, e->value );
		free(e->key);
		delete e;
	}

	HashSetIterator goodLeftIter( goodLeftHash );
	//printf("good left rule count %d.\n", goodLeftHash.size() );
	while ( e=(Element * ) goodLeftIter.next() )
	{
		//printf("key %s, value %s.\n", e->key, e->value );
		free(e->key);
		delete e;
	}

	HashSetIterator seenTaggingIter( seenTaggingHash );
	//printf("seen tagging count %d.\n", seenTaggingHash.size() );
	while ( e=(Element * ) seenTaggingIter.next() )
	{
		//printf("key %s, value %s.\n", e->key, e->value );
		free(e->key);
		delete e;
	}

}

void BrillTagger::runTagger( Vector< IToken * > & tokens )
{
	//int count = tokens.count();
	startStateTagger( tokens );
	//printf("after start state tagger, start outputing.\n");
	for ( int i=0; i<(int)(tokens.count()); i++ )
	{
		GeoToken * token = (GeoToken*)tokens[i];
		//printf("%s/%s ", token->getTokenString().c_str(), token->getPos() );
	}
	//printf("\nend of output.\n");
	finalStateTagger( tokens );  
}

void BrillTagger::startStateTagger( Vector<IToken*> & tokens )
{
	char line[MAXLINELEN];      /* input line buffer */
	Vector<char*> tagKeys, tagValues;
	char *tempstr,*tempstr2;
	char **perl_split_ptr, **therule,**therule2;
	char bigram1[MAXWORDLEN],bigram2[MAXWORDLEN];
	char noun[10],proper[10];
	int count2,count3,rulesize,tempcount;
	char tempstr_space[MAXWORDLEN+MAXAFFIXLEN],bigram_space[MAXWORDLEN*2];

	HashSet tagHash( hashfn_string, comparefn );
	HashSet ntotHash( hashfn_string, comparefn );
	HashSet bigramHash( hashfn_string, comparefn );

	Element * e;

	/* Read in corpus to be tagged.  Actually, just record word list, */
	/* since each word will get the same tag, regardless of context. */


	int count = (int)(tokens.count() );

	for ( int i=0; i<count; i++ )
	{
		GeoToken * token = (GeoToken*)tokens[i];
		//printf("%s ", token->getTokenString().c_str() );
		if ( token->isLineEnd() )
		{
			//  printf("\n\n");
		}

	}

	for ( int i=0; i<count; i++ )
	{
		GeoToken * token = (GeoToken*)tokens[i];
		if ( NULL == getLexicon( token->getTokenString().c_str() ) )
		{
			e = new Element( mystrdup( token->getTokenString().c_str() ), NULL );
			if ( ntotHash.add( e ) )
			{	// alread existed
				//assigned[j]=0;
				free( e->key );
				delete e;
			}
		}
	}

	// Added by Benjamin Han: it's not part of Brill's algorithm.
	//                        Doe it help?
	// for the words not found in the lexicon we add the
	// surrounding bigrams into bigram_hash for the
	// good_left/right and fgood_left/right lexical rules
#if 0
	if (j!=0 && j!=temp-1)
	{
		sprintf(bigram_space,"%s %s", (char *)*temp_perl_split_ptr, (char *)*(temp_perl_split_ptr+1));
		//Registry_add( bigram_hash,mystrdup(bigram_space),(char *)1);
		e = new Element( mystrdup( bigram_space ), (char*)1 ) ;
		if ( bigramHash.add( e ) )
		{
			/* bigram already existed in the table */
			free( e->key );
			delete e;
		}

		sprintf( bigram_space,"%s %s", *( temp_perl_split_ptr - 1 ), (char *)*temp_perl_split_ptr );

		//Registry_add(bigram_hash,mystrdup(bigram_space), (char *)1);
		e = new Element( mystrdup( bigram_space ), (char*)1 );
		if ( bigramHash.add( e ) )
		{
			/* bigram already existed in the table */
			free( e->key );
			delete e;
		}

	}
	else if (j!=temp-1)
	{
		sprintf( bigram_space,"%s %s",(char *)*temp_perl_split_ptr, (char *)*(temp_perl_split_ptr+1) );
		//Registry_add(bigram_hash,mystrdup(bigram_space),(char *)1);
		e = new Element( mystrdup( bigram_space), (char*)1 );
		if ( bigramHash.add( e ) )
		{
			/* bigram already existed in the table */
			free( e->key );
			delete e;
		}

	}
	else if (j!=0)
	{
		sprintf(bigram_space,"%s %s", (char *)*(temp_perl_split_ptr-1),	(char *)*temp_perl_split_ptr );
		//Registry_add(bigram_hash,mystrdup(bigram_space), (char *)1);
		e = new Element( mystrdup( bigram_space ), (char*)1 );
		if ( bigramHash.add( e ) )
		{
			/* bigram already existed in the table */
			free( e->key );
			delete e;
		}

	}
#endif


	HashSetIterator ntotIter( ntotHash );

	while ( e=(Element * ) ntotIter.next() )
	{
		//printf("ntotHash key %s\n", e->key );
		tagKeys.add( e->key );
		tagValues.add( e->value );
		//		free(e->key);
		delete e;

	}

	// Benjamin Han: Now tag_array_val contains a bunch of (char*)1

	/*  READ IN THE BIGRAM HERE (from bigramArray) */
	// Benjamin Han: we're sure that every element in bigramArray
	// is of length > 1, as initialized in function Tagger
	int bigramCount = (int)(bigrams.count());
	for( int i=0; i < bigramCount; i++) 
	{
		sscanf( bigrams[i],"%s%s",bigram1,bigram2);
		Element elem1( bigram2, 0 );
		if ( getGoodRight( bigram1 ) && ( NULL !=  ntotHash.get( &elem1 ) )  ) 
		{
			sprintf(bigram_space,"%s %s",bigram1,bigram2);
			e = new Element( mystrdup( bigram_space), (char*)1 );
			if ( bigramHash.add( e ) )
			{
				/* bigram already existed in the table */
				free( e->key );
				delete e;
			}

		}
		Element elem2( bigram1, 0 );
		if ( getGoodLeft( bigram2 ) && ntotHash.get( &elem2 ) ) 
		{
			sprintf(bigram_space,"%s %s",bigram1,bigram2);
			//Registry_add(bigram_hash,mystrdup(bigram_space),(char *)1);
			e = new Element( mystrdup( bigram_space), (char*)1 );
			if ( bigramHash.add( e ) )
			{
				/* bigram already existed in the table */
				free( e->key );
				delete e;
			}
		}

	}


	/********** START STATE ALGORITHM
	YOU CAN USE OR EDIT ONE OF THE TWO START STATE ALGORITHMS BELOW, 
	# OR REPLACE THEM WITH YOUR OWN ************************/

	strcpy(noun,"NN");
	strcpy(proper,"NNP");

	/* UNCOMMENT THIS AND COMMENT OUT START STATE 2 IF ALL UNKNOWN WORDS
	SHOULD INITIALLY BE ASSUMED TO BE TAGGED WITH "NN".
	YOU CAN ALSO CHANGE "NN" TO A DIFFERENT TAG IF APPROPRIATE. */

	/*** START STATE 1 ***/
	/*   for (count=0; count < Darray_len(tag_array_val);++count) 
	Darray_set(tag_array_val,count,noun); */

	/* THIS START STATE ALGORITHM INITIALLY TAGS ALL UNKNOWN WORDS WITH TAG 
	"NN" (singular common noun) UNLESS THEY BEGIN WITH A CAPITAL LETTER, 
	IN WHICH CASE THEY ARE TAGGED WITH "NNP" (singular proper noun)
	YOU CAN CHANGE "NNP" and "NN" TO DIFFERENT TAGS IF APPROPRIATE.*/

	/*** START STATE 2 ***/
	int tagValueCount = (int)(tagValues.count());
	//for (count=0; count < Darray_len(tag_array_val);++count) 
	for (count=0; count < tagValueCount; ++count ) 
	{
		char * str = tagKeys[count];
		tagValues[count] = str[0] >='A' && str[0] <= 'Z' ? proper : noun;
	}

	/******************* END START STATE ALGORITHM ****************/

	Vector<char*> allocations;
	int lRuleCount = (int)(lRules.count());
	for (count=0; count < lRuleCount; ++count ) 
	{
		strcpy( line, lRules[count] );
		therule = perl_split_independent(line);
		therule2 = &therule[1];
		rulesize=0;
		perl_split_ptr = therule;
		while(*(++perl_split_ptr) != NULL) 
		{
			++rulesize;
		}

		if (strcmp(therule[1],"char") == 0) 
		{
			int tagKeyCount = (int)(tagKeys.count());
			for (count2=0; count2<tagKeyCount; ++count2) 
			{
				if ( strcmp( tagValues[count2],therule[rulesize-1] ) !=0) 
				{
					if( strpbrk( tagKeys[count2], therule[0]) != NULL) 
			  {
				  //Jerry Yu. need revisit
				  //SetTagVal(tag_array_val,freeptrs,count2,therule[rulesize-1],&first);
				  allocations.add ( ( tagValues[count2] = mystrdup(therule[rulesize-1]) ) );
			  }
				}
			}
		}
		else if (strcmp(therule2[1],"fchar") == 0) 
		{ 
			int tagKeyCount = (int)(tagKeys.count());
			for (count2=0;count2<tagKeyCount;++count2) 
			{
				if (strcmp( tagValues[count2],therule[0]) ==0) 
				{
					if( strpbrk( tagKeys[count2], therule2[0]) != NULL ) 
			  {
				  allocations.add( ( tagValues[count2] = mystrdup( therule[rulesize-1] ) ) );

			  }
				}
			}
		}
		else if (strcmp(therule[1],"deletepref") == 0) 
		{
			int tagKeyCount = (int)(tagKeys.count());
			for (count2=0; count2<tagKeyCount; ++count2) 
			{
				//if (strcmp( (char*)Darray_get(tag_array_val,count2),therule[rulesize-1]) !=0 ) 
				if (strcmp( tagValues[count2],therule[rulesize-1]) !=0 ) 
				{
					//tempstr = (char*)Darray_get(tag_array_key,count2);
					tempstr = tagKeys[count2];
					for (count3=0;count3<atoi(therule[2]);++count3) 
			  {
				  if (tempstr[count3] != therule[0][count3])
					  break;

			  }
			  if (count3 == atoi(therule[2])) 
			  {
				  tempstr += atoi(therule[2]);
				  if ( getLexicon( tempstr) != NULL ) 
				  {
					  allocations.add( ( tagValues[count2] = mystrdup( therule[rulesize-1] ) ) );
				  }
			  }
				}
			}
		}

		else if (strcmp(therule2[1],"fdeletepref") == 0) 
		{
			int tagKeyCount = (int)(tagKeys.count());
			for (count2=0;count2<tagKeyCount;++count2) 
			{
				if (strcmp( tagValues[count2],therule[0]) == 0)
				{ 
					tempstr= tagKeys[count2];
					for (count3=0;count3<atoi(therule2[2]);++count3) 
			  {
				  if (tempstr[count3] != therule2[0][count3])
					  break;
			  }
			  if (count3 == atoi(therule2[2])) 
			  {
				  tempstr += atoi(therule2[2]);
				  if ( getLexicon( tempstr ) != NULL ) 
				  {
					  allocations.add( ( tagValues[count2] = mystrdup( therule[rulesize-1] ) ) );
				  }
			  }
				}
			}
		}


		else if (strcmp(therule[1],"haspref") == 0) 
		{
			int tagKeyCount = (int)(tagKeys.count());
			for (count2=0; count2<tagKeyCount; ++count2) 
			{
				if ( strcmp( tagValues[count2],therule[rulesize-1])!=0) 
				{
					tempstr = tagKeys[count2];
					for (count3=0;count3<atoi(therule[2]);++count3) 
			  {
				  if (tempstr[count3] != therule[0][count3])
					  break;
			  }
			  if (count3 == atoi(therule[2])) 
			  {
				  allocations.add( ( tagValues[count2] = mystrdup( therule[rulesize-1] ) ) );
			  }
				}
			}
		}

		else if (strcmp(therule2[1],"fhaspref") == 0) 
		{
			int tagKeyCount = (int)(tagKeys.count());
			for ( count2=0; count2<tagKeyCount; ++count2 ) 
			{
				if ( strcmp( tagValues[count2],therule[0] ) == 0 )
				{ 
					tempstr= tagKeys[count2];
					for (count3=0;count3<atoi(therule2[2]);++count3) 
			  {
				  if (tempstr[count3] != therule2[0][count3])
					  break;
			  }
			  if (count3 == atoi(therule2[2])) 
			  {
				  allocations.add( ( tagValues[count2] = mystrdup( therule[rulesize-1] ) ) );
			  }
				}
			}
		}


		else if (strcmp(therule[1],"deletesuf") == 0) 
		{
			int tagKeyCount = (int)(tagKeys.count());
			for (count2=0;count2<tagKeyCount;++count2) 
			{
				if (strcmp( tagValues[count2], therule[rulesize-1])!=0) 
				{
					tempstr = tagKeys[count2];
					tempcount=(int)(strlen(tempstr)-atoi(therule[2]));
					for (count3=tempcount; count3<(int)strlen(tempstr); ++count3) 
			  {
				  if (tempstr[count3] != therule[0][count3-tempcount])
					  break;
			  }
			  if (count3 == strlen(tempstr)) 
			  {
				  tempstr2 = mystrdup(tempstr);
				  tempstr2[tempcount] = '\0';
				  if ( getLexicon( tempstr2) != NULL ) 

				  {
					  allocations.add( ( tagValues[count2] = mystrdup( therule[rulesize-1] ) ) );
				  }
				  free(tempstr2);
			  }
				}
			}
		}

		else if (strcmp(therule2[1],"fdeletesuf") == 0) 
		{
			int tagKeyCount = (int)(tagKeys.count());
			for (count2=0;count2<tagKeyCount;++count2) 
			{
				if ( strcmp( tagValues[count2],therule[0]) == 0)
				{ 
					tempstr=tagKeys[count2];
					tempcount=(int)(strlen(tempstr)-atoi(therule2[2]));
					for (count3=tempcount; count3<(int)strlen(tempstr); ++count3) 
			  {
				  if (tempstr[count3] != therule2[0][count3-tempcount])
					  break;
			  }
			  if (count3 == strlen(tempstr))
			  {
				  tempstr2 = mystrdup(tempstr);
				  tempstr2[tempcount] = '\0';
				  if ( getLexicon( tempstr2) != NULL ) 
				  {
					  allocations.add( ( tagValues[count2] = mystrdup( therule[rulesize-1] ) ) );
				  }
				  free(tempstr2);
			  }
				}
			}
		}
		else if (strcmp(therule[1],"hassuf") == 0) 
		{
			int tagKeyCount = (int)(tagKeys.count());
			for (count2=0;count2<tagKeyCount;++count2) 
			{
				if (strcmp( tagValues[count2],therule[rulesize-1])!=0) 
				{
					tempstr = tagKeys[count2];
					tempcount=(int)(strlen(tempstr)-atoi(therule[2]));
					for (count3=tempcount; count3<(int)strlen(tempstr); ++count3) 
			  {
				  if (tempstr[count3] != therule[0][count3-tempcount])
					  break;
			  }
			  if (count3 == strlen(tempstr)) 
			  {
				  allocations.add( ( tagValues[count2] = mystrdup( therule[rulesize-1] ) ) );
			  }
				}
			}
		}

		else if (strcmp(therule2[1],"fhassuf") == 0) 
		{
			int tagKeyCount = (int)(tagKeys.count());
			for (count2=0;count2<tagKeyCount;++count2) 
			{
				if (strcmp( tagValues[count2],therule[0]) == 0)
				{ 
					tempstr = tagKeys[count2];
					tempcount = (int)(strlen(tempstr)-atoi(therule2[2]));
					for (count3=tempcount; count3<(int)strlen(tempstr); ++count3) 
			  {
				  if (tempstr[count3] != therule2[0][count3-tempcount])
					  break;
			  }
			  if (count3 == strlen(tempstr))
			  {
				  allocations.add( ( tagValues[count2] = mystrdup( therule[rulesize-1] ) ) );
			  }
				}
			}
		}

		else if (strcmp(therule[1],"addpref") == 0) 
		{
			int tagKeyCount = (int)(tagKeys.count());
			for (count2=0;count2<tagKeyCount;++count2) 
			{
				if (strcmp( tagValues[count2],therule[rulesize-1]) == 0)
				{
					sprintf(tempstr_space,"%s%s", therule[0],tagKeys[count2] );
					if ( getLexicon( tempstr_space ) != NULL ) 
					{
						allocations.add( ( tagValues[count2] = mystrdup( therule[rulesize-1] ) ) );
					}
				}
			}
		}

		else if (strcmp(therule2[1],"faddpref") == 0) 
		{
			int tagKeyCount = (int)(tagKeys.count());
			for (count2=0;count2<tagKeyCount;++count2) 
			{
				if (strcmp( tagValues[count2],therule[rulesize-1]) == 0)
				{
					sprintf(tempstr_space,"%s%s",therule2[0], tagKeys[count2] );
					if ( getLexicon( tempstr_space ) != NULL ) 

					{
						allocations.add( ( tagValues[count2] = mystrdup( therule[rulesize-1] ) ) );

					}
				}
			}
		}


		else if (strcmp(therule[1],"addsuf") == 0) 
		{
			int tagKeyCount = (int)(tagKeys.count());
			for (count2=0;count2<tagKeyCount;++count2) 
			{
				if (strcmp( tagValues[count2],therule[rulesize-1]) !=0 )
				{
					sprintf(tempstr_space,"%s%s", tagKeys[count2], therule[0]);
					if ( getLexicon( tempstr_space ) != NULL ) 

					{
						allocations.add( ( tagValues[count2] = mystrdup( therule[rulesize-1] ) ) );
					}
				}
			}
		}


		else if (strcmp(therule2[1],"faddsuf") == 0) 
		{
			int tagKeyCount = (int)(tagKeys.count());
			for (count2=0;count2<tagKeyCount;++count2) 
			{
				if (strcmp( tagValues[count2],therule[0]) ==0 ) 
				{
					sprintf(tempstr_space,"%s%s", tagKeys[count2], therule2[0]);
					if ( getLexicon( tempstr_space ) != NULL ) 

					{
						allocations.add( ( tagValues[count2] = mystrdup( therule[rulesize-1] ) ) );
					}
				}
			}
		}


		else if (strcmp(therule[1],"goodleft") == 0) 
		{
			int tagKeyCount = (int)(tagKeys.count());
			for (count2=0;count2<tagKeyCount;++count2) 
			{
				if (strcmp( tagValues[count2],therule[rulesize-1]) !=0 ) 
				{
					sprintf(bigram_space,"%s %s", tagKeys[count2],therule[0]);
					Element elem( bigram_space, 0 );
					if ( bigramHash.get( &elem ) != NULL ) 
					{
						allocations.add( ( tagValues[count2] = mystrdup( therule[rulesize-1] ) ) );
					}
				}
			}
		}

		else if (strcmp(therule2[1],"fgoodleft") == 0) 
		{
			int tagKeyCount = (int)(tagKeys.count());
			for (count2=0;count2<tagKeyCount;++count2) 
			{
				if (strcmp( tagValues[count2],therule[0]) ==0) 
				{
					sprintf(bigram_space,"%s %s",tagKeys[count2],therule2[0]);
					Element elem( bigram_space, 0 );
					if ( bigramHash.get( &elem ) != NULL )
					{
						allocations.add( ( tagValues[count2] = mystrdup( therule[rulesize-1] ) ) );
					}
				}
			}
		}

		else if (strcmp(therule[1],"goodright") == 0) 
		{
			int tagKeyCount = (int)(tagKeys.count());
			for (count2=0;count2<tagKeyCount;++count2) 
			{
				if ( strcmp( tagValues[count2],therule[rulesize-1]) !=0) 
				{
					sprintf( bigram_space,"%s %s",therule[0],tagKeys[count2]);
					Element elem( bigram_space, 0 );
					if ( bigramHash.get( &elem ) != NULL )
					{
						allocations.add( ( tagValues[count2] = mystrdup( therule[rulesize-1] ) ) );
					}
				}
			}
		}

		else if (strcmp(therule2[1],"fgoodright") == 0) 
		{
			int tagKeyCount = (int)(tagKeys.count());
			for (count2=0;count2<tagKeyCount;++count2) 
			{
				if (strcmp( tagValues[count2],therule[0]) ==0) 
				{
					sprintf(bigram_space,"%s %s",therule2[0], tagKeys[count2]);
					Element elem( bigram_space, 0 );
					if ( bigramHash.get( &elem ) != NULL )
					{
						allocations.add( ( tagValues[count2] = mystrdup( therule[rulesize-1] ) ) );
					}
				}
			}
		}
		for ( int i=0; i<rulesize; i++ ) 
		{
			free(therule[i]);
		}

		free(therule[rulesize]);
		free(therule);
	}

	/* now go from darray to hash table */

	int tagKeyCount = (int)(tagKeys.count());
	for (count=0;count<tagKeyCount;++count) 
	{
		e = new Element( tagKeys[count], tagValues[count] );  
		if ( tagHash.add( e ) )
		{/* alread existed */
			delete e;
		}
	}

	count = (int)(tokens.count());
	for ( int i=0; i<count; i++ )
	{
		GeoToken * token = (GeoToken*)tokens[i];
		String tokenString = token->getTokenString();
		String newTag("");
		Element * elem = 0;
		int index = tokenString.indexOf( "//" );
		if ( index >=0 )
		{
			/* a word can be pretagged by putting two slashes between the */
			/* word and the tag ::  The boy//NN ate . */
			/* if a word is pretagged, we just spit out the pretagging */
			/* Jerry Yu. should do nothing here */
			//bufp = strcat(bufp, *temp_perl_split_ptr);
			//bufp = strcat(bufp, " "); 
		}
		else if ( ( elem = getLexicon( tokenString.c_str() ) ) != NULL ) 
		{
			newTag = elem->value;
		}
		else 
		{
			Element elem( (char*)(tokenString.c_str()), 0 );
			Element * e = (Element *) tagHash.get( &elem );
			if ( e )
			{
				newTag += e->value;
			}
		}
		token->setPos( newTag.c_str() );
	}
	for ( int i=0; i<(int)(tokens.count()); i++ )
	{
		GeoToken * token = (GeoToken*)tokens[i];
		//printf("%s-%s.\n", token->getTokenString().c_str(), token->getPos() );
	}

	HashSetIterator tagIter( tagHash );
	//printf("tag hashset count %d.\n", tagHash.size() );
	while ( e=(Element * )tagIter.next() )
	{
		//printf("key %s, value %s.\n", e->key, e->value );
		//free(e->key);
		delete e;
	}

	HashSetIterator bigramIter( bigramHash );
	//printf("bigram hashset count %d.\n", bigramHash.size() );
	while ( e=(Element * ) bigramIter.next() )
	{
		//printf("key %s, value %s.\n", e->key, e->value );
		free(e->key);
		delete e;
	}

	tagKeyCount = (int)(tagKeys.count());
	for (count=0;count<tagKeyCount;++count)
	{
		//printf("tagKey free %s.\n", tagKeys[count] );
		free(tagKeys[count] );
	}

	int allocationCount = (int)(allocations.count());
	//printf("count is a %d.\n", allocationCount );
	for ( int i=0; i<allocationCount; i++ )
	{
		//printf("free alloctaion %s.\n", allocations[i] );
		free( allocations[i] );
	}
}

void BrillTagger::finalStateTagger( Vector<IToken*> & tokens )
{
	char            staart[] = "STAART";
	char            **word_corpus_array,**tag_corpus_array;
	int            word_corpus_array_index,tag_corpus_array_index;
	char          **split_ptr;
	int             corpus_size;
	/* int numwords; */
	int             count,tempcount1,tempcount2,arraySize;
	char            old[MAXTAGLEN], newStr[MAXTAGLEN], when[50],
		tag[MAXTAGLEN], lft[MAXTAGLEN], rght[MAXTAGLEN],
		prev1[MAXTAGLEN], prev2[MAXTAGLEN], next1[MAXTAGLEN],
		next2[MAXTAGLEN], curtag[MAXTAGLEN],
		curwd[MAXWORDLEN],word[MAXWORDLEN],
		atempstr2[256];

	int tokenCount = (int)(tokens.count());
	arraySize = tokenCount;
	bool isLineStart = true;
	for ( int i=0; i<tokenCount; i++ )
	{
		GeoToken * token = (GeoToken*)tokens[i];
		//printf("%s ", token->getTokenString().c_str() );
		if ( isLineStart )
		{
			arraySize += 2; //first token of the line will has staart staart inserted
			isLineStart = false;
		}
		if ( token->isLineEnd() )
		{
			isLineStart = true;;
			//printf("\n\n");
		}
	}

	word_corpus_array = (char **)malloc(sizeof(char *) * arraySize);
	tag_corpus_array = (char **)malloc(sizeof(char *) * arraySize);
	word_corpus_array_index = tag_corpus_array_index = 0;

	/* read in corpus from buf.  This corpus has already been tagged (output of */
	/* the start state tagger) */

	isLineStart = true;
	for ( int i=0; i<tokenCount; i++ )
	{
		GeoToken * token = (GeoToken*)tokens[i];
		if ( isLineStart ) /* if token is line start */
		{
			word_corpus_array[word_corpus_array_index++] = staart;
			word_corpus_array[word_corpus_array_index++] = staart;
			tag_corpus_array[tag_corpus_array_index++] = staart;
			tag_corpus_array[tag_corpus_array_index++] = staart;		
			isLineStart = false;
		}
		word_corpus_array[word_corpus_array_index++] = mystrdup( token->getTokenString().c_str() );
		tag_corpus_array[tag_corpus_array_index++] = mystrdup( token->getPos() );
		if ( token->isLineEnd() )
		{
			isLineStart = true;
		}
		//printf("%s - %s.\n", token->getTokenString().c_str(), token->getPos() );
		//printf("%d - %d %x - %x word-tag: %s - %s.\n", word_corpus_array_index-1, tag_corpus_array_index-1, word_corpus_array[word_corpus_array_index-1], tag_corpus_array[tag_corpus_array_index-1], word_corpus_array[word_corpus_array_index-1], tag_corpus_array[tag_corpus_array_index-1] );
	}
	//printf("word_corpus_array_index %d, tag_corpus_array_index %d.\n", word_corpus_array_index, tag_corpus_array_index );

	// read in rule from cRuleArray, and process each rule
	corpus_size = tag_corpus_array_index - 1;

	int cRuleCount = (int)(cRules.count());

	for ( int i=0; i<cRuleCount; i++) 
	{
		split_ptr = perl_split( (char*)cRules[i] );
		strcpy(old, split_ptr[0]);
		strcpy(newStr, split_ptr[1]);
		strcpy(when, split_ptr[2]);
		if (strcmp(when, "NEXTTAG") == 0 ||
			strcmp(when, "NEXT2TAG") == 0 ||
			strcmp(when, "NEXT1OR2TAG") == 0 ||
			strcmp(when, "NEXT1OR2OR3TAG") == 0 ||
			strcmp(when, "PREVTAG") == 0 ||
			strcmp(when, "PREV2TAG") == 0 ||
			strcmp(when, "PREV1OR2TAG") == 0 ||
			strcmp(when, "PREV1OR2OR3TAG") == 0) {
				strcpy(tag, split_ptr[3]);
			} 
		else if (strcmp(when, "NEXTWD") == 0 ||
			strcmp(when,"CURWD") == 0 ||
			strcmp(when, "NEXT2WD") == 0 ||
			strcmp(when, "NEXT1OR2WD") == 0 ||
			strcmp(when, "NEXT1OR2OR3WD") == 0 ||
			strcmp(when, "PREVWD") == 0 ||
			strcmp(when, "PREV2WD") == 0 ||
			strcmp(when, "PREV1OR2WD") == 0 ||
			strcmp(when, "PREV1OR2OR3WD") == 0) 
		{	
		}
		else if (strcmp(when, "SURROUNDTAG") == 0) 
		{
			strcpy(lft, split_ptr[3]);
			strcpy(rght, split_ptr[4]);
		} 
		else if (strcmp(when, "PREVBIGRAM") == 0) 
		{
			strcpy(prev1, split_ptr[3]);
			strcpy(prev2, split_ptr[4]);
		} 
		else if (strcmp(when, "NEXTBIGRAM") == 0) 
		{
			strcpy(next1, split_ptr[3]);
			strcpy(next2, split_ptr[4]);
		}
		else if (strcmp(when,"LBIGRAM") == 0||
			strcmp(when,"WDPREVTAG") == 0) 
		{
			strcpy(prev1,split_ptr[3]);
			strcpy(word,split_ptr[4]); 	
		} 
		else if (strcmp(when,"RBIGRAM") == 0 ||
			strcmp(when,"WDNEXTTAG") == 0) 
		{
			strcpy(word,split_ptr[3]);
			strcpy(next1,split_ptr[4]); 		
		} else if (strcmp(when,"WDAND2BFR")== 0 ||
			strcmp(when,"WDAND2TAGBFR")== 0) 
		{
			strcpy(prev2,split_ptr[3]);
			strcpy(word,split_ptr[4]);
		}
		else if (strcmp(when,"WDAND2AFT")== 0 ||
			strcmp(when,"WDAND2TAGAFT")== 0) 
		{
			strcpy(next2,split_ptr[4]);
			strcpy(word,split_ptr[3]);
		}

		for (count = 0; count <= corpus_size; ++count) 
		{
			strcpy(curtag, tag_corpus_array[count]);
			if (strcmp(curtag, old) == 0) 
			{
				strcpy(curwd,word_corpus_array[count]);
				sprintf(atempstr2,"%s %s",curwd,newStr);
				if (! RESTRICT_MOVE || 
					!getLexicon( curwd ) ||
					getSeenTagging( atempstr2 ) )
				{
					if (strcmp(when, "SURROUNDTAG") == 0) 
					{
						if (count < corpus_size && count > 0) 
						{
							if ( strcmp(lft, tag_corpus_array[count - 1]) == 0 &&
								strcmp(rght, tag_corpus_array[count + 1]) == 0 )
								change_the_tag(tag_corpus_array, newStr, count);
						}
					} 
					else if (strcmp(when, "NEXTTAG") == 0) 
					{
						if (count < corpus_size) 
						{
							if ( strcmp(tag,tag_corpus_array[count + 1]) == 0 )
								change_the_tag(tag_corpus_array, newStr, count);
						}
					}  
					else if (strcmp(when, "CURWD") == 0) 
					{
						if (strcmp(word, word_corpus_array[count]) == 0)
							change_the_tag(tag_corpus_array, newStr, count);
					} 
					else if (strcmp(when, "NEXTWD") == 0) 
					{
						if (count < corpus_size) 
						{
							if ( strcmp(word, word_corpus_array[count + 1]) == 0 )
								change_the_tag(tag_corpus_array, newStr, count);
						}
					} 
					else if (strcmp(when, "RBIGRAM") == 0) 
					{
						if (count < corpus_size) 
						{
							if ( strcmp(word, word_corpus_array[count]) == 0 &&
								strcmp(next1, word_corpus_array[count+1]) == 0 )
								change_the_tag(tag_corpus_array, newStr, count);
						}
					} 
					else if (strcmp(when, "WDNEXTTAG") == 0) 
					{
						if (count < corpus_size) 
						{
							if (strcmp(word, word_corpus_array[count]) == 0 &&
								strcmp(next1, tag_corpus_array[count+1]) ==	0 )
								change_the_tag(tag_corpus_array, newStr, count);
						}
					}

					else if (strcmp(when, "WDAND2AFT") == 0) 
					{
						if (count < corpus_size-1) 
						{
							if ( strcmp(word, word_corpus_array[count]) == 0 &&
								strcmp(next2, word_corpus_array[count+2]) == 0 )
								change_the_tag(tag_corpus_array, newStr, count);
						}
					}
					else if (strcmp(when, "WDAND2TAGAFT") == 0) 
					{
						if (count < corpus_size-1) 
						{
							if (strcmp(word, word_corpus_array[count]) == 0 &&
								strcmp(next2, tag_corpus_array[count+2]) ==	0 )
								change_the_tag(tag_corpus_array, newStr, count);
						}
					}

					else if (strcmp(when, "NEXT2TAG") == 0) 
					{
						if (count < corpus_size - 1) 
						{
							if (strcmp(tag, tag_corpus_array[count + 2]) == 0)
								change_the_tag(tag_corpus_array, newStr, count);
						}
					} 
					else if (strcmp(when, "NEXT2WD") == 0) 
					{
						if (count < corpus_size - 1) 
						{
							if (strcmp(word, word_corpus_array[count + 2]) == 0)
								change_the_tag(tag_corpus_array, newStr, count);
						}
					} 
					else if (strcmp(when, "NEXTBIGRAM") == 0) 
					{
						if (count < corpus_size - 1) 
						{
							if (strcmp(next1, tag_corpus_array[count + 1]) == 0 &&	   
								strcmp(next2, tag_corpus_array[count + 2]) == 0)
								change_the_tag(tag_corpus_array, newStr, count);
						}
					} 
					else if (strcmp(when, "NEXT1OR2TAG") == 0) 	   
					{
						if (count < corpus_size) 
						{
							if (count < corpus_size-1) 
								tempcount1 = count+2;
							else 
								tempcount1 = count+1;
							if (strcmp(tag, tag_corpus_array[count + 1]) == 0 ||
								strcmp(tag, tag_corpus_array[tempcount1]) == 0)
								change_the_tag(tag_corpus_array, newStr, count);
						}
					}  
					else if (strcmp(when, "NEXT1OR2WD") == 0) 
					{
						if (count < corpus_size) 
						{
							if (count < corpus_size-1) 
								tempcount1 = count+2;
							else 
								tempcount1 = count+1;
							if	( strcmp(word, word_corpus_array[count + 1]) == 0 ||
								strcmp(word, word_corpus_array[tempcount1]) == 0)
								change_the_tag(tag_corpus_array, newStr, count);
						}
					}   
					else if (strcmp(when, "NEXT1OR2OR3TAG") == 0) 
					{
						if (count < corpus_size) 
						{
							if (count < corpus_size -1)
								tempcount1 = count+2;
							else 
								tempcount1 = count+1;
							if (count < corpus_size-2)
								tempcount2 = count+3;
							else 
								tempcount2 =count+1;
							if	(strcmp(tag, tag_corpus_array[count + 1]) == 0 ||
								strcmp(tag, tag_corpus_array[tempcount1]) == 0 ||
								strcmp(tag, tag_corpus_array[tempcount2]) == 0)
								change_the_tag(tag_corpus_array, newStr, count);
						}
					} 
					else if (strcmp(when, "NEXT1OR2OR3WD") == 0) 
					{
						if (count < corpus_size) 
						{
							if (count < corpus_size -1)
								tempcount1 = count+2;
							else 
								tempcount1 = count+1;
							if (count < corpus_size-2)
								tempcount2 = count+3;
							else 
								tempcount2 =count+1;
							if (strcmp(word, word_corpus_array[count + 1]) == 0 ||
								strcmp(word, word_corpus_array[tempcount1]) == 0 ||
								strcmp(word, word_corpus_array[tempcount2]) == 0)
								change_the_tag(tag_corpus_array, newStr, count);
						}
					}  
					else if (strcmp(when, "PREVTAG") == 0) 
					{
						if (count > 0) 
						{
							if (strcmp(tag, tag_corpus_array[count - 1]) == 0) 
							{
								change_the_tag(tag_corpus_array, newStr, count);
							}
						}
					} 
					else if (strcmp(when, "PREVWD") == 0) 
					{
						if (count > 0) 
						{
							if (strcmp(word, word_corpus_array[count - 1]) == 0) 
							{
								change_the_tag(tag_corpus_array, newStr, count);
							}
						}
					} 
					else if (strcmp(when, "LBIGRAM") == 0) 
					{
						if (count > 0) 
						{
							if ( strcmp(word, word_corpus_array[count]) == 0 &&
								strcmp(prev1, word_corpus_array[count-1]) == 0 )
								change_the_tag(tag_corpus_array, newStr, count);
						}
					}
					else if (strcmp(when, "WDPREVTAG") == 0) 
					{
						if (count > 0) 
						{
							if (strcmp(word, word_corpus_array[count]) == 0 &&
								strcmp(prev1, tag_corpus_array[count-1]) ==	0 )
								change_the_tag(tag_corpus_array, newStr, count);
						}
					}
					else if (strcmp(when, "WDAND2BFR") == 0) 
					{
						if (count > 1) 
						{
							if (strcmp(word, word_corpus_array[count]) == 0 &&
								strcmp(prev2, word_corpus_array[count-2]) == 0 )
								change_the_tag(tag_corpus_array, newStr, count);
						}
					}
					else if (strcmp(when, "WDAND2TAGBFR") == 0) 
					{
						if (count > 1) 
						{
							if (strcmp(word, word_corpus_array[count]) == 0 &&
								strcmp(prev2, tag_corpus_array[count-2]) ==	0 )
								change_the_tag(tag_corpus_array, newStr, count);
						}
					}

					else if (strcmp(when, "PREV2TAG") == 0) 
					{
						if (count > 1) 
						{
							if (strcmp(tag, tag_corpus_array[count - 2]) == 0)
								change_the_tag(tag_corpus_array, newStr, count);
						}
					} 
					else if (strcmp(when, "PREV2WD") == 0) 
					{
						if (count > 1) 
						{
							if (strcmp(word, word_corpus_array[count - 2]) == 0)
								change_the_tag(tag_corpus_array, newStr, count);
						}
					} 
					else if (strcmp(when, "PREV1OR2TAG") == 0) 
					{
						if (count > 0) 
						{
							if (count > 1) 
								tempcount1 = count-2;
							else
								tempcount1 = count-1;
							if (strcmp(tag, tag_corpus_array[count - 1]) == 0 ||
								strcmp(tag, tag_corpus_array[tempcount1]) == 0)
								change_the_tag(tag_corpus_array, newStr, count);
						}
					} 
					else if (strcmp(when, "PREV1OR2WD") == 0) 
					{
						if (count > 0) 
						{
							if (count > 1) 
								tempcount1 = count-2;
							else
								tempcount1 = count-1;
							if (strcmp(word, word_corpus_array[count - 1]) == 0 ||
								strcmp(word, word_corpus_array[tempcount1]) == 0)

							{
								change_the_tag(tag_corpus_array, newStr, count);
							}
						}
					} 
					else if (strcmp(when, "PREV1OR2OR3TAG") == 0) 
					{
						if (count > 0) 
						{
							if (count>1) 
								tempcount1 = count-2;
							else 
								tempcount1 = count-1;
							if (count >2) 
								tempcount2 = count-3;
							else 
								tempcount2 = count-1;
							if (strcmp(tag, tag_corpus_array[count - 1]) == 0 ||
								strcmp(tag, tag_corpus_array[tempcount1]) == 0 ||
								strcmp(tag, tag_corpus_array[tempcount2]) == 0 )
							{
								change_the_tag(tag_corpus_array, newStr, count);
							}
						}
					} 
					else if (strcmp(when, "PREV1OR2OR3WD") == 0) 
					{
						if (count > 0) 
						{
							if (count>1) 
								tempcount1 = count-2;
							else 
								tempcount1 = count-1;
							if (count >2) 
								tempcount2 = count-3;
							else 
								tempcount2 = count-1;
							if (strcmp(word, word_corpus_array[count - 1]) == 0 ||
								strcmp(word, word_corpus_array[tempcount1]) == 0 ||
								strcmp(word, word_corpus_array[tempcount2]) == 0)
								change_the_tag(tag_corpus_array, newStr, count);
						}
					} 
					else if (strcmp(when, "PREVBIGRAM") == 0) 
					{
						if (count > 1) 
						{
							if (strcmp(prev2, tag_corpus_array[count - 1]) == 0 &&
								strcmp(prev1, tag_corpus_array[count - 2]) == 0)
								change_the_tag(tag_corpus_array, newStr, count);
						}
					}
					else 
						fprintf(stderr,
						"ERROR: %s is not an allowable transform type\n",
						when);
				}
			}
		}  /* end: for (count =0; count <= corpus_size; ++count) */
		free(*split_ptr);      // leak plugged by Benjamin Han
		free(split_ptr);
	}  /* end: for (i=0;i<Darray_get(cRuleArray);i++) */



	int wordIndex = 0;
	isLineStart = true;
	for ( int i = 0; i < tokenCount; i++) 
	{
		GeoToken * token = (GeoToken*) tokens[i];
		if ( isLineStart )
		{
			wordIndex += 2;
			isLineStart = false;
		}
		//printf("%s/%s - %s ",  word_corpus_array[wordIndex], tag_corpus_array[wordIndex],token->getTokenString().c_str() );
		token->setPos( tag_corpus_array[wordIndex] );
		if ( token->isLineEnd() )
		{
			isLineStart = true;
			//printf("\n\n");
		}
		wordIndex++;
	}

	for ( int i=0; i<word_corpus_array_index; i++)
	{
		if ( word_corpus_array[i] != staart ) 
		{
			free( word_corpus_array[i] );
		}
		if ( tag_corpus_array[i]!=staart && tag_corpus_array[i]!=newStr )
		{
			free( tag_corpus_array[i] );
		}
	}

	free(word_corpus_array);
	free(tag_corpus_array);

}


