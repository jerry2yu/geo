#include <stdio.h>
#include <stdlib.h>
#include "cgiparser.h"
#include <ctype.h>
#include "string.h"
#include "config.h"

#define MAX_LEN      2000

/**
 * list all correctly and incorrectly tagge files
 * rightOrWrong: 0: right , then list only right urls
               1: wrong,then list only wrong tagged urls
   */

void listFiles(int rightOrWrong) 
{
   /*open url file, compare each url in the file with given url, 
     to see whether it is saved already */
   FILE *urlFile;
   int max_display_url=50;
   fpos_t pos;
   int len, id=-1, pre_rightOrWrong=-1, pre_numRight=-1, pre_numExtracted=-1, pre_numTotal=-1;
   int total_numRight=0, total_numExtracted=0, total_numTotal=0;
   int total_numPage=0;
   char buffer[MAX_LEN], data[MAX_LEN], *p1, *p2;

   //printf("open file %s<br>\n", PATH_WEBPAGE_URL);
   urlFile = fopen(PATH_WEBPAGE_URL, "r+");
   if (!urlFile) 
   {
      urlFile = fopen(PATH_WEBPAGE_URL, "w+");
      if (!urlFile) 
      {
         printf("Can't create/open URL storage file %s\n", PATH_WEBPAGE_URL);
         exit (0);
      }
   }
   
   while ( (fgetpos( urlFile, &pos)==0) && (fgets(buffer, MAX_LEN, urlFile) != NULL) ) 
   {
      //   printf("%s<br>\n",buffer);
      //number of pages
      total_numPage ++;
      // get id column
      p1 = strchr(buffer, '\t');

      if ( !p1 ) 
      {
         printf("Wrong file format\n");
         exit(0);
      }
      
      len = p1-buffer+1;
      strncpy(data, buffer, len);
      data[len] = '\0';
      //   printf("id: %s<br>\n", data);
      
      if (id<atoi(data))
         id = atoi(data);

      // get right/wrong column
      p2 = strchr(p1+1, '\t');
      
      if (!p2) 
      {
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
      if (!p2) 
      {
         printf("Wrong file format\n");
         exit(0);
      }
      
      len = p2 - p1 -1;
      strncpy(data, p1+1, len);
      data[len]= '\0';
      pre_numRight = atoi(data);
      //printf("numRight: %d<br>\n", pre_numRight);

      //get numExtracted column
      p1 = p2;
      p2 = strchr(p1+1, '\t');
      if (!p2) 
      {
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
      if (!p2) 
      {
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
      //   printf("url: %s<br>\n", data);
      
      if (strlen(data)<=0) 
      {
         printf("Wrong file format\n");
         exit(0);
      }

      total_numRight +=pre_numRight;
      total_numExtracted += pre_numExtracted;
      total_numTotal += pre_numTotal;
 
      if (pre_rightOrWrong == rightOrWrong) 
      {//output url

         printf("(%d-%d/%d/%d) <a href='%s%s'>",id, pre_numRight, pre_numExtracted, pre_numTotal,GEO_URL,data);
         if ((int)strlen(data)>max_display_url) {
            data[max_display_url-3]='.';
            data[max_display_url-2]='.';
            data[max_display_url-1]='.';
            data[max_display_url]='\0';
         }

         printf("%s</a><br>\n",data);
      }
      


   }

   if(rightOrWrong == 0) 
   {
      printf("Total %d addresses in %d webpages, %d addresses extracted, among them %d are correct<br>\n", total_numTotal, total_numPage, total_numExtracted, total_numRight);
      printf("<br>Precision: %.4f, Recall: %.4f<br>\n", (double)total_numRight/(double)total_numExtracted, (double)total_numRight/(double)total_numTotal);
   }

   fclose(urlFile);



}

int main(int argc, char *argv[]) 
{
   int rightOrWrong;
   printf("Content-type: text/html\n\n");
   
   printf("<br><table width=\"860\" border=0 cellspacing='4'>\n");
   printf("<tr><td width=\"50%%\"><b>Correctly Tagged</b></td><td width=\"50%%\"><b>Incorrectly Tagged</b></td></tr>\n");
   printf("<tr><td valign=top>\n");

   rightOrWrong = 0;
   listFiles(rightOrWrong);
   printf("</td><td valign=top>\n");
   rightOrWrong = 1;
   listFiles(rightOrWrong);
   printf("</td></tr></table>\n");


}
