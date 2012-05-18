%option noyywrap
%option never-interactive
%option prefix="html" 
%option outfile="displayHtml.c"
%s begin_href
%s begin_img_href
%s begin_form_href
%s end_href
%{

/**
 * File: displayHtml.l
 * --------------------
 * scan html file and convert all relative url into absolute url path
 * and output the html
 * Update: Feb 2, 2005 by Zheyuan Yu
 */
 
#include "displayHtml.h"
#include "assert.h"
/* tags */
#define URL 1
#define IMGURL 2
#define FORMURL 3
#define UNKNOWN 100

long htmlloc;


%}
A (A|a)
IMG (I|i)(M|m)(G|g)
HREF (H|h)(R|r)(E|e)(F|f)
SRC (S|s)(R|r)(C|c)
FORM (F|f)(O|o)(R|r)(M|m)
ACTION (A|a)(C|c)(T|t)(I|i)(O|o)(N|n)

%%
\<[^\>]*{A}[^\>]*{HREF}[ ]*[=][ '"]*			{printf("%s",htmltext); BEGIN(begin_href);}
\<[^\>]*{IMG}[^\>]*{SRC}[ ]*[=][ '"]*			{printf("%s",htmltext); BEGIN(begin_img_href);}
\<[^\>]*{FORM}[^\>]*{ACTION}[ ]*[=][ '"]*			{printf("%s",htmltext); BEGIN(begin_form_href);}
['"\>]			{ printf("%s",htmltext); BEGIN(end_href);}
<begin_href>[^'"\>]*         { htmlloc += htmlleng; return URL; }
<begin_img_href>[^'"\>]*         { htmlloc += htmlleng; return IMGURL; }
<begin_form_href>[^'"\>]*         { htmlloc += htmlleng; return FORMURL; }

.                                       {htmlloc += htmlleng; return UNKNOWN; }
%%



/**
 * display link_url in absolute url format
 * link_url is url in the page of page_url
 * e.g. given page_url: http://www.google.com/about/address.html
			link_url: news.html
			will output http://www.goole.com/about/news.html
			if link_url: /new.html
 * @append_geo_url: 0: not append geo_url, 1: append geo url to the begin of extracted url
 */
void display_url(char* page_url, char* link_url, int append_geo_url) {
	int i, len = strlen(page_url);
	char* absolute_path = (char*)malloc(len+1);
	char* domain_url = (char*)malloc(len+1);
	//char *encode_domain_url, *encode_absolute_path, *encode_link_url;
	strcpy(absolute_path, page_url);
	strcpy(domain_url, page_url);

	// get domain of given url
	//e.g.: given url http://www.google.com/about/address.html, will save http://www.google.com to domain_url
	for (i=len; i>0; i--) {
		if (page_url[i] == '/') {
		    if (page_url[i-1] == '/' ) // is "//"
				break;
			else   // not "//"
		    	domain_url[i] = '\0';
		}
	}
	/*if (strcmp(link_url, "/search")==0) {
	printf("\npage_url: %s\n", page_url);
	printf("\ndomain_url: %s\n", domain_url);
	printf("\nlink_url: %s\n", link_url);
	}
	*/
	
	// get absolute_path of given url
	//e.g.: given url http://www.google.com/about/address.html, will save http://www.google.com/about to absolute_path
	for (i=len; i>0; i--) {
		if ( (page_url[i] == '/') && (page_url[i-1] != '/' ) ) { // not "//"
			absolute_path[i] = '\0';
			break;
		}
	}
	//encode url, after encoding, max string length expansion is 3 times
	//encode_domain_url = encode(domain_url);
	//encode_absolute_path = encode(absolute_path);
	//encode_link_url = encode(link_url);
	if (append_geo_url == 1)
		printf("%s", GEO_URL);
		
	if (strncmp(link_url, "http", 4) == 0) //full absolute url
		printf("%s", link_url);
	else
		if ( *link_url == '/' ) // relative url, root path
			printf("%s%s", domain_url, link_url);
		else
			printf("%s/%s", absolute_path, link_url);

    
    //free(encode_domain_url);
    //free(encode_absolute_path);
    //free(encode_link_url);
    free(domain_url);
    free(absolute_path);
	

}
void url_match(char* url)
{
	int tok;
	while ( (tok = htmllex()) ) {
		//printf("token: %s, tok: %d\n",htmltext);
		switch (tok) {
			case URL:
				//printf("url: %s\n<br>",htmltext);
				display_url(url, htmltext, 1);
				break;
			case IMGURL:
				//printf("image url: %s\n<br>",htmltext);
				display_url(url, htmltext, 0);
				break;
			case FORMURL:
				//printf("form url: %s[end]\n",htmltext);
				display_url(url, htmltext, 1);
				break;

			case UNKNOWN:
				printf("%s",htmltext);
				break;
			default:
				printf("%s",htmltext);
				break;
		}
    }
}




/**
 * Function: displayHtmlAbsoluteURL 
 * --------------------
 * scan html file and convert all relative url into absolute url path, by append given url to relative html
 * and output the html
 * Update: Feb 2, 2005 by Zheyuan Yu
 */

 void displayHtmlAbsoluteURL (char* html, char* url)
 {
 	assert(html);
	realloc(html,strlen(html)+2);
	if (!html) {
		printf("out of memory when convert text to tokens!\n");
		exit(0);
	}
	//append a '\0' to the end of string to make sure it is end with two '\0' for flex to scan
	*(html+strlen(html)+1) = '\0';

	YY_BUFFER_STATE buffer_state =yy_scan_string (html);
	url_match(url);
	yy_delete_buffer(buffer_state);
 }




