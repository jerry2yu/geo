#include <stdio.h>
#include <stdlib.h>
#include "cgiparser.h"
#include "getAddress.h"
#include <ctype.h>
#include "string.h"

int main(int argc, char *argv[])
{
	char* query;
	char buff[64*1000];
	char *url = buff;

	printf("Content-type: text/html\n\n");


	query = getenv("QUERY_STRING");
	//query = "q=www.google.ca/intl/en/corporate/address.html";
	//query = "q=http://www.revenue.state.co.us/TPS_Dir/wrap.asp?incl=revenuemail";
	if (argc>1)
		query = argv[1];
	/* This function reverses the mods that http does
	* to special charachters and blanks
	*/
	// urlEncode = URLencode(query);
	// printf("after encoding: %s\n",urlEncode);
	//URLdecode(query);
	// printf("after decoding: %s\n",query);

	//	free(urlEncode);
	if (query==NULL) {
		printf("Please specify url!");
		return 0;
	}
	if (strlen(query)<=0) {
		printf("Please specify url!");
		return 0;
	}

	//  URLdecode(query);

	//  printf("%s\n",query);

	//given query, get the url
	GetStringComponent2("geo", query, url);

	if (strlen(url) <= 0) {
		printf("please input url\n");
		return 0;
	}
	// bypass space
	while (isspace(*url)) url++;
	//printf("url:%s\n",url);
	// add http:// to the begin of url, if it doesn't have http://
	if ( strncmp(url, "http", 4) != 0) { // not start from http, insert http to the begain
		//move url content back 7 position for "http://"
		memmove( url+7, url, strlen(url)+1 );
		strncpy(url, "http://", 7);
		//printf("%s\n",url);
	}




	//printf("url: %s\n",url);
	//printf ("<html><head><title>%s</title></head><body bgcolor=#ffffff>";
	getAddress (url);
	//  printf("</body></html>");
	return 0;
}
