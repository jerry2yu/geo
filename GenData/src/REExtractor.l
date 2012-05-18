/**
 * Extracting addresses from source text
* ({POBOX}[ ])?{STREETNO}[ ].{2,60}[ ,]+{USSTATE}[ ,.]+{USZIP}([, ]?({USA}))?   {addressloc+=addressleng;return ADDRESSUS;}
 */
%option noyywrap
%option never-interactive
%option prefix="address" 
%option outfile="REExtractor.cpp"
%{


/* address patterns */
#define UNKNOWN 4
#define ADDRESSUS 1
#define ADDRESSCA 2
#define ADDRESSUK 3

extern FILE * addressin;
extern char * addresstext;
long addressloc;


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
({POBOX}[ ])?{STREETNO}[ ].{2,60}[ ,]+{USSTATE}[ ,.]+{USZIP}([, ]?({USA}))?   {addressloc+=addressleng;return ADDRESSUS;}
({POBOX}[ ])?{STREETNO}[ ].{2,60}[ ,.]+{CAZIP}([ ]?({CANADA}))?   {addressloc+=addressleng;return ADDRESSCA;}
({POBOX}[ ])?{STREETNO}[ ].{2,60}[ ,.]+{UKZIP}([ ]?({UK}))?   {addressloc+=addressleng;return ADDRESSUK;}
.           {addressloc += addressleng; return UNKNOWN; }
%%


#include "REExtractor.h"

bool REExtractor::run( Vector<IToken*> & tokens, Vector<Label * > & newLabels )
{
   bool ret = true;
   newLabels.clear();
   positions.clear();
   labels = &newLabels;
   String sourceText;
   int count = tokens.count();
   for ( int i=0; i<count; i++ )
   {
     GeoToken * token = (GeoToken*)(tokens[i]);
     positions.add( sourceText.length() );
     //assert( sourceText.length() == positions[positions.count()-1] );
     //printf("sourceText length: %d == %d\n", sourceText.length(), positions[positions.count()-1] );
     sourceText += token->getTokenString() + " ";
   }
   scanSource( sourceText.c_str() );
//printf("%s\n", sourceText.c_str() );
   return ret;
}

void REExtractor::scanSource( const char * sourceText )
{
/* Note: following statement nolonger true for flex 2.5.33 append a '\0' to the end of string to make sure it is end with two '\0' for flex to scan */

  YY_BUFFER_STATE buf_state = address_scan_string( sourceText );
  flexMatch();
  address_delete_buffer(buf_state);

}


void REExtractor::flexMatch()
{
   int tok=0;
   addressloc=0;
   while ( (tok = addresslex()) ) 
   {
//   printf("tok: %d-%s addressloc %d, addressleng %d\n", tok, addresstext,addressloc, addressleng );

     switch (tok) 
     {
      case ADDRESSUS:
         //printf("US: %s.\n", addresstext );
	 saveAddress( addressloc - addressleng, addressloc - 1 );
         break;
      case ADDRESSCA:
       //  printf("CA: %s addressloc %d - len : %d %d.\n", addresstext, addressloc, addressleng, strlen(addresstext) );
	// saveAddress( addressloc - addressleng, addressloc - 1 );
	 break;
       case UNKNOWN:
         //printf("UNKNOWN %s\n",addresstext);
         break;
       default:
	 //printf("Default %s\n", addresstext);
	 break;
     }
   }
}
void REExtractor::saveAddress( int start, int end )
{
  int count = positions.count();
  int startIndex=0, endIndex=0;
  bool startFound = false;
  for ( int i=0; i<count; i++ )
  {
//    printf("i: %d position[i]: %d - start: %d, end %d\n",i,  positions[i], start, end );
    if ( positions[i] == start )
    {
      startIndex = i;
      startFound = true;
    }
    if ( !startFound && start < positions[i] )
    {
       startIndex = i-1;
       startFound = true;
    }
    if ( end == positions[i] )
    {
      endIndex = i;
      break;
    }
    if ( end < positions[i] )
    {
      endIndex = i-1;
      break;
    }
  }
  //printf("saveAddress start index %d - end %d\n", startIndex, endIndex );
  if ( startIndex >= 0 && endIndex > 0 )
  {
    labels->add( new Label( startIndex, endIndex ) );
  }
}
