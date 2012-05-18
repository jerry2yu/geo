/* Copyright 2002 Daniel Egnor.  See LICENSE file.
*
* The geo_find() function defined here uses an address map built by 
* geo-*-to-* to parse an address and convert it to geographical
* coordinates.
*/

#include "geo.h"
#include "io.h"

#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
/**
* The strcasecmp() function shall compare, while ignoring differences in case, 
* the string pointed to by s1 to the string pointed to by s2. 
* The strncasecmp() function shall compare, while ignoring differences in case, 
* not more than n bytes from the string pointed to by s1 to the string pointed to by s2.
*
* The strcmp subroutine then returns a value that is:
* Less than 0 if the value of string String1 is lexicographically less than string String2.
* Equal to 0 if the value of string String1 is lexicographically equal to string String2.
* Greater than 0 if the value of string String1 is lexicographically greater than string String2.
*/
int strncasecmp(const char *s1, const char *s2, size_t n)
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

#define D(x)
//#define D(x)

struct state 
{
	struct io_file *index;	/* index file */

	/* Text input */
	int input_depth;
	const char *input_begin,*input_end;	/* begin and end pointers to the string */
	char buffer[100],*buffer_next,*buffer_end;
	const char *next;	/* next token string without \0 terminator */

	/* Working hypothesis */
	int address;						/* street number */
	struct { int begin,end; } range[4];
	int range_count;

	/* Answer */
	struct geo_location *out;
};

