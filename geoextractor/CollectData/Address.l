%option noyywrap 
%option never-interactive
%option prefix="address" 
%option outfile="Address.cpp"
%{

#include <iostream>
#include <fstream>
#include <ctype.h>
#include "SimpleExtract.h"
#include "SimpleDic.h"
#include "Address.h"

/**
 * Defines
 */
#define DOCNO 1
#define SCRIPT 2
#define DOCHDR 3
#define HEAD 4
#define HEADLINE 5
#define DOCTYPE 6
#define TAG 7
#define TITLE 8
#define ADDRESSUS 9 
#define ADDRESSCA 10
#define ADDRESSUK 11
#define PHONE 13
#define NEWLINE 20
#define WORD 21
#define UPWORD 22
#define CONTRACTION 23
#define ACRONYM 24
#define ACRONYM2 25
#define UNKNOWN 26


extern FILE * addressin;
extern char * addresstext;
long addresspos;
%}

DIGIT   [0-9] 
PHONESEPARATOR ([(]|[)]|[ ]|[.]|[-])
POBOX (([P|p][.]?[ ]?[O|o][.]?[ ]?)([B|b][O|o][X|x])?)|("Post Office Box")|([B|b][O|o][X|x])
STREETNO ([0-9]{1,6})|"One"|"Two"|"Three"
USZIP [0-9]{5}([- ][0-9]{4})?
CAZIP [A-Za-z][0-9][A-Za-z][ ]?[0-9][A-Za-z][0-9]
UKZIP [A-Za-z]{1,2}([0-9]{1,2}|([0-9][A-Za-z]))[ ]?[0-9][A-Za-z]{2}
USSTATE ("AA")|("AE")|("AK")|("AL")|("AP")|("AR")|("AS")|("AZ")|("CA")|("CO")|("CT")|("DC")|("DE")|("FL")|("FM")|("GA")|("GU")|("HI")|("IA")|("ID")|("IL")|("IN")|("KS")|("KY")|("LA")|("MA")|("MD")|("ME")|("MH")|("MI")|("MN")|("MO")|("MP")|("MS")|("MT")|("NC")|("ND")|("NE")|("NH")|("NJ")|("NM")|("NV")|("NY")|("OH")|("OK")|("OR")|("PA")|("PR")|("PW")|("RI")|("SC")|("SD")|("TN")|("TX")|("UT")|("VA")|("VI")|("VT")|("WA")|("WI")|("WV")|("WY")("A.A.")|("A.E.")|("A.K.")|("A.L.")|("A.P.")|("A.R.")|("A.S.")|("A.Z.")|("C.A.")|("C.O.")|("C.T.")|("D.C.")|("D.E.")|("F.L.")|("F.M.")|("G.A.")|("G.U.")|("H.I.")|("I.A.")|("I.D.")|("I.L.")|("I.N.")|("K.S.")|("K.Y.")|("L.A.")|("M.A.")|("M.D.")|("M.E.")|("M.H.")|("M.I.")|("M.N.")|("M.O.")|("M.P.")|("M.S.")|("M.T.")|("N.C.")|("N.D.")|("N.E.")|("N.H.")|("N.J.")|("N.M.")|("N.V.")|("N.Y.")|("O.H.")|("O.K.")|("O.R.")|("P.A.")|("P.R.")|("P.W.")|("R.I.")|("S.C.")|("S.D.")|("T.N.")|("T.X.")|("U.T.")|("V.A.")|("V.I.")|("V.T.")|("W.A.")|("W.I.")|("W.V.")|("W.Y.")|("Armed Forces Africa")|("Armed Forces Americas")|("Armed Forces Canada")|("Armed Forces Europe")|("Armed Forces Middle East")|("Armed Forces Pacific")|("Alabama")|("Alaska")|("American Samoa")|("Arizona")|("Arkansas")|("California")|("Colorado")|("Connecticut")|("Delaware")|("District of Columbia")|("Federated States of Micronesia")|("Florida")|("Georgia")|("Guam")|("Hawaii")|("Idaho")|("Illinois")|("Indiana")|("Iowa")|("Kansas")|("Kentucky")|("Louisiana")|("Maine")|("Marshall Islands")|("Maryland")|("Massachusetts")|("Michigan")|("Minnesota")|("Mississippi")|("Missouri")|("Montana")|("Nebraska")|("Nevada")|("New Hampshire")|("New Jersey")|("New Mexico")|("New York")|("North Carolina")|("North Dakota")|("Northern Mariana Islands")|("Ohio")|("Oklahoma")|("Oregon")|("Palau")|("Pennsylvania")|("Puerto Rico")|("Rhode Island")|("South Carolina")|("South Dakota")|("Tennessee")|("Texas")|("Utah")|("Vermont")|("Virgin Islands")|("Virginia")|("Washington")|("West Virginia")|("Wisconsin")|("Wyoming") 
USA  ([U|u][.]?[ ]?[S|s][.]?[ ]?[A|a][.]?)|([U|u][N|n][I|i][T|t][E|e][D|d][ ]?[S|s][T|t][A|a][T|t][E|e][S|s])
CANADA [C|c][A|a][N|n][A|a][D|d][A|a]
UK  ([U|u][.]?[ ]?[K|k][.]?)|([U|u][N|n][I|i][T|t][E|e][D|d][ ]?[K|k][I|i][N|n][G|g][D|d][O|o][M|m])
TEL (([T|t][E|e][L|l])|([F|f][A|a][X|x])|([P|p][H|h][O|o][N|n][E|e]))

