#include <stdio.h>
#include <stdlib.h>
#include "cgiparser.h"
#include "GetAddress.h"
#include <ctype.h>
#include "string.h"

/**
 * Get the cgi query string and extract the query parameter 'q=' which should be an URL.
 * The extractor then will grab the web page, extract addresses from it, and show user the extracted addresses.
 * The address sections in the web page will be highlighted.
 */ 
int main(int argc, char *argv[])
{
   char* query;
   char buff[64*1000];
   char *url = buff;

   printf("Content-type: text/html\n\n");

   query = getenv("QUERY_STRING");
   
   //query = "geo=www.google.ca/intl/en/corporate/address.html";
   //query = "geo=http://www.revenue.state.co.us/TPS_Dir/wrap.asp?incl=revenuemail";
   if (argc>1)
   {
      query = argv[1];
   }
     
   if (query && strlen (query) > 0)
   { 

      //given query, get the url
      GetStringComponent2("geo", query, url);

      if ( strlen(url) > 0) 
      {
         // bypass space
         while (isspace(*url))
         {
            ++url;
         }
      
         // add http:// to the begin of url, if it doesn't have http://
         if ( strncmp(url, "http", 4) != 0) 
         { 
            // not start from http, insert http to the begain
            //move url content back 7 position for "http://"
            memmove( url+7, url, strlen(url)+1 );
            strncpy(url, "http://", 7);
         }
      
         //printf("url: %s\n",url);
         
         /**
          * Extract and diplay addresses
          */
         getAddress (url);
         
      }
      else
      {
         printf("please input url\n");
      }
   }
   else
   {
      printf("Please specify url!\n");
   }

   return 0;
}
