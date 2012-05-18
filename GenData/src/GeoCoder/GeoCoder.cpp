#include "GeoCoder.h"
#include "../Config.h"

#define D(x)
#define MIN_TOKENS_IN_ADDRESS 4

GeoCoder::GeoCoder()
{
	index = io_open( GEO_MAP_FILE );
	out = new geo_location();
	out->zip_code = 0;
	strcpy(out->street_name,"");
	strcpy(out->city_name,"");
	strcpy(out->state_name,"");
	rangeCount=0;
}

GeoCoder::~GeoCoder()
{
	if ( index )
		io_close( index );
	if ( out )
		delete out;
}

void GeoCoder::run( Vector<IToken*> & tokens )
{
	int count = (int)(tokens.count());
	for ( int i=0; i<count; i++ )
	{
		GeoToken * token = (GeoToken*) tokens[i];
		if ( token->getTokenString().isNumber() )
		{
			D( printf("Starting Number: %d.\n", atoi( token->getTokenString().c_str() ) ); );
			int endIndex = getAddressEnd( tokens, i );
			if ( endIndex != -1 && endIndex - i >= MIN_TOKENS_IN_ADDRESS )
			{ /* is an address, endIndex is the vector index of the last token in the address */
				setAddressTag( tokens, i, endIndex );
				i = endIndex + 1;
			}
		}
	}
}

#define FRAME_WINDOW_SIZE 5
#define MAX_ADDRESS_LENGTH 20
void GeoCoder::runFIPS( Vector<IToken*> & tokens )
{
	int count = (int)(tokens.count());
	lastNamePos = lastNumberPos = -1;
	for ( int i=0; i<count; i++ )
	{
		GeoToken * token = (GeoToken*) tokens[i];
		if ( token->getTokenString().isNumber() )
		{
			lastNumberPos = i;
			lastNamePos = i;
			streetNo = atoi( token->getTokenString().c_str() );
		}
		if ( lastNumberPos != -1 && i - lastNamePos <= FRAME_WINDOW_SIZE  && i - lastNumberPos <= MAX_ADDRESS_LENGTH )
		{
			//printf("is number %s.\n", token->getTokenString().c_str() );
			int newPos = getNames( tokens, i+1 );
			if ( newPos == i+1 && i+2 < count )
			{
				newPos = getNames( tokens, i+2 );
			}
			if ( newPos == i+2 && i+3 < count )
			{	
				getNames( tokens, i+3 );
			}
			if ( newPos > lastNamePos )
			{
				lastNamePos = newPos;
			}
			rangeCount = 0;
		}


#if 0
		int endIndex = getAddressEnd( tokens, i );
		if ( endIndex != -1 )
		{ /* is an address, endIndex is the vector index of the last token in the address */
			setAddressTag( tokens, i, endIndex );
			i = endIndex + 1;
		}
#endif

	}
}

void GeoCoder::setAddressTag( Vector<IToken*> & tokens, int startIndex, int endIndex )
{
	if ( startIndex<=endIndex && startIndex >=0 && endIndex < tokens.count() )
	{
		for ( int i=startIndex; i<=endIndex; i++ )
		{
			GeoToken * token = (GeoToken*)tokens[i];
			/* set GeocoderStart, GeoCoderMid, GeoCoderEnd, GoeCoderOther */
			i == startIndex ? token->setGeocoderStart() : i == endIndex ? token->setGeocoderEnd() : token->setGeocoderMid();
			//	  printf("GeoCoder %s - %s\n", token->getTokenString().c_str(), token->isGeocoderStart()?"START":token->isGeocoderEnd()?"END":token->isGeocoderMid()?"MID":token->isGeocoderEnd()?"END":"UNKNOWN");
			token->setGeocoderAddress(); // set GEO/NOTGEO feature
		}
	}
}

int GeoCoder::getAddressEnd( Vector<IToken*> & tokens, int start )
{
	int ret = -1;
	int bufferLen = 82;
	char * buffer = (char*)malloc( bufferLen );
	buffer[0]=0;
	int index = start;
	int count = (int)(tokens.count());
	int len = 0;

	while (  index < count )
	{
		GeoToken * token = (GeoToken*) tokens[index];
		if ( len + (int)(token->getTokenString().length()) + 2 > bufferLen )
		{
			break;
		}
		sprintf( buffer + len, "%s ", token->getTokenString().c_str() );
		++index;
		len += (int)(token->getTokenString().length()) + 1;
	}
	/* get index of last token in address */
	int endIndex = getAddressEnd( buffer );
	free( buffer );
	index = start;
	if ( endIndex > 0 ) 
	{ /* is an address */
		int totalTokens = 0;
		len = 0;
		while ( index < count )
		{
			GeoToken * token = (GeoToken*) tokens[index];
			len += (int)(token->getTokenString().length()) + 1;
			if ( len > endIndex )
			{
				break;
			}
			++index;
		}
		ret = index;
		// printf("totalTokens %d last token is %s.\n", index - start, (GeoToken*)tokens[index]->getTokenString().c_str() );
	}

	return ret;
} 

