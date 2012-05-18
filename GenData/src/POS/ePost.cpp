// ======================================================================
// ePost.cpp - main program of an English Part-of-Speech Tagger
// 
// 102300: Benjamin Han <benhdj@cs.cmu.edu> Created.
// 110500: Benjamin Han <benhdj@cs.cmu.edu> Added a simple exception for
//         rudimentary file IO error handling.
// 080401: Benjamin Han <benhdj@cs.cmu.edu> Adapted for MSVC++; chaned 
//         license to LGPL.
// 071802: Benjamin Han <benhdj@cs.cmu.edu> Added #include <iterator> for
//         gcc 3.1.
// ======================================================================

//   Copyright (C) 2001 Benjamin Han <benhdj@cs.cmu.edu>
//   The original Transformation-based Part-of-Speech Tagger is written
//   by Eric Brill <brill@cs.jhu.edu> - see the separate copyright notice
//   under post directory
//
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Lesser General Public
//    License as published by the Free Software Foundation; either
//    version 2.1 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public
//    License along with this library; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

//#include "except.h"
#include <fstream>
#include <string>
#include <iostream>
//#include <list>
#include "../vector.h"
#include "BrillTagger.h"
#define BUF_SIZE 10240


using namespace std;

#define ERR_SUCCESS                    0
#define ERR_ILLEGAL_OPT                1
#define ERR_OPEN_INPUT_FILE            2
#define ERR_ILLEGAL_CORPUS_FILE_FORMAT 3
#define ERR_POSTERROR                  4


void printHelp ()
{
	cerr<<"* epost input-file"<<endl<<endl;
	cerr<<"  input-file: A formatted and tokenized English corpus."<<endl<<endl;  
	cerr<<"  *IMPORTANT*: DO NOT INCLUDE EMPTY LINES OR LINES WITH ONLY WHITE CHARS!"<<endl<<endl;
}

/**
 * tag the given file 
 */
bool runPOS ( const char * fileName )
{
	int ret = true;
	FILE *fp;
	char buf[BUF_SIZE];

	Vector<char*> sentences;
	fp=fopen( fileName, "r");
	if ( fp )
	{
		while (fgets(buf,BUF_SIZE,fp))
		{
			//string * str = new string ( buf );
			//str->trimEnd( "\n" );
			//if (s[s.length()-1]== '\n') s.erase(s.length()-1);
			int len = strlen( buf );
			if ( len > 0 && buf[ len-1 ] =='\n' )
			{
				buf[len-1] = '\0';
			}		
			if ( len > 1 && buf[len - 2]=='\r' )
		    {
		      buf[len-2]='\0';
		    }
			len = strlen( buf ) + 1;
			char * textLine = (char*)malloc( len );
			memcpy( textLine, buf, len );
			sentences.add( textLine );
		}
		fclose(fp);

	}
	else
	{
		cerr<<"* ERROR: Cannot open "<<fileName<<" for reading."<<endl;
		ret = false;
	}

	int count = sentences.count();
	if ( count > 0 )
	{
		char **buffer;

		buffer= (char**) malloc( count * sizeof(char*) );
		for ( int i=0; i< count; i++ )
		{
		  buffer[i] = sentences[i];
		}

		// ================= Call the tagger now ================= 
		/*buffer = StartStateTagger (buffer, numSen, lexicon_hash,
			good_right_hash, good_left_hash,
			bigramArray, lRuleArray);
			*/
		BrillTagger brillTagger( LEXICON, BIGRAMS, LRULEFILE, CRULEFILE );
		//buffer = brillTagger.startStateTagger( buffer, numSen, &lRules, &bigrams );
		buffer = brillTagger.startStateTagger( buffer, count );
		// lexicon_hash is effectively the same as a word_hash, which
		// is read from the lexicon file and originally used here. We
		// just replace word_hash with lexicon_hash here (save memory!)
		/*buffer = FinalStateTagger (buffer, numSen, seenTagging_hash,
			lexicon_hash, cRuleArray);
			*/
		buffer = brillTagger.finalStateTagger (buffer, count );

		// ============== End of call to the tagger ==============

		// display text with tags
		for( int i=0; i<count; i++ ) 
		{
			printf("tags> %s.\n", buffer[i] );
			free(  buffer[i] );
			//delete sentences[i];
		}

		free( buffer );
	}
	return ret;
}



int main(int argc, char* argv[])
{
	int ret = ERR_SUCCESS;

	string s;

	cerr<<"* Brill's English Part-of-Speech Tagger v1.0, adpated by Benjamin Han "<<endl<<endl;
	if (argc!=2)
	{
		printHelp();
		cerr<<"* Illegal command-line syntax."<<endl;
		return ERR_ILLEGAL_OPT;
	}

	runPOS( argv[1] );



	/*try
	{
	*/
//	POST post;

//	post.run( sList );

//		copy( sList.begin(), sList.end(), ostream_iterator<string>(cout,"\n") );
	/*  }

	catch (Exception &e)
	{
	cerr<<"* ERROR: "<<e<<endl;
	return ERR_POSTERROR;
	}
	*/
	return ret;
}
