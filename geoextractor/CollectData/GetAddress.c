#include "HttpFetcher.h"
#include "cvector.h"
#include "config.h"
#include "text2token.h"
#include "Address.h"
#include "GetAddress.h"
#include "UrlHandler.h"
#define DEF_BUFF_SIZE 1024*1024
/** 
 * Function: free_token
 * ---------------------------------
 * free_token defines the space of functions that can be used as the
 * clean-up function for each element as it is deleted from the vector
 * or when the entire cvector is destroyed.  The cleanup function is 
 * called with a pointer to an element about to be deleted.
 */
void free_token(void *elemAddr) {
	assert(elemAddr);
//	NGram * n=(NGram*)elemAddr;
//	printf("%s\n",n->ngram);
	free (((Token*)elemAddr)->term);
}
/* free address vector element */
void free_address(void *elemAddr) {
	assert(elemAddr);
	free (((Address*)elemAddr)->address);
}
/* convert text to tokens, remove tags
   and convert back to string: tokens
   and keep all positions in array "positions"
   array: text, tokens, positions must be released by the caller program.
*/
void convertToken2Text( char*text, char* tokens, int** positions) {
	cvector tokenVector;
	int* p;
	char* t = tokens;
	Token *token;
	int i,j,len;
	assert(text);
	assert(tokens);
	assert(positions);
        VectorNew (&tokenVector, sizeof (Token),free_token, DEF_VECTOR_SIZE);
	getTokensFromText(text, &tokenVector);
	assert (text);
	for (i=0;i<tokenVector.ItemsCount;i++) {
		token = (Token*)VectorNth(&tokenVector,i);
		sprintf( t, "%s", token->term);
		t+=strlen(token->term);
		//append ' ' to the end of each token
		if (i!=tokenVector.ItemsCount)
			*t++=' ';
	} 
	*t='\0';
	//record the position of token in the text
	*positions = (int*)malloc( sizeof(int) * strlen(text) );

	if (!*positions) {
		printf("Not enough memory!\n");
		exit(0);
	}
	p=*positions;
    for (i=0;i<tokenVector.ItemsCount;i++) {
	    token = (Token*)VectorNth(&tokenVector,i);
		len = strlen(token->term);
		//printf("\ntoken: %s\n", token->term);
		//record position of the token (each char will has a position, and value is the positon of first character of the token)
		for (j=0;j<len;j++) {
			*p++ = token->position+j;
			//printf("%c->%d ",*(text+token->position+j), token->position+j);
		}
		if (i!=tokenVector.ItemsCount-1)
			*p++= token->position+j-1; //position as next token
    }

	VectorDispose(&tokenVector);

}
	//get highlighed html text into textHighlight
	// give source text, and addressVector
void getHighlight(const char* s, char* d, const cvector *addressVector) {
	Address *adr;
	const char *tag;
	int cur_s = 0; //cursor of source text
	int cur_d = 0; //cursor of dest text
	int index;
	int i,len;

	//iterate through all extracted addresses
	for (i=0; i<addressVector->ItemsCount; i++) {
		adr = (Address*)VectorNth(addressVector,i);
		//printf("%s<br>\n", adr->address);
        //copy begin to start of address part
		strncpy(d+cur_d, s+cur_s, adr->start - cur_s);
		cur_d += adr->start - cur_s ;
		cur_s += adr->start - cur_s ;

		switch (i%2) {
			case 0:
				tag = HIGHLIGHT_TAG0;
				break;
			case 1:
				tag = HIGHLIGHT_TAG1;
				break;
			default:
				tag = HIGHLIGHT_TAG0;
		}

		index=cur_s;
		while (cur_s <= adr->end) {

			//copy start tag
			strcpy(d+cur_d, tag);
			cur_d += strlen(tag);
			
			//copy address part
			while ( (cur_s <= adr->end) && ( *(s+cur_s)!='<' ) ) {
				d[cur_d++] = s[cur_s++];
			}
			
			//copy end tag
			strcpy(d+cur_d, HIGHLIGHT_TAG_END);
			cur_d += strlen(HIGHLIGHT_TAG_END);

			//copy the html tags
			while ( (cur_s <= adr->end) && ( *(s+cur_s)!='>' ) ) {
				d[cur_d++] = s[cur_s++];
			}
			if ((cur_s <= adr->end) && ( *(s+cur_s)=='>' ))
				d[cur_d++] = s[cur_s++];

		}


		/*printf("%s, start: %d, end: %d<br>\n",adr->address, adr->start, adr->end);
		for (j=adr->start; j<=adr->end; j++)
			printf("%c",*(text+j));
		printf("\n");
		*/
	}
    len = strlen(s);
	//append remaind text to dest
    if (cur_s < len)
		strcpy(d+cur_d, s+cur_s);

}

	//tag given html text and save tagged text  into d
	// give source text, and addressVector