int GeoCoder::getAddressEnd( const char * buffer )
{
	D( printf("Potential address: %s.\n", buffer ); );
	struct geo_location loc;
	/* geocoding the input address */
	int endIndex = geo_find( index, buffer,(int)(strlen(buffer)),&loc);
	D( if ( endIndex > 0 )
	{ /* is an address */
		printf("Address %.*s.\n",endIndex, buffer );
	} );
	return endIndex > 0 ? endIndex : -1;

}

int GeoCoder::find_name( int begin,int end, char type,const char *name,int name_len )
{
	const int size = 45;
	const int count = (end - begin) / size;
	const int mid = begin + size * (count / 2);
	char test[41];
	if (count <= 1) return begin;
	if (io_in( index,mid - size,test,sizeof test) < 0) return -1;
	if (name_len > sizeof test - 1)
		name_len = sizeof test - 1;
	/* binary search the name in the index file */
	//D( printf("find_name %.*s type %c %.*s.\n", name_len, name, type, name_len+1, test ) );
	if (type > test[0]
	|| (type == test[0] && strncasecmp( name, test + 1, name_len) > 0) )
	{
		return find_name( mid,end,type,name,name_len);
	}
	else
	{
		return find_name( begin,mid,type,name,name_len);
	}
}

void GeoCoder::tagToken( Vector<IToken*> & tokens, int start, int end, char type )
{
	while ( start <= end )
	{
		GeoToken * token = (GeoToken*)(tokens[start]);
		//printf("%s ", ()->getTokenString().c_str() );
		type == 'O' || type == 'E' ? token->setStreet() : type == 'C' ? token->setCity() : type == 'S' ? token->setState() : token->setZip();
		++start;
	}
}

int GeoCoder::getNames( Vector<IToken*> & tokens, int index )
{
	int start = index;
	int count = (int)(tokens.count());
	String name;
	while ( index < count )
	{
		bool nameFound = false;
		GeoToken * token = (GeoToken*) tokens[index];
		if ( name.length() == 0 )
		{
			name = token->getTokenString() + " ";
		}
		else
		{
			name += token->getTokenString();
			name += " ";
		}
		//name = "New York";
		bool partialMatch = false;
		if ( getName( 'A', name, partialMatch ) ) /* replace alias */
		{
			//printf("alias %s\n", name.c_str() );
			//tagToken( tokens, start, index, 'A');
			//nameFound = true;
		}
		if ( getName( streetNo % 2 ? 'O' : 'E', name, partialMatch ) ) /* get street */
		{
			//printf("street %s\n", name.c_str() );
			tagToken( tokens, start, index, streetNo % 2 ? 'O' : 'E' );
		}
		if ( partialMatch )
			nameFound = true;
		if ( getName( 'C', name, partialMatch ) )
		{
			//printf("city %s\n", name.c_str() );
			tagToken( tokens, start, index, 'C');
		}
		if ( partialMatch )
			nameFound = true;
		if ( getName( 'S', name, partialMatch ) )
		{
			//printf("state %s\n", name.c_str() );
			tagToken( tokens, start, index, 'S');
		}
		if ( partialMatch )
			nameFound = true;
		if ( getZip( name ) )
		{

			printf("ZIP: %s\n", name.c_str() );
			tagToken( tokens, start, index, 'Z' );
			nameFound = true;
		}
		if ( !nameFound )
		{
			break;
		}
		index++;

	}

	return index;

}

int GeoCoder::getName( String & name )
{
	bool partialMatch=false;
	return getName( 'A', name, partialMatch ); /* replace alias */
}