static int is_valid(struct state *s) 
{
	int i,pos[sizeof(s->range)/sizeof(*s->range)];
	float deta = (float)0.01;

	if (0 == s->range_count) return 0;

	for (i = 0; i < s->range_count; ++i) 
	{
		//D(printf("    Metarange #%d: %d entries\n",i,(s->range[i].end - s->range[i].begin) / 8));
		pos[i] = s->range[i].begin;
	}

	for (;;) 
	{
		int i_begin = 1,i_end = 1;
		int best_begin = 1,best_end = 1;
		char out_side = 'X';
		int out_addr = 0,out_lat = 0,out_long = 0;

		//D(printf("Searching ---\n"));
		for (i = 0; i != i_begin; i = (i + 1) % s->range_count) 
		{
			int r_begin,r_end;
			for (;;) 
			{
				int n;
				if (pos[i] >= s->range[i].end || pos[i] < 0) 
				{
					//D(printf("    Range #%d @ %d: End\n",i,(pos[i] - s->range[i].begin) / 8));
					return 0;
				}
				n = io_in_i4(s->index,
					io_in_i4(s->index,pos[i],&r_begin),&r_end);
				if (n < 0) 
					return 0;
				//D(printf("    Range #%d @ %d: [%d,%d)\n",i,(pos[i] - s->range[i].begin) / 8,r_begin,r_end));
				if (r_end > best_begin && r_begin < r_end) 
					break;
				pos[i] = n;
			}
			if (r_begin > best_begin) 
			{
				best_begin = r_begin;
				best_end = r_end;
				i_begin = i_end = i;
				//D(printf("        Best -> [%d#%d,%d#%d)\n",best_begin,i_begin,best_end,i_end));
			}
			else if (r_end < best_end) 
			{
				best_end = r_end;
				i_end = i;
				//D(printf("        Best -> [%d#%d,%d#%d)\n",best_begin,i_begin,best_end,i_end));
			}
		}

		D(printf("Address range ---\n"));
		pos[i_end] += 8; /* hack... */
		do 
		{
			int next,lon,lat,addr;
			best_begin = io_in_i4(s->index,best_begin,&next);
			best_begin = io_in_i4(s->index,best_begin,&lon);
			best_begin = io_in_i4(s->index,best_begin,&lat);
			best_begin = io_in_i4(s->index,best_begin,&addr);

			/* PERF: This should be a binary search... */
			while (best_begin >= 0 && best_begin < next) {
				short da;
				signed char dln,dlt;
				char side;
				best_begin = io_in_i2(s->index,best_begin,&da);
				best_begin = io_in_i1(s->index,best_begin,&dln);
				best_begin = io_in_i1(s->index,best_begin,&dlt);
				best_begin = io_in(s->index,best_begin,&side,1);
				D(printf("    %c: %d vs. %d\n",side,addr + da,s->address / 2));
				if (addr + da <= s->address / 2) {
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
				else if (NULL != s->out) 
				  {
					const int parity = s->address % 2;

					s->out->before.address =
						2 * out_addr + parity;
					s->out->before.latitude =
						out_lat / 100000.0;
					s->out->before.longitude =
						out_long / 100000.0;

					if ('X' == side) --da;
					s->out->after.address =
						2 * (addr + da) + parity;
					s->out->after.latitude =
						(lat + dlt) / 100000.0;
					s->out->after.longitude =
						(lon + dln) / 100000.0;
					//printf("s->address %d out_addr %d addr %d da %d.\n", s->address, out_addr, addr, da );
					s->out->at.address = s->address;
					deta =  addr + da - out_addr == 0 ? 0.01: 0; /* Jerry Yu's tempory fix for divid by zero error */
					s->out->at.latitude = (out_lat +
						(lat + dlt - out_lat) *
						(s->address / 2 - out_addr) /
						(addr + da - out_addr + deta )) /
						100000.0;

					s->out->at.longitude = (out_long +
						(lon + dln - out_long) *
						(s->address / 2 - out_addr) /
						(addr + da - out_addr + deta )) /
						100000.0;

					s->out->side = out_side;
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

/**
* Recursively binary search the map file to find given name.
* @param	begin - start search position of the file
* @param	end - end search position of the file
* @param	index - map file
* @param	name - name to search
* @param	name_len - length of the name string
*/
static int find_name( struct io_file *index,int begin,int end, char type,const char *name,int name_len )
{
	const int size = 45;
	const int count = (end - begin) / size;
	const int mid = begin + size * (count / 2);
	char test[41];
	if (count <= 1) return begin;
	if (io_in(index,mid - size,test,sizeof test) < 0) return -1;
	if (name_len > sizeof test - 1)
		name_len = sizeof test - 1;
	/* binary search the name in the index file */
	//D( printf("find_name %.*s type %c %.*s.\n", name_len, name, type, name_len, test ) );
	if (type > test[0]
	|| (type == test[0] && strncasecmp( name, test + 1, name_len) > 0))
	{
		return find_name(index,mid,end,type,name,name_len);
	}
	else
	{
		return find_name(index,begin,mid,type,name,name_len);
	}
}

static const char *next_word(struct state *,const char *);

/**
* Meat of the address finding
* Search the name in the index file, compare type with given type,
* if the name is found, and there is alias, do alias expansion
* @param last - pointer to the next token
* @param type - which type of name to search in the index file. 
*				E O: street name, which has even or odd street number , C: city name, S, state name
* @param nextFunc - function pointer to function need to be run for next state
* @return	0 if name not found, 1 if found
*/
static int get_name_at( struct state *s, char type, int (*nextFunc)(struct state *), const char *last )
{
	char n[41];
	const char *next,*save;
	/* get length of current token */
	int len = (int)(last - s->next);
	int begin,end,pos;

	if (io_in_i4(s->index,
		io_in_i4(s->index,
		io_in_i4( s->index, 0, NULL ), &begin ), &end ) < 0
		|| (pos = find_name(s->index,begin,end,type,s->next,len)) < 0 /* recursive binary search the name in the index file */
		||  pos == end
		|| (pos = io_in_i4(s->index,io_in(s->index,pos,n,sizeof n),&begin)) < 0	/* get the type into n, and begin position*/
		||  pos == end
		|| (pos = io_in_i4(s->index,io_in(s->index,pos,NULL,sizeof n),&end)) < 0 /* get end position */
		||  n[0] != type	/* compare given type with the type found in index file*/
		||  strncasecmp(n + 1,s->next,len))	/* compare name, might be redundent */
	{
		D(printf("    '%c' \"%.*s\" not found\n",type,len,s->next));
		return 0;
	}
	else
	{
	D(printf("    '%c' \"%.*s\" found begin %d, end %d\n",type,len,s->next, begin, end ) );
	}

#if 0
	int ret = 0;
	if ( io_in_i4(s->index,
			io_in_i4(s->index,
				io_in_i4( s->index, 0, NULL ), &begin ), &end ) >= 0
		)
	{
		pos = find_name(s->index,begin,end,type,s->next,len); /* recursive binary search the name in the index file */
		if ( pos >= 0 && pos != end )
		{
			pos = io_in(s->index,pos,n,sizeof n);	/* read in type */
			if ( pos >=0 && pos != end )
			{
				pos = io_in_i4(s->index,pos, &begin );	/* read in begin position*/
				if ( pos >=0 && pos != end )
				{
					pos = io_in(s->index,pos,NULL,sizeof n);
					pos = io_in_i4(s->index, pos,&end ); /* get end position */
					if ( pos >= 0 &&  n[0] == type	/* compare given type with the type found in index file*/
						&&  0 == strncasecmp(n + 1,s->next,len))	/* compare name, might be redundent */
					{
						ret = 1;
					}
				}
			}
		}
	}
	if ( ret == 0 )
	{
		D(printf("    '%c' \"%.*s\" not found\n",type,len,s->next));
		return 0;
	}
	else
	{
	D(printf("    '%c' \"%.*s\" found begin %d, end %d\n",type,len,s->next, begin, end ) );
	}
#endif

	/* n+1 point to the found name string, len is the string length */
	if ('=' == n[len + 1]) 
	{ 
		/* alias expansion */
		char * const replace = (s->next - s->buffer) + s->buffer;	/* is that just s->next ? */
		int delta;

		begin = len + 2;
		D( printf( "sizeof n: %d.\n", sizeof n ) );
		while (begin < sizeof n && ' ' == n[begin]) ++begin;

		end = sizeof n;
		while (end > begin && ' ' == n[end - 1]) --end;
		if (end < sizeof n) ++end;
		if (end - begin > len) end = begin + len;
		D(printf("    Replacing '%.*s' with '%.*s'\n",len,n + 1,end - begin,&n[begin]));

		memcpy(replace,n + begin,end - begin);
		memmove(replace + (end - begin),
			replace + len,
			s->buffer_end - replace - len);

		delta = len - (end - begin);
		s->buffer_end -= delta;
		s->buffer_next -= delta;
		D(printf("    Buffer is now: '%.*s'\n",s->buffer_end - s->buffer,s->buffer));
		return 1;
	}
	/* if still have more tokens, recursively find the name in the index file*/
	next = next_word(s,last);
	if ( next != last && get_name_at( s, type, nextFunc, next ) ) return 1;

	pos = len;
	while (++pos < sizeof n) 
	{
		if (' ' != n[pos]) 
			return 0;
	}
	/* get range */
	s->range[s->range_count].begin = begin;
	s->range[s->range_count].end = end;
	//D(printf(">>> '%c' \"%.*s\" found range begin %d end %d\n",type,len - 1,n + 1, s->range[s->range_count].begin, s->range[s->range_count].end ) );
	++s->range_count;
	save = s->next;
	s->next = last;
	/* find name from following tokens */
	if ( NULL != nextFunc && nextFunc(s) ) 
	{
		/* copy the found name to corresponding s->out( street, city or state ) */
		char *out = NULL;
		switch (type) 
		{
		case 'E':
		case 'O':
			out = s->out ? s->out->street_name : NULL;
			break;
		case 'C':
			out = s->out ? s->out->city_name : NULL;
			break;
		case 'S':
			out = s->out ? s->out->state_name : NULL;
			break;
		}

		if (NULL != out) 
		{
			strncpy(out,n + 1,len - 1);
			out[len - 1] = '\0';
		}

		s->next = save;
		return 1;
	}

	D(printf("<<< '%c' \"%.*s\"\n",type,len - 1,n + 1));
	--s->range_count;
	s->next = save;
	return 0;
}

static const char *input_word( struct state *s,const char *pos ) 
{
	assert(pos >= s->buffer && pos <= s->buffer_end);
	if (pos != s->buffer_end) {
		while (pos < s->buffer_end && *pos != ' ') ++pos;
		while (pos < s->buffer_end && *pos == ' ') ++pos;
		return pos;
	}

	while (s->input_begin != s->input_end 
		&& (s->input_depth > 0 || !isalnum(*s->input_begin))) 
	{
		const char ch = *s->input_begin++;
		if ('(' == ch)
			++s->input_depth;
		if (')' == ch && s->input_depth > 0)
			--s->input_depth;
	}

	while (s->input_begin != s->input_end
		&&  s->buffer_end != &s->buffer[sizeof s->buffer]
#ifdef WIN32
		&&  iswalnum( *s->input_begin )) 
#else
		&&  isalnum( *s->input_begin )) 
#endif
{
			if (s->buffer == s->buffer_end
#ifdef WIN32
				|| !iswdigit(s->buffer_end[-1]) || !iswalpha(*s->input_begin))
#else
				|| !isdigit(s->buffer_end[-1]) || !isalpha(*s->input_begin))
#endif
				*s->buffer_end++ = *s->input_begin;
			++s->input_begin;
		}

		if (pos != s->buffer_end 
			&&  s->buffer_end != &s->buffer[sizeof s->buffer])
			*s->buffer_end++ = ' ';
		return s->buffer_end;
}
/**
* get word that is next to the given position
*/
static const char *next_word(struct state *s,const char *pos) 
{
	const char * const next = input_word(s,pos);
	const char * const save = s->next;
	assert(pos >= s->buffer && pos <= s->buffer_next);
	if (pos != s->buffer_next || next == pos) return next;

	s->next = pos;
	s->buffer_next = s->buffer + (next - s->buffer);
	if (get_name_at(s,'A',NULL,s->buffer_next)) 
	{
		s->buffer_next = s->buffer + (pos - s->buffer);
		s->next = save;
		return next_word(s,pos);
		/* NOTE! make sure aliases do not form a cycle */
	}

	s->next = save;
	return next;
}
/**
* find token in the index file
* @param	type - which type of name to search in the index file
* @param	f - skipp stuff function pointer
* @return	1 if name if found, otherwise 0
*/
static int get_name(struct state *s,char type,int (*f)(struct state *)) 
{
	/* get memory address of next token */
	const char * const next = next_word(s,s->next);
	if (s->next == next) return 0;
	return get_name_at(s,type,f,next);
}

static int get_zip(struct state *s) 
{
	const char * const next = next_word(s,s->next);
	int zip,zip_offset;

	if (s->next == next) return 0;
	zip = io_strntoi(s->next, (int)( next - s->next ));
	if (zip <= 0 || zip >= 100000
		||  io_in_i4(s->index,0,&zip_offset) < 0)
		return 0;

	s->range[s->range_count].begin = zip_offset + 4 * zip;
	s->range[s->range_count].end = zip_offset + 4 * zip + 8;
	D(printf(">>> %d, zip_offset %d, range begin %d end %d\n",zip, zip_offset, s->range[s->range_count].begin, s->range[s->range_count].end ));
	++s->range_count;
	if ( zip >=10000 )
//	if (is_valid(s)) 
	{
		if (NULL != s->out) s->out->zip_code = zip;
		D( printf("<<< valid zip %d\n",zip); );
		return 1;
	}
	D( printf("<<< not valid zip %d\n",zip); );
	--s->range_count;
	return 0;
}

static int optional_zip(struct state *s) {
	return get_zip(s) || is_valid(s);
}

static int get_state(struct state *s) 
{
	return get_name(s,'S',optional_zip) || optional_zip(s);
/* Jerry's testing */
/*	int ret = get_name(s,'S',optional_zip);
	const char * save;
	const char * next;
	int retry = 3;
	if ( !ret )
	{
		save = s->next;
		while ( !ret && retry-- > 0 )
		{
			s->next = next_word(s,s->next );
			ret = get_name(s, 'S',optional_zip );
		}
		s->next = save;
	}
	next = next_word(s,s->next); //testing Jerry.
	printf("get_state %.*s Result: %s.\n", next-s->next, s->next, ret ? "true" : "false" );

	return ret || optional_zip(s);
	*/
}

static int get_city(struct state *s) 
{

	return get_name(s,'C',get_state) || get_zip(s);
}

/**
* @return	1 if city if found, otherwise 0
*/
static int skip_stuff(struct state *s) {
	const char * const begin = s->next;
	int skipped;
	for (skipped = 0; skipped < 7; ++skipped)
		if (get_city(s)) 
		{
			s->next = begin;
			return 1;
		}
		else 
		{
			const char * const next = next_word(s,s->next);
			if (next == s->next) break;
			s->next = next;
		}

		s->next = begin;
		return 0;
}

/**
* Get address for a given string that follows to a potential street number
* @return 1 if is address, 0 not address
*/
static int get_street(struct state *s) 
{
	/* search name in the index file, if street number is odd, type is 'O', if even, type is 'E'.
	skip_stuff: skip function pointer */
	return get_name( s, (s->address % 2) ? 'O' : 'E', skip_stuff );
}

/**
* main method to get address
* if first token is number, will treat it as a potential street number, and start to match street in the index.
* street matching will be tried three times with the following tokens before give up
* @return 1 if found, otherwise 0
*/
static int get_address(struct state *s) 
{
	const char * const begin = s->next;
	const char * const next = next_word(s,begin);

	if (begin == next) 
		return 0;
	/* get street number */
	s->address = io_strntoi(begin,(int)( next - s->next ));
	/* if no street number found, not an address */
	if (0 == s->address) 
		return 0;
	s->next = next;
	/* get street name, try three times before giving up. */
	if (get_street(s)) 
	{
		s->next = begin;
		return 1;
	}
#if 0
	s->next = next_word(s,s->next);
	if (get_street(s)) 
	{
		s->next = begin;
		return 1;
	}

	s->next = next_word(s,s->next);
	if (get_street(s)) {
		s->next = begin;
		return 1;
	}
#endif
	s->next = begin;
	return 0;
}

/**
* Geocoding given address
* @param index - map file
* @param str - potential address string to be geocoded
* @param len - length of the address string
* @param out - output geocoding result
* @return	1 if found, otherwise 0
*/
int geo_find ( struct io_file *index,const char *str, int len, struct geo_location *out ) 
{
	struct state s;
	int ret;
	if (NULL == index) return 0;
	s.index = index;
	s.input_depth = 0;
	s.input_begin = str;
	s.input_end = str + len;
	s.next = s.buffer_end = s.buffer_next = s.buffer;
	s.address = s.range_count = 0;
	if (NULL != (s.out = out)) 
	{
		out->zip_code = 0;
		strcpy(out->street_name,"");
		strcpy(out->city_name,"");
		strcpy(out->state_name,"");
	}
	if (  ret = get_address( &s ) )	
	{	/* is an address */
	  	//printf("address is: %.*s.\n", s.input_begin - str, str );
	  ret = s.input_begin - str; /* Jerry Yu's hack */
	  /* hack for USA address */
	  if ( strncmp( s.input_begin, " USA", 4 ) == 0 )
		  ret +=4;
	  else if ( strncmp( s.input_begin, " usa", 4 ) == 0 )
		  ret +=4;
	  else if ( strncmp( s.input_begin, " U.S.A.", 7 ) == 0 )
		  ret +=7;
	  else if ( strncmp( s.input_begin, " U.S.", 5 ) == 0 )
		  ret +=5;
	  else if ( strncmp( s.input_begin, " US", 3 ) == 0 )
		  ret +=3;
	}
	/*else
	  {
	     printf("not address %s.\n", str );
	  }
	*/
	
	return ret;
}