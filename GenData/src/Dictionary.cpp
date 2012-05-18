#include "Dictionary.h"
#include <fstream>
#define D(x);
Dictionary::Dictionary()
{
	openDictionary( us, US_STATE_FILE );
	openDictionary( sd, STREET_DIRECTION_FILE );
	openDictionary( ss, STREET_SUFFIX_FILE );
	openDictionary( sud, SECONDARY_UNIT_DESIGNATOR_FILE );
	openDictionary( sud, OTHERS_FILE );
}

Dictionary::~Dictionary()
{
}

bool Dictionary::openDictionary( TernarySearchTree<char> &dic, const char * fileName )
{
  D( printf("Opening file %s.\n", fileName ); );
	bool ret = false;
	char buffer[DIC_LINE_MAX_LENGTH];
	FILE * fp = fopen( fileName,"r" );

	if ( fp )
	{
		while ( fgets( buffer, DIC_LINE_MAX_LENGTH, fp ) != NULL )
		{
			/* read in each line and extract url and file id */
			if ( *buffer==0 )
				continue;
			string str( buffer );
			str.trimEnd("\n\r").trimEnd("\r\n");
			dic.add( str.toLower().c_str(), '1' );
			D( printf( "Adding token %s.\n", str.c_str() ); );
		}
		fclose(fp);
		ret = true;
	}
	return ret;
}

void Dictionary::lookup( Vector<IToken*> & tokens )
{
	int count = (int)(tokens.count());
	for ( int i=0; i<count; i++ )
	{
		GeoToken * token = (GeoToken*)tokens[i];
		token->setUSState( isUSState( token->getTokenString().c_str() ) );
		token->setStreetDirection( isStreetDirection( token->getTokenString().c_str() ) );
		token->setStreetSuffix( isStreetSuffix( token->getTokenString().c_str() ) );
		token->setSecondaryUnitDesignator( isSecondaryUnitDesignator( token->getTokenString().c_str() ) );
	}

}

bool Dictionary::isUSState( const char * str )
{
	return us.contains( String( str ).toLower().c_str() );
}

bool Dictionary::isStreetDirection( const char * str )
{
	return sd.contains( String( str ).toLower().c_str() );
}

bool Dictionary::isStreetSuffix( const char * str )
{
	return ss.contains( String( str ).toLower().c_str() );
}

bool Dictionary::isSecondaryUnitDesignator( const char * str )
{
	return sud.contains( String( str ).toLower().c_str() );
}

bool Dictionary::isOtherGeoToken( const char * str )
{
	return other.contains( String( str ).toLower().c_str() );
}