int GeoCoder::getName( char type, String & name, bool & partialMatch )
{
	partialMatch = false;
	char n[41];
	int ret = 0;
	int begin,end,pos;
	int len = name.length();
	if ( io_in_i4(index,
		io_in_i4(index,
		io_in_i4( index, 0, NULL ), &begin ), &end ) >= 0
		)
	{
		pos = find_name( begin, end, type, name.c_str(), len ); /* recursive binary search the name in the index file */
		if ( pos >= 0 && pos != end )
		{
			pos = io_in( index,pos,n,sizeof n );	/* read in type */
			if ( pos >=0 && pos != end )
			{
				pos = io_in_i4( index,pos, &begin );	/* read in begin position*/
				if ( pos >=0 && pos != end )
				{
					pos = io_in( index,pos,NULL,sizeof n);
					pos = io_in_i4( index, pos,&end ); /* get end position */
					if ( pos >= 0 &&  n[0] == type	/* compare given type with the type found in index file*/
						&&  0 == strncasecmp(n + 1, name.c_str(), len ) )
					{
						if ( getNameLengthInDatabase( n+1, sizeof n ) == len || n[len+1]== '=' )	/* Jerry. Mods to exactly match */
						{
							ret = 1;
						}
						partialMatch = true;
					}
				}
			}
		}

	}
	if ( ret == 0 )
	{
		D(printf("    '%c' \"%.*s\" not found\n",type,len,name.c_str()));
		return 0;
	}
	else
	{
		D(printf("    '%c' \"%.*s\" found begin %d, end %d\n",type,len,name.c_str(), begin, end ) );
	}

	/* n+1 point to the found name string, len is the string length */
	if ('=' == n[len + 1]) 
	{ 
		/* alias expansion */
		char * replace = n;
		int delta;

		begin = len + 3;
		while (begin < sizeof n && ' ' == n[begin]) ++begin;

		end = sizeof n;
		while (end > begin && ' ' == n[end - 1]) --end;
		if (end < sizeof n) ++end;
		if (end - begin > len) end = begin + len;
		D(printf("    Replacing '%.*s' with '%.*s'\n",len,n + 1,end - begin,&n[begin]));
		strncpy( replace, n + begin, end-begin );
		replace[end-begin]=0;
		name = replace;

		delta = len - (end - begin);
		//s->buffer_end -= delta;
		//s->buffer_next -= delta;
		//D(printf("    Buffer is now: '%.*s'\n",s->buffer_end - s->buffer,s->buffer));
		return 1;
	}
	/* get range */
	range[rangeCount].begin = begin;
	range[rangeCount].end = end;
	++rangeCount;
	//D(printf("<<< '%c' \"%.*s\"\n",type,len ,n + 1));
	return ret;

}

int GeoCoder::getZip( String str ) 
{
	str.trim();
	if ( str.length() < 5 || str.length() > 10 )
		return 0;
	int i = str.indexOf("-");
	if ( i > 0 )
	{
		if ( str.indexOf( "-", i+1 ) != -1 )
			return 0;	/*have more than one "-", not zip */
		str = str.substring(0,i);
	}

	if ( !str.isNumber() ||	str.length() < 5 || str.length() > 10 )
		return 0;
	int zip,zip_offset;
	int len = str.length();
	if ( 0 ==  len ) return 0;
	zip = io_strntoi( str.c_str(), len );
	if ( zip <= 0 || zip >= 100000
		||  io_in_i4( index,0,&zip_offset) < 0)
		return 0;
	//	rangeCount = 0;
	range[ rangeCount ].begin = zip_offset + 4 * zip;
	range[ rangeCount ].end = zip_offset + 4 * zip + 8;
	D(printf(">>> %d, zip_offset %d, range begin %d end %d\n",zip, zip_offset, range[rangeCount].begin, range[rangeCount].end ));
	++rangeCount;
	//if (isValidZip()) 
	{
		return 1;
	}
	D(printf("<<< not valid zip %d\n",zip));
	//--rangeCount;
	return 0;
}