void tagAddress(const char* s, char* d, const cvector *addressVector) {
	Address *adr;
	const char *tag_start;

	int cur_s = 0; //cursor of source text
	int cur_d = 0; //cursor of dest text
	int i,len;


	//iterate through all extracted addresses
	for (i=0; i<addressVector->ItemsCount; i++) {
		adr = (Address*)VectorNth(addressVector,i);
		//printf("%s<br>\n", adr->address);
        //copy begin to start of address part
		strncpy(d+cur_d, s+cur_s, adr->start - cur_s);
		cur_d += adr->start - cur_s ;
		cur_s += adr->start - cur_s ;




		//copy start tag
		if(adr->country == 0)//US address
			tag_start = GEO_TAG_START_US;

		if(adr->country == 1)//CA address
			tag_start = GEO_TAG_START_CA;

		if(adr->country == 2)//UK address
			tag_start = GEO_TAG_START_UK;

		strcpy(d+cur_d, tag_start);
		cur_d += strlen(tag_start);

        //copy address part
		strncpy(d+cur_d, s+cur_s, adr->end - adr->start+1);
		cur_d += adr->end - adr->start+1;
		cur_s += adr->end - adr->start+1;

		//copy end tag
		strcpy(d+cur_d, GEO_TAG_END);
		cur_d += strlen(GEO_TAG_END);


		/*printf("%s, start: %d, end: %d<br>\n",adr->address, adr->start, adr->end);
		for (j=adr->start; j<=adr->end; j++)
			printf("%c",*(text+j));
		printf("\n");
		*/
	}
    len = strlen(s);
	//append remaind text to dest
    if (cur_s < len)
		strcpy(d+cur_d, s+cur_s);

}
	/*
	check url file, to see whether this url already in the list
	if not append it to the file, and save tagged webpage to file f0000newid
	if exists, update the url file: id right/wrong @numRight @numWrong url
	@numRight: total number of correctly extracted addresses
	@numExtracted: total number of addressed extracted by rule base algrithm
	@numTotal: total number of addresses in the webpage
	@numRight, @numExtracted and @numTotal will be used to calculate recall and precision
	*/

