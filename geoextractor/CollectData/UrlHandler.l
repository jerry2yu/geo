%option noyywrap
%option never-interactive
%option prefix="html" 
%option outfile="UrlHandler.c"
%s begin_href
%s begin_img_href
%s begin_form_href
%s end_href
%{

/**
 * File: UrlHandler.l
 * --------------------
 * scan one html file and convert all its relative urls into absolute urls
 * and output the html
 * Update: Feb 2, 2005 by Zheyuan Yu
 */
 
#include "UrlHandler.h"
#include "assert.h"

/**
 * tags 
 */
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
\<[^\>]*{A}[^\>]*{HREF}[ ]*[=][ '"]*            { printf("%s",htmltext); BEGIN(begin_href); }
\<[^\>]*{IMG}[^\>]*{SRC}[ ]*[=][ '"]*           { printf("%s",htmltext); BEGIN(begin_img_href); }
\<[^\>]*{FORM}[^\>]*{ACTION}[ ]*[=][ '"]*       { printf("%s",htmltext); BEGIN(begin_form_href); }
['"\>]                                          { printf("%s",htmltext); BEGIN(end_href); }
<begin_href>[^'"\>]*                            { htmlloc += htmlleng; return URL; }
<begin_img_href>[^'"\>]*                        { htmlloc += htmlleng; return IMGURL; }
<begin_form_href>[^'"\>]*                       { htmlloc += htmlleng; return FORMURL; }
.                                               { htmlloc += htmlleng; return UNKNOWN; }
%%

/**
 * Converts and shows an relative url in absolute url format
 * linkUrl is url in the page of page_url
 * e.g. given page_url: http://www.google.com/about/address.html
 *         linkUrl: news.html
 *         will output http://www.goole.com/about/news.html
 *         if linkUrl: /new.html
 * @param linkUrl - an url in the page
 * @param pageUrl - url of the page
 * @param isGeoUrlAppended:  whether geo url appeded to the begin of extracted url: 0: no 1: yest
 */
void displayUrl (char* pageUrl, char* linkUrl, int isGeoUrlAppended) 
{
   int i, len = strlen(pageUrl);
   char* absolutePath = (char*)malloc(len+1);
   char* domainUrl = (char*)malloc(len+1);

   strcpy(absolutePath, pageUrl);
   strcpy(domainUrl, pageUrl);

   // get domain of given url
   // e.g.: given url http://www.google.com/about/address.html, will save http://www.google.com to domainUrl
   for (i=len; i>0; i--)
   {
      if (pageUrl[i] == '/') 
      {
         if (pageUrl[i-1] == '/' ) // is "//"
         {
            break;
         }
         else   // not "//"
         {
            domainUrl[i] = '\0';
         }
      }
   }
   
   /*if (strcmp(linkUrl, "/search")==0) {
   printf("\npageUrl: %s\n", pageUrl);
   printf("\ndomainUrl: %s\n", domainUrl);
   printf("\nlinkUrl: %s\n", linkUrl);
   }
   */
   
   // get absolutePath of given url
   //e.g.: given url http://www.google.com/about/address.html, will save http://www.google.com/about to absolutePath
   
   for (i=len; i>0; i--) 
   {
      if ( (pageUrl[i] == '/') && (pageUrl[i-1] != '/' ) ) 
      { // not "//"
         absolutePath[i] = '\0';
         break;
      }
   }
   
   if (isGeoUrlAppended == 1)
      printf("%s", GEO_URL);
      
   if (strncmp(linkUrl, "http", 4) == 0) //full absolute url
   {
      printf("%s", linkUrl);
   }
   else
   {
      if ( *linkUrl == '/' ) // relative url, root path
      {
         printf("%s%s", domainUrl, linkUrl);
      }
      else
      {
         printf("%s/%s", absolutePath, linkUrl);
      }
   }
    
   free(domainUrl);
   free(absolutePath);
}

/**
 * Process the url
 */
void processUrl(char* url)
{
   int tok;
   while ( (tok = htmllex()) ) {
      //printf("token: %s, tok: %d\n",htmltext);
      switch (tok) {
         case URL:
            //printf("url: %s\n<br>",htmltext);
            displayUrl(url, htmltext, 1);
            break;
         case IMGURL:
            //printf("image url: %s\n<br>",htmltext);
            displayUrl(url, htmltext, 0);
            break;
         case FORMURL:
            //printf("form url: %s[end]\n",htmltext);
            displayUrl(url, htmltext, 1);
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
   // make sure the string buffer is large enough.
   //realloc(html,strlen(html)+2);
   
   if (html) 
   {
      //append a '\0' to the end of string to make sure it is end with two '\0' for flex to scan
      *(html+strlen(html)+1) = '\0';

      YY_BUFFER_STATE buffer_state =yy_scan_string (html);

      // process the url
      processUrl (url);

      yy_delete_buffer(buffer_state);
   }
   else
   {
      printf("out of memory when convert text to tokens!\n");
      exit(0);
   }
   
}