int GeoCoder::isValidZip()
{
	int i,pos[sizeof(range)/sizeof(*range)];
	float deta = (float)0.01;

	if (0 == rangeCount) return 0;

	for (i = 0; i < rangeCount; ++i) 
	{
		D(printf("    Metarange #%d: %d entries begin %d\n",i,(range[i].end - range[i].begin) / 8, range[i].begin));
		pos[i] = range[i].begin;
	}

	for (;;) 
	{
		int i_begin = 1,i_end = 1;
		int best_begin = 1,best_end = 1;
		char out_side = 'X';
		int out_addr = 0,out_lat = 0,out_long = 0;

		D(printf("Searching ---\n"));
		for (i = 0; i != i_begin; i = (i + 1) % rangeCount) 
		{
			int r_begin,r_end;
			for (;;) 
			{
				int n;
				if (pos[i] >= range[i].end || pos[i] < 0) 
				{
					D(printf("    Range #%d @ %d: End\n",i,(pos[i] - range[i].begin) / 8));
					return 0;
				}
				n = io_in_i4(index,
					io_in_i4(index,pos[i],&r_begin),&r_end);
				if (n < 0) 
					return 0;
				D(printf("    Range #%d @ %d: [%d,%d)\n",i,(pos[i] - range[i].begin) / 8,r_begin,r_end));
				if (r_end > best_begin && r_begin < r_end) 
					break;
				pos[i] = n;
			}
			if (r_begin > best_begin) 
			{
				best_begin = r_begin;
				best_end = r_end;
				i_begin = i_end = i;
				D(printf("        Best -> [%d#%d,%d#%d)\n",best_begin,i_begin,best_end,i_end));
			}
			else if (r_end < best_end) 
			{
				best_end = r_end;
				i_end = i;
				D(printf("        Best -> [%d#%d,%d#%d)\n",best_begin,i_begin,best_end,i_end));
			}
		}

		D(printf("Address range ---\n"));
		pos[i_end] += 8; /* hack... */
		do 
		{
			int next,lon,lat,addr;
			best_begin = io_in_i4(index,best_begin,&next);
			best_begin = io_in_i4(index,best_begin,&lon);
			best_begin = io_in_i4(index,best_begin,&lat);
			best_begin = io_in_i4(index,best_begin,&addr);

			/* PERF: This should be a binary search... */
			while (best_begin >= 0 && best_begin < next) {
				short da;
				signed char dln,dlt;
				char side;
				best_begin = io_in_i2(index,best_begin,&da);
				best_begin = io_in_i1(index,best_begin,&dln);
				best_begin = io_in_i1(index,best_begin,&dlt);
				best_begin = io_in(index,best_begin,&side,1);
				//D(printf("    %c: %d vs. %d\n",side,addr + da,streetNo / 2));
				if (addr + da <= streetNo / 2) {
					out_side = side;
					out_addr = addr + da;
					out_lat = lat + dlt;
					out_long = lon + dln;
				} else if ('X' == out_side) 
				{
					best_begin = next;
					D(printf("    Not this time.\n"));
					break;
				}
				else if (true)// if (NULL != out) 
				{
					const int parity = streetNo % 2;

					out->before.address =
						2 * out_addr + parity;
					out->before.latitude =
						out_lat / 100000.0;
					out->before.longitude =
						out_long / 100000.0;

					if ('X' == side) --da;
					out->after.address =
						2 * (addr + da) + parity;
					out->after.latitude =
						(lat + dlt) / 100000.0;
					out->after.longitude =
						(lon + dln) / 100000.0;
					//D( printf("s->address %d out_addr %d addr %d da %d.\n", s->address, out_addr, addr, da ) );
					out->at.address = streetNo;
					deta =  addr + da - out_addr == 0 ? 0.01: 0; /* Jerry Yu's tempory fix for divid by zero error */
					out->at.latitude = (out_lat +
						(lat + dlt - out_lat) *
						( streetNo / 2 - out_addr) /
						(addr + da - out_addr + deta )) /
						100000.0;

					out->at.longitude = (out_long +
						(lon + dln - out_long) *
						(streetNo / 2 - out_addr) /
						(addr + da - out_addr + deta )) /
						100000.0;

					out->side = out_side;
					D(printf("    Success!\n"));
					return 1;
				} 
				else {
					D(printf("    Success...\n"));
					return 1;
				}
			}
		} 
		while (best_begin > 0 && best_begin < best_end);
	}
}

int GeoCoder::strncasecmp(const char *s1, const char *s2, size_t n)
{
	while ( n != 0) {
		if (toupper(*s1) != toupper(*s2)) {
			return toupper(*s1) - toupper(*s2);
		}
		if (*s1 == '\0') {
			return 0;
		}
		n--; s1++; s2++;
	}
	return 0;

}

int GeoCoder::getNameLengthInDatabase( const char * name, int len )
{
	int index = 0;
	while ( index < len )
	{
		if ( index + 1 ==len )
		{
			break;
		}
		if ( name[index] == ' ' && name[index+1] == ' ' )
		{
			++index;
			break;
		}
		++index;
	}
	return index;
}