void saveTaggedText(char* url, char* originalText, char* taggedText, int rightOrWrong, int numRight, int numExtracted, int numTotal) {
	/*open url file, compare each url in the file with given url, 
	  to see whether it is saved already */
	FILE *urlFile, *oFile;
	int MAX_LEN = 2000;
	fpos_t pos;
	int len, id=-1, found=0, pre_rightOrWrong=-1, pre_numRight=-1, pre_numExtracted, pre_numTotal=-1;
	char buffer[MAX_LEN], data[MAX_LEN], *p1, *p2;
	char* new_url;
	assert(strlen(url)>0);
	assert(url);
	assert(originalText);
	assert(taggedText);
	assert(rightOrWrong==0 ||rightOrWrong==1);
	len = strlen(url)-1;
	//remove '/' from end of url
	new_url	= (char*)malloc(strlen(url));
	strcpy(new_url, url);
	while( isspace(new_url[len]) ) 
		len --;
	if (new_url[len]== 47) // '/'
		new_url[len] = '\0';

	//printf("open file %s<br>\n", PATH_WEBPAGE_URL);
	urlFile = fopen(PATH_WEBPAGE_URL, "r+");
	if (!urlFile) {
		urlFile = fopen(PATH_WEBPAGE_URL, "w+");
		if (!urlFile) {
			printf("Can't create/open file %s to save url\n", PATH_WEBPAGE_URL);
			exit (0);
		}
	}
	while ( (fgetpos( urlFile, &pos)==0) && (fgets(buffer, MAX_LEN, urlFile) != NULL) ) {
	//	printf("%s<br>\n",buffer);

		// get id column
		p1 = strchr(buffer, '\t');

		if ( !p1 ) {
			printf("Wrong file format\n");
			exit(0);
		}
		len = p1-buffer+1;
		strncpy(data, buffer, len);
		data[len] = '\0';
		//printf("id: %s<br>\n", data);
		if (id<atoi(data))
			id = atoi(data);

		// get right/wrong column
		p2 = strchr(p1+1, '\t');
		if (!p2) {
			printf("Wrong file format\n");
			exit(0);
		}
		len = p2 - p1 -1;
		strncpy(data, p1+1, len);
		data[len]= '\0';
		pre_rightOrWrong = atoi(data);
		//printf("right/wrong: %s<br>\n", data);


		// get numRight column
		p1 = p2;
		p2 = strchr(p1+1, '\t');
		if (!p2) {
			printf("Wrong file format\n");
			exit(0);
		}
		len = p2 - p1 -1;
		strncpy(data, p1+1, len);
		data[len]= '\0';
		pre_numRight = atoi(data);
		//printf("numRight: %d<br>\n", pre_numRight);
		
		// get numExtracted column
		p1 = p2;
		p2 = strchr(p1+1, '\t');
		if (!p2) {
			printf("Wrong file format\n");
			exit(0);
		}
		len = p2 - p1 -1;
		strncpy(data, p1+1, len);
		data[len]= '\0';
		pre_numExtracted = atoi(data);
		//printf("numExtracted: %d<br>\n", pre_numExtracted);


		// get numTotal column
		p1 = p2;
		p2 = strchr(p1+1, '\t');
		if (!p2) {
			printf("Wrong file format\n");
			exit(0);
		}
		len = p2 - p1 -1;
		strncpy(data, p1+1, len);
		data[len]= '\0';
		pre_numTotal = atoi(data);
		//printf("numTotal: %d<br>\n", pre_numTotal);


		// get url column
		strcpy(data, p2+1);
		len = strlen(data);
		data[len-1]='\0';
		//printf("url: %s<br>\n", data);
		if (strlen(data)<=0) {
			printf("Wrong file format\n");
			exit(0);
		}
		if ( strcmp(data, new_url) == 0 ) {
			//url exists, update the url file
			fsetpos (urlFile,&pos);
			// move from current file position to right/wrong column
			/*if ( fseek( urlFile, p1+1-buffer,  SEEK_CUR )!=0 ) {
				printf("Wrong when update url file");
				exit(0);
			}*/
			fprintf(urlFile, "%d\t%d\t%5d\t%5d\t%5d\t%s\n", id, rightOrWrong, numRight, numExtracted, numTotal, new_url);

			//fputc('0'+rightOrWrong, urlFile);
			printf("Tagged data file has been updated<br>\n");
			found = 1;
			break;

		}
		


	}


	// not found in the url file
	if (!found)	{
		fprintf(urlFile, "%d\t%d\t%5d\t%5d\t%5d\t%s\n", ++id, rightOrWrong, numRight, numExtracted, numTotal, new_url);
		//new url, append it to url file
		printf("Tagged data file has been saved<br>\n");
	}
	fclose(urlFile);

	//ouput the original file
	strcpy(data, PATH_WEBPAGE_ORIGINAL);
	sprintf(data+strlen(PATH_WEBPAGE_ORIGINAL), "g%05d", id);
	//printf("file name: %s<br>\n", data);
	oFile = fopen(data, "w+");
	if (!oFile) {
		printf("Can't create/open file %s to save url\n", data);
		exit (0);
	}
	fprintf(oFile, "<DOC>\n<DOCNO>g%05d</DOCNO>\n<DOCHDR>%s</DOCHDR>\n",id, new_url);
	fprintf(oFile, "%s", originalText);
	fclose(oFile);

	//file existed, and rightOrWrong flag changed, so delete existed file.

	if ( found && (pre_rightOrWrong!=rightOrWrong) ) {
		if (pre_rightOrWrong) //wrong
			strcpy(data, PATH_WEBPAGE_TAGGED_INCORRECT);
		else 
			strcpy(data, PATH_WEBPAGE_TAGGED_CORRECT);
		sprintf(data+strlen(data), "f%05d", id);

		//delete pre saved file
		remove(data);

	}

	//ouput the tagged file
	if (rightOrWrong) //wrong
		strcpy(data, PATH_WEBPAGE_TAGGED_INCORRECT);
	else 
		strcpy(data, PATH_WEBPAGE_TAGGED_CORRECT);
	sprintf(data+strlen(data), "f%05d", id);
	//printf("file name: %s<br>\n", data);
	oFile = fopen(data, "w+");
	if (!oFile) {
		printf("Can't create/open file %s to save url\n", data);
		exit (0);
	}
	fprintf(oFile, "<DOC>\n<DOCNO>f%05d</DOCNO>\n<DOCHDR>%s</DOCHDR>\n",id, new_url);
	fprintf(oFile, "%s", taggedText);
	fclose(oFile);


	free(new_url);


}
int getAddress (char* url) {
	char *stream, *text, *textHighlight, *lenstr;
	char *tokens = (char*)malloc(DEF_BUFF_SIZE);
	int *positions;// array to record position of each token in the text
	cvector addressVector;
	Address *adr;

	long len;
	int MAXLEN = 1805;
    int EXTRA = 11;
	/* 4 for field name "data", 1 for "=" */
    int MAXINPUT = MAXLEN+EXTRA+2;
	char input[MAXINPUT];
	char* data = input, *p;
	int rightOrWrong = -1, numRight, numTotal;


	//char* domain_url;
	int i;
	http_setTimeout(8);//seconds 
	//fetch web page
	int ret = httpFetch (url, &stream);
	if (ret == -1) {
		printf("%s\n",http_strerror());
		exit(0);
	}
	//printf("ret: %d, strlen: %d\n",ret, strlen(stream));
	assert(stream);
	text= (char*)malloc(ret+2);
	if (!text) {
		printf("out of memory when convert text to tokens!\n");
		exit(0);
	}

	strncpy(text, stream, ret);
	//append a '\0' to the end of string to make sure it is end with two '\0' for flex to scan
	*(text+ret) = '\0';
	*(text+ret+1) = '\0';
	free(stream);



	/* convert text to tokens, remove tags
	and convert back to string: tokens
	and keep all positions in array "positions"
	*/
	convertToken2Text(text, tokens, &positions);

	// get base domain of given url
	//e.g. given http://www.google.com/address, return http://www.google.com to domain_url
	/* domain_url = (char*)malloc(strlen(url)+1);
	strcpy(domain_url, url);
	for (i=strlen(url); i>0; i--) {
	if (url[i] == '/') {
	if (url[i-1] == '/' ) // is "//"
	break;
	else   // not "//"
	domain_url[i] = '\0';
	}
	}
	printf ("<base href=\"%s%s\">\n", GEO_URL, domain_url);
	free(domain_url);
	*/
	VectorNew (&addressVector, sizeof (Address),free_address, DEF_ADDRESS_PER_PAGE);
	//extract address,
	//get position from positions vector
    //and save extracted address, position, country to addressVector
	extractAddress(tokens, positions, &addressVector);

	//display the parsed text
	//printf("tokens: %s\n",tokens);
	//printf("url: %s\n", url);
	//printf("domain_url: %s\n", domain_url);
	//output header

	printf("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=ISO-8859-1\">\n");
	printf("<table border=1 width=100%%><tr><td><table border=1 bgcolor=#ffffff cellpadding=10 cellspacing=0 width=100%% color=#ffffff><tr><td>\n");
	printf("<font face=arial,sans-serif color=black size=-1>\n");
	printf("<b><a href='%s'>US, UK & Canadian Addresses</a> extracted by <a href='%s'>Geo Extractor</a> from web page</b> <a href='%s'>%s</a></font><br><br>\n",LIST_FILES_URL, HOME_PAGE,url,url);

	//printf("%s,",textHighlight);
	//display extracted address
	//table header
	printf("<table width=100%% border=0 cellpadding=0 cellspacing=0><tr><td bgcolor=#3366cc><img width=1 height=1 alt=''></td></tr></table>\n");
	printf("<table width=100%% border=0 cellpadding=0 cellspacing=0 bgcolor=#e5ecf9><tr><td width=10></td><td bgcolor=#e5ecf9 nowrap><br>\n");
	printf("<font face=arial,sans-serif color=black size=-1><b>\n");

	for (i=0; i<addressVector.ItemsCount; i++) {
		adr = (Address*)VectorNth(&addressVector,i);
		printf("%s<br>\n", adr->address);
		/*printf("%s, start: %d, end: %d<br>\n",adr->address, adr->start, adr->end);
		for (j=adr->start; j<=adr->end; j++)
			printf("%c",*(text+j));
		printf("\n");
		*/

	}
	printf("</b></font>\n");
	printf("<br></td></tr></table>\n");
	printf("<table width=100%% border=0 cellpadding=0 cellspacing=0><tr><td bgcolor=#3366cc><img width=1 height=1 alt=''></td></tr></table>\n");

	textHighlight = (char*)malloc(DEF_BUFF_SIZE);
	numRight=numTotal =addressVector.ItemsCount;

	/* if there is a user post, we save the user input to get tagged data*/
	lenstr = getenv("CONTENT_LENGTH");
	if ( !(lenstr == NULL || sscanf(lenstr,"%ld",&len)!=1 || len > MAXLEN) ) {
		tagAddress(text, textHighlight, &addressVector);
		fgets(input, len+1, stdin);
		URLdecode(input);
		data = input+EXTRA;
		//printf("posted: %s\n",data);
		len = strlen("right");
		if ( strncmp(data, "right", len)==0 ) { 
			rightOrWrong = 0; //set flag for right or wrong extraction
		}
		len = strlen("wrong");
		if ( strncmp(data, "wrong", len)==0 ) {
			rightOrWrong = 1; //user input "Wrong Extraction"
		}

		// get user input: numRight, which is number of correct extracted address
		data += strlen("right") + strlen("&numRight=");
		p = data;
		while (*data++ !='&');
		*data= '\0';
		numRight = atoi(p);
		//printf("numRight: %d\n", numRight);

		// get user input numTotal, which is number of total address in the page
		p = data+strlen("numTotal=");
		numTotal = atoi(p);
		//printf("numTotal: %d\n", numTotal);



		if (rightOrWrong == 0) {
			//printf("webpage saved to RIGHT folder\n");
			saveTaggedText(url, text, textHighlight, rightOrWrong, numRight, addressVector.ItemsCount, numTotal);
		}
		if ( rightOrWrong == 1 ) {
			//printf("webpage saved to WRONG folder\n");
			saveTaggedText(url, text, textHighlight, rightOrWrong, numRight, addressVector.ItemsCount, numTotal);
		}

		//printf("tagged text: %s\n", textHighlight);
	}

	// give source text, and addressVector
	//highlight all extracted address in the webpage
	getHighlight(text, textHighlight, &addressVector);


	/* if there is at least one address extracted, show user input to let user 
	judge where extraction is correct*/


	if ( SHOW_COLLECT_DATA_INTERFACE) {

		printf("<FORM ACTION=\"%s%s\" METHOD=\"POST\">\n", GEO_URL, url);
		printf("<font face=arial,sans-serif color=black size=-1>\n");
		
		printf("<P><input name=\"extraction\" type=\"radio\" value=\"right\" ");
		if ((rightOrWrong == 0)||(rightOrWrong == -1)) //if no user input or user input: extracted address all correct
			printf("checked");
		printf("> All address extracted correctly<br>\n");
		printf("<input name=\"extraction\" type=\"radio\" value=\"wrong\" ");
		if (rightOrWrong == 1) //user input: extracted address all correct
			printf("checked");
		printf("> Not all addresses extracted correctly. \n");
		printf("<input type=\"text\" name=\"numRight\" size=\"4\" value=\"%d\"> addresses extracted correctly from total <input type=\"text\" name=\"numTotal\" size=\"4\" value=\"%d\"> addresses<BR>\n", numRight, numTotal);
		printf("<INPUT TYPE=\"SUBMIT\" VALUE=\"Save Webpage\"></font></FORM>\n");
		//show google search
		printf("<SCRIPT language=\"JavaScript\">function OnSubmitForm(){ document.g.action =\"%shttp://www.google.com/search?num=100&q=\"+document.g.q.value.replace(\" \",\"%%2B\");}</SCRIPT>\n", GEO_URL);
		printf("<table border=0 align=right><tr><td>\n");
		printf("<form action=\"\" method=\"post\" name=\"g\" onSubmit=\"return OnSubmitForm();\">\n");
		printf("<input size=\"32\" name=\"q\">\n");
		printf("<INPUT TYPE=\"SUBMIT\" name=\"Submit\" VALUE=\"Google\"></form>\n");
		printf("</td></tr></table>\n");

	}
	printf("</td></tr></table></td></tr></table>\n");



	//extract address from original html text
  // extract_address(text);
 // printf("Original <hr>%s",text);
  printf("<hr>\n");
  //printf("%s",textHighlight);
  displayHtmlAbsoluteURL(textHighlight, url);

  VectorDispose(&addressVector);
  free (positions);
  free (text);
  free (tokens);
  

  return 0;

}