%%

({POBOX}[ ])?{STREETNO}[ ].{2,60}[ ,]+{USSTATE}[ ,.]+{USZIP}([, ]?({USA}))?   {addresspos+=addressleng;return ADDRESSUS;}
({POBOX}[ ])?{STREETNO}[ ].{2,60}[ ,.]+{CAZIP}([ ]?({CANADA}))?   {addresspos+=addressleng;return ADDRESSCA;}
({POBOX}[ ])?{STREETNO}[ ].{2,60}[ ,.]+{UKZIP}([ ]?({UK}))?   {addresspos+=addressleng;return ADDRESSUK;}
.         {addresspos +=addressleng; return UNKNOWN;}

%%

void flex_address_match(int* positions, cvector *addressVector)
{
   int tok=0;
   SimpleExtract se;
   Address adr;
   
   while ((tok = addresslex())) 
   {
      switch (tok) 
      {
      case ADDRESSUS:
          
         // An USA address is extract
          
         //tokenize and look up dictionary to confirm the potential address
         // cout<<addresstext<<endl; 
         //  w=se.is_us_address(addresstext,addressleng);
         // if (w>0)
         //    cout<<addresstext<<"<br>"<<endl;
         //    else
         //       cout<<"not us address "<<addresstext<<endl;
         
         adr.address =(char*)malloc(strlen(addresstext)+1);
         strcpy(adr.address, addresstext);
         
         // save the text start position
         adr.start = *(positions+addresspos-addressleng);
         
         // printf("address: %s\n", addresstext);
         //printf("strlen: %d\n", strlen(addresstext));
         
         // save the text end position
         adr.end = *(positions+addresspos-1);
         
         //address country: USA
         adr.country = 0;
         
         //append address to addressVector
         VectorAppend(addressVector, &adr);
      break;
      
      case PHONE:
        
        // A phone number is extracted
        
        printf ("PHONE: %s\n", addresstext);

       break;
       
    case ADDRESSCA:
       
       // A Canadian address is extracted
       
       //cout<<addresstext<<"<br>"<<endl;
       
       adr.address =(char*)malloc(strlen(addresstext)+1);
       strcpy(adr.address, addresstext);
       adr.start = *(positions+addresspos-addressleng);
       adr.end = *(positions+addresspos-1);
       adr.country = 1; //address country: CANADA
       //append address to addressVector
       VectorAppend(addressVector, &adr);
    break;
    
    case ADDRESSUK:
    
       // An UK address is extracted
       
       // cout<<addresstext<<endl;
       adr.address =(char*)malloc(strlen(addresstext)+1);
       strcpy(adr.address, addresstext);
       adr.start = *(positions+addresspos-addressleng);
       adr.end = *(positions+addresspos-1);
       adr.country = 2; //address country: UK
       //append address to addressVector
       VectorAppend(addressVector, &adr);
       
       
    break;
    
    case NEWLINE:
    break;
    
    case UNKNOWN:
       //cout<<addresstext<<endl;
    break;
    
    default:
    break;

    }
  }
}

void setString(char* s)
{
  yy_scan_string(s);
}

void extractAddress (char* s, int* positions, cvector *addressVector)
{
   assert(s);
   
   if ( !realloc( s, strlen(s)+2 ) )
   {
      printf("out of memory when convert text to tokens!\n");
      exit(0);
   }
   
   //append a '\0' to the end of string to make sure it is end with two '\0' for flex to scan
   *(s+strlen(s)+1) = '\0';

   YY_BUFFER_STATE buf_state = address_scan_string( s );
   
   // printf("%s\n",s);

   //extract address from given input token vector (positions),
   // and output extracted addresses in addressVector
   flex_address_match (positions, addressVector);
   
   address_delete_buffer(buf_state);
}

#ifdef _STANDALONE
int main(int argc, char *argv[])
{
  if (argc>1)
      yyin = fopen(argv[1], "r");
  if (!yyin) yyin=stdin;
  flex_match();
  return 0;
}
#endif
