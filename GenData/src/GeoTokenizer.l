/**
 * Lex file for tokenize web pages, and extract features
 *
 * {CAZIP}									{webloc += webleng; return CAZIP;}
 */
%option noyywrap
%option never-interactive
%option prefix="web" 
%option outfile="GeoTokenizer.cpp"
%{


/* features */
#define WORD_FORMAT_FEATURE_START 200
#define GEO_FEATURE_START 300

#define UNKNOWN 200
#define EMAIL 201
#define INITCAP 202
#define ALLCAPS 203
#define CONTAINDIGITS 204
#define ALLDIGITS 205
#define ACRONYM 206
#define WORD 207
#define PUNCTUATION 208
#define TIME 209
#define URL 210
#define CONTRACTION 211
#define ACRONYM2 ACRONYM    // Jerry. Unnecessary to distinguise it with ACRONYM, let both of them use same id.
#define LONELYINITIAL 212  /* Initial such as A. */
#define UPPERCASECHAR 213  /* One uppercase char */
#define LOWERCASECHAR 214  /* One lowercase char */
#define CONTAINDASH   215  /* Contains at least one dash */
#define PHONE 216
#define USZIP 217
#define POBOX 218


/*GEO FEATURES */
//#define CAZIP 302

/* html tags */
#define B_DOC 101
#define E_DOC 102
#define B_DOCHDR 103
#define E_DOCHDR 104
#define F_DOCNO 105
#define B_DOCOLDNO 106
#define E_DOCOLDNO 107
#define B_SCRIPT 108
#define E_SCRIPT 109
#define B_COMMENT 110
#define E_COMMENT 111
#define B_FORM 112
#define E_FORM 113
#define B_SELECT 114
#define E_SELECT 115


#define B_STYLE 119
#define E_STYLE 120
#define B_TITLE 121
#define E_TITLE 122
#define B_DATE 123
#define E_DATE 124
#define B_HEAD 125
#define E_HEAD 126





#define B_LINE 127
#define E_LINE 128

#define B_AHREF 129
#define E_AHREF 130
#define OTHERTAG 131


#define TOTAL_TAGS 40
#define CLOSE		1000

/* define address tags */
#define GEO 1
#define SN 2
#define NS 3
#define UN 4
#define UNIT 5
#define ST 6
#define STREET 7
#define SD 8
#define FL 9
#define CITY 10
#define STATE 11
#define COUNTRY 12
#define POB 13
#define PON 14
#define POS 15
#define POSN 16
#define ZIP 17
#define NAME 18
#define C 19
#define BUILDING 20
#define BN 21
#define OTHER 22

/* tags */
#define DOC 23
#define DOCNO 24
#define DOCHDR 25
#define DOCOLDNO 26
#define SCRIPT 27
#define TAG 29
#define SYMBOL 30
#define STYLE 31
#define HEAD 32
#define LINK 33
#define SPACE 34
#define CR 35
#define COMMENT 36

extern FILE * webin;
extern char * webtext;
long webloc;


%}

BR	(B|b)(R|r)
DIV	(D|d)(I|i)(V|v)
H1	(H|h)(1|1)
H2	(H|h)(2|2)
H3	(H|h)(3|3)
H4	(H|h)(4|4)
H5	(H|h)(5|5)
H6	(H|h)(6|6)
HR	(H|h)(R|r)
TD	(T|t)(D|d)
LI  (L|l)(I|i)
HEAD	(H|h)(E|e)(A|a)(D|d)
DIGIT   [0-9]
PHONESEPARATOR ([(]|[)]|[ ]|[\.]|[-])
USZIP [0-9]{5}([- ][0-9]{4})?
CAZIP [A-Za-z][0-9][A-Za-z][ ]?[0-9][A-Za-z][0-9]
GEO     (G|g)(E|e)(O|o)
C	(C|c)
SN      (S|s)(N|n)
NS      (N|n)(S|s)
UN      (U|u)(N|n)
UNIT    (U|u)(N|n)(I|i)(T|t)
ST      (S|s)(T|t)
STREET  (S|s)(T|t)(R|r)(E|e)(E|e)(T|t)
SD      (S|s)(D|d)
FL      (F|f)(L|l)
CITY    (C|c)(I|i)(T|t)(Y|y)
STATE   (S|s)(T|t)(A|a)(T|t)(E|e)
COUNTRY (C|c)(O|o)(U|u)(N|n)(T|t)(R|r)(Y|y)
POB     (P|p)(O|o)(B|b)
PON     (P|p)(O|o)(N|n)
POS     (P|p)(O|o)(S|s)
POSN    (P|p)(O|o)(S|s)(N|n)
ZIP     (Z|z)(I|i)(P|p)
NAME    (N|n)(A|a)(M|m)(E|e)
BUILDING (B|b)(U|u)(I|i)(L|l)(D|d)(I|i)(N|n)(G|g)
BN      (B|b)(N|n)
OTHER   (O|o)(T|t)(H|h)(E|e)(R|r)
SCRIPT  (S|s)(C|c)(R|r)(I|i)(P|p)(T|t)
LINK    (L|l)(I|i)(N|n)(K|k)
HREF    (H|h)(R|r)(E|e)(F|f)
FORM    (F|f)(O|o)(R|r)(M|m)
SELECT  (S|s)(E|e)(L|l)(E|e)(C|c)(T|t)
TOPDNSSUFFIX         (ac|ad|ae|af|ag|ai|al|am|an|ao|aq|ar|as|at|au|aw|az|ax|ba|bb|bd|be|bf|bg|bh|bi|bj|bm|bn|bo|br|bs|bt|bv|bw|by|bz|ca|cc|cd|cf|cg|ch|ci|ck|cl|cm|cn|co|cr|cs|cu|cv|cx|cy|cz|de|dj|dk|dm|do|dz|ec|ee|eg|eh|er|es|et|eu|fi|fj|fk|fm|fo|fr|ga|gb|gd|ge|gf|gg|gh|gi|gl|gm|gn|gp|gq|gr|gs|gt|gu|gw|gy|hk|hm|hn|hr|ht|hu|id|ie|il|im|in|io|iq|ir|is|it|je|jm|jo|jp|ke|kg|kh|ki|km|kn|kp|kr|kw|ky|kz|la|lb|lc|li|lk|lr|ls|lt|lu|lv|ly|ma|mc|md|mg|mh|mk|ml|mm|mn|mo|mp|mq|mr|ms|mt|mu|mv|mw|mx|my|mz|na|nc|ne|nf|ng|ni|nl|no|np|nr|nu|nz|om|pa|pe|pf|pg|ph|pk|pl|pm|pn|pr|ps|pt|pw|py|qa|re|ro|ru|rw|sa|sb|sc|sd|se|sg|sh|si|sj|sk|sl|sm|sn|so|sr|st|sv|sy|sz|tc|td|tf|tg|th|tj|tk|tl|tm|tn|to|tp|tr|tt|tv|tw|tz|ua|ug|uk|um|us|uy|uz|va|vc|ve|vg|vi|vn|vu|wf|ws|ye|yt|yu|za|zm|zw)
GENERICTOPDOMAIN  (aero|biz|cat|com|coop|info|jobs|mobi|museum|name|net|org|pro|travel|gov|edu|mil|int)

DNS      (("http://"|"https://"|"ftp://")?[^"'\r\n<>& @\[\]\=\/\(\)\:\;\?\*]*[.]({GENERICTOPDOMAIN}|{TOPDNSSUFFIX}))
POBOX (([P|p][.]?[ ]?[O|o][.]?[ ]?)([B|b][O|o][X|x])?)|("Post Office Box")|([B|b][O|o][X|x])

%%

("\r\n"|"\n\r"|[\n]|[\r])*                                    {webloc += webleng; return CR; }
"<DOC>"                                 {webloc += webleng; return B_DOC; }
"</DOC>"                                {webloc += webleng; return E_DOC; }
"<DOCHDR>"                              {webloc += webleng; return B_DOCHDR; }
"</DOCHDR>"                             {webloc += webleng; return E_DOCHDR; }
"<DOCNO>"[^<]*"</DOCNO>"                {webloc += webleng; return F_DOCNO; }
"<DOCOLDNO>"                            {webloc += webleng; return B_DOCOLDNO; }
"</DOCOLDNO>"                           {webloc += webleng; return E_DOCOLDNO; }
"<"[sS][tT][yY][lL][eE][^>]*">"         {webloc += webleng; return B_STYLE; }
"</"[sS][tT][yY][lL][eE]">"             {webloc += webleng; return E_STYLE; } 
"<"{SCRIPT}[^>]*">"     {webloc += webleng; return B_SCRIPT; }
"</"{SCRIPT}[ ]*">"         {webloc += webleng; return E_SCRIPT; } 
"<"[tT][iI][tT][lL][eE][^>]*">"         {webloc += webleng; return B_TITLE; }
"</"[tT][iI][tT][lL][eE]">"             {webloc += webleng; return E_TITLE; }
"<"[dD][aA][tT][eE][^>]*">"             {webloc += webleng; return B_DATE; }
"</"[dD][aA][tT][eE]">"                 {webloc += webleng; return E_DATE; }
"<!--"[^>]*"-->"                        {webloc += webleng; return COMMENT; } /* comment will not be processed */
"<!--"                                  {webloc += webleng; return B_COMMENT; }
"<!"" "*"["([a-zA-Z])*"]"[ -]*"-->"                                   {webloc += webleng; return E_COMMENT; }
"<"*[a-zA-Z0-9]*[- ]*"-->"                                   {webloc += webleng; return E_COMMENT; }
\<[ ]*[aA][^>]*{HREF}[^>]*\>            {webloc += webleng; return B_AHREF; }
\<\/[ ]*[aA][ ]*\>                    {webloc += webleng; return E_AHREF; }

\<\/{FORM}([^>'"]|\"[^"]*\"|'[^']*')*\>					{webloc +=webleng; return E_FORM;}
\<{FORM}([^>'"]|\"[^"]*\"|'[^']*')*\>					{webloc +=webleng; return B_FORM;}
\<\/{SELECT}([^>'"]|\"[^"]*\"|'[^']*')*\>					{webloc +=webleng; return E_SELECT;}
\<{SELECT}([^>'"]|\"[^"]*\"|'[^']*')*\>					{webloc +=webleng; return B_SELECT;}



\<\/{HEAD}([^>'"]|\"[^"]*\"|'[^']*')*\>					{webloc +=webleng; return E_HEAD;}
\<{HEAD}([^>'"]|\"[^"]*\"|'[^']*')*\>					{webloc +=webleng; return B_HEAD;}

\<{LINK}[^>]*\>                         {webloc += webleng; return LINK; }


	
	/* ADDRESS Tags */
\<\/{GEO}\>	{webloc += webleng; return CLOSE+GEO; }
\<{GEO}[^>]*(([U|u][S|s])|([U|u][K|k])|([C|c][A|a]))[^>]*\>	{webloc += webleng; return GEO; }


                /*html tags, format feature */
\<\/{BR}[^>]*\>	{webloc += webleng; return E_LINE;}
\<{BR}[^>]*\>		{webloc += webleng; return B_LINE;}
\<\/{DIV}[^>]*\>	{webloc += webleng; return E_LINE;}
\<{DIV}[^>]*\>	{webloc += webleng; return B_LINE;}
\<\/{TD}[^>]*\>	{webloc += webleng; return E_LINE;} 
\<{TD}[^>]*\>		{webloc += webleng; return B_LINE;}
\<\/{H6}[^>]*\>	{webloc += webleng; return E_LINE;}
\<{H6}[^>]*\>		{webloc += webleng; return B_LINE;}
	
\<\/{H5}[^>]*\>	{webloc += webleng; return E_LINE;}
\<{H5}[^>]*\>		{webloc += webleng; return B_LINE;}
	
\<\/{H4}[^>]*\>	{webloc += webleng; return E_LINE;}
\<{H4}[^>]*\>		{webloc += webleng; return B_LINE;}
	
\<\/{H3}[^>]*\>	{webloc += webleng; return E_LINE;}
\<{H3}[^>]*\>		{webloc += webleng; return B_LINE;}
	
\<\/{H2}[^>]*\>	{webloc += webleng; return E_LINE;}
\<{H2}[^>]*\>		{webloc += webleng; return B_LINE;}
	
\<\/{H1}[^>]*\>	{webloc += webleng; return E_LINE;}
\<{H1}[^>]*\>		{webloc += webleng; return B_LINE;}

\<\/{LI}[^>]*\>	{webloc += webleng; return E_LINE;}
\<{LI}[^>]*\>		{webloc += webleng; return B_LINE;}


[0-9]+[0-9 .:]*(([aA][ ]*[.]?[ ]*[mM][ ]*[.]?)|([pP][ ]*[.]?[ ]*[mM][ ]*[.]?))               {webloc += webleng; return TIME; }
([+][ ]*)?[(]?({DIGIT}{1,5}{PHONESEPARATOR}+)*{DIGIT}{3,4}{PHONESEPARATOR}+{DIGIT}{3,4}({PHONESEPARATOR}+{DIGIT}{1,6})?		{webloc += webleng; return PHONE;}
{USZIP}									{webloc += webleng; return USZIP;}
{DNS}[/][^"<>\)'\(\\\[\]\r ]* {webloc += webleng; return URL;}
{DNS}/[^a-zA-Z0-9]                        {webloc += webleng; return URL; }
[A-Za-z0-9][_\.\-a-zA-Z0-9]*(@|&#64;)[A-Za-z0-9]+([_\.\-a-zA-Z0-9]+)*		{webloc += webleng; return EMAIL;}
{POBOX}                                 {webloc += webleng; return POBOX; } /* P.O.Box */
[A-Z]                                   {webloc += webleng; return UPPERCASECHAR; } /* One uppercase char */
[a-z]                                   {webloc += webleng; return LOWERCASECHAR; } /* One lowercase char */
[A-Z]\.                                 {webloc += webleng; return LONELYINITIAL; } /* Initial such as A. */
[A-Z][a-z]*					            {webloc += webleng; return INITCAP;}
[A-Z][A-Z]+                             {webloc += webleng; return ALLCAPS; }
([0-9]+[[A-Za-z]+[A-Za-z0-9]*)|([A-Za-z]+[0-9]+[A-Za-z]*)				{webloc += webleng; return CONTAINDIGITS; }
{DIGIT}+								{webloc += webleng; return ALLDIGITS; }



[a-zA-Z0-9]+                            {webloc += webleng; return WORD; }
[a-zA-Z0-9]+[ ]*[-][ ]*[a-zA-Z0-9]+      {webloc += webleng; return WORD; /* such as e-mail */}
[A-Z][A-Z]+((\')?[s])?                  {webloc += webleng; return ACRONYM2; }
[a-zA-Z0-9]+['][a-zA-Z]+                 {webloc += webleng; return CONTRACTION;}
[a-zA-Z0-9]+"\222"[a-zA-Z]+                 {webloc += webleng; return CONTRACTION;/*/222 is ' */} 
[A-Z]\.([A-Z]\.)+                       {webloc += webleng; return ACRONYM; }
[-]*[a-zA-Z0-9]*[-]+[a-zA-Z0-9]*[-]*    {webloc += webleng; return CONTAINDASH; } /* Contains at least one dash */


"<"[ ]*[/]?[ ]*[a-zA-Z][ ]*">"          {webloc += webleng; return OTHERTAG; }
"<"[ ]*[/]?[ ]*[a-zA-Z][^<>]*[^-<>]+">" {webloc += webleng; return OTHERTAG;/* zap tags */ }
"<!"[^-][^>]*">"                        {webloc += webleng; return UNKNOWN;/* zap other tags*/}
[&nbsp;]				{webloc += webleng; return UNKNOWN; /* space */ }
[&][a-zA-Z]+[;]                         {webloc += webleng; return UNKNOWN;/* zap symbols */ }
[&][#][0-9]*[;]                         {webloc += webleng; return UNKNOWN;/* zap symbols */ } 
[,]|[.]|[!]|[?]|[;]|[:]|[+]|[-]|[']|[(]|[)]|["]|"["|"]"|[{]|[}]|[=]|[_]|"\/"|"\\"|[#]|[<]|[>]		{webloc += webleng; return PUNCTUATION;}


" "*                                    {webloc += webleng; return SPACE; }
.                                       {webloc += webleng; return UNKNOWN; }
%%


#include "GeoTokenizer.h"

GeoTokenizer::GeoTokenizer() : lineStatus(0), stopWord(false), geo(false),
  script(false), style(false), docHeader(false), head(false), comment(false),
  geoStart(false), geoEnd(false), geoBoundaryFlag(GeoTokenizer::GEO_OTHER)
{
}

GeoTokenizer::~GeoTokenizer()
{
}

bool GeoTokenizer::parse( const char * fileName, vector<IToken*> * tokens )
{
  assert ( fileName && tokens );
  yyin = fopen( fileName, "r" );
  if (!yyin)
  {
      yyin = stdin;
  }

  /* set_string(source); */
  /* parse the file */
  flexMatch( tokens );
  if ( yyin ) 
  {
    fclose(yyin);
  }

  return true;
}


bool GeoTokenizer::flexMatch( vector<IToken*> * tokens )
{
    int tok=0;
    
    while ( ( tok = weblex() ) ) 
    {
      int geoFeatureFlag = -1; /* flag for geo features, such as Telephone, zip, US States, -1 means not a geo feature */

#if 0
if ( !isScript() && !isStyle() && !isDocHeader() && !isHead() && !isComment() ) 
{
printf(">>>current state isnot stopword, token %s - %d.\n", webtext, tok );
}
else
{
printf(">>>current state is stopword %d %d %d %d %d, token %s.\n", isScript(), isStyle(), isDocHeader(), isHead(), isComment(), webtext);
}
#endif

      switch (tok) 
      {


	case B_LINE:
          {
                /* start of a new text line */
                this->setLineStart( );

                //printf("B_LINE -> %s, %d.\n", webtext, tok );
          }

	  break;
	case E_LINE:
                //printf("E_LINE -> %s, %d.\n", webtext, tok );
	      break;

	case GEO:
                /* <GEO> tag started */
                this->setGeoStart();
                // printf("GeoStart is set. %s\n", this->isGeoStart() ? "TRUE":"FALSE");
		this->setGeo( true );                
//                printf("GEO -> %s, %d.\n", webtext, tok );
		break;		
	case GEO+CLOSE:
                /* <GEO> tag ended with </GEO> */
                this->setGeoEnd();
                this->setGeo( false );
                if ( tokens->count() > 0 )
                {
                  GeoToken * prevToken = (GeoToken*)( tokens->get( tokens->count() -1 ) );
                  prevToken->setGeoEnd();
               // printf("set token %s to GEOEND\n", prevToken->getTokenString().c_str() );
                }

               // printf("/GEO -> %s, %d.\n", webtext, tok );
		break;
        case EMAIL:
        case INITCAP:
        case ALLCAPS:
        case CONTAINDIGITS:
        case ALLDIGITS:
        case ACRONYM:
        case WORD:
        case PUNCTUATION:
        case TIME:
        case URL:
        case CONTRACTION:
        case LONELYINITIAL:
        case UPPERCASECHAR:
        case LOWERCASECHAR:
        case CONTAINDASH:
        case PHONE:
        case USZIP:
        case POBOX:
        {
//if (tok==TIME)
//printf("TIME %s ->%d.\n", webtext, tok );
//if (tok==URL)
//printf("URL %s -> %d.\n", webtext, tok );
            /* append the token to token list */

            //if ( !isScript() && !isStyle() && !isDocHeader() && !isHead() && !isComment() )
            //if ( !isScript() && !isStyle() && !isDocHeader() && !isComment() )
            if ( !isScript() && !isStyle() && !isDocHeader() )
            {
              GeoToken * token = new GeoToken( webtext, tok - WORD_FORMAT_FEATURE_START, lineStatus );
              
  //            printf("%s -> %s\n", token->getTokenString().c_str(), this->isGeoStart()?"START":this->isGeoMid()?"MID":this->isGeoEnd()?"END":this->isGeoOther()?"OTHER":"UNKNOWN");

              if ( this->isLineStart() )
              {
                token->setLineStart();
                int count = tokens->count();
                if ( count > 0 )
                {
                  GeoToken * prevToken = (GeoToken*)( tokens->get( count -1 ) );
                  if ( prevToken->isLineStart() )
                   {
                     prevToken->setLineJustOneToken();
                     //printf(" Set prev Token %s to LineJustOneToken.\n", prevToken->getTokenString().c_str() );
                   }
                   else
                   {
                     prevToken->setLineEnd();     /* change previous token state to "end of line" */
                     //printf(" Set prev Token %s to LineEnd.\n", prevToken->getTokenString().c_str() );
                   }
                }

                this->setLineMid(); /* change state to "middle of line" */
              }
              else if ( this->isLineMid() )
              {
                token->setLineMid();
              }
	      /* set isGeo, notGeo feature */
              if ( this->isGeo() )           /* in the <GEO></GEO> block */
              {
                token->setGeo( 1 );
              }
              /* set Geo Start, Mid, End feature, it has some overlap with isGeo, notGeo feature, but we want to keep them seperate */

              if ( this->isGeoOther() )
              {
                token->setGeoOther();
               // printf("set token %s to GEOOTHER => %s\n", token->getTokenString().c_str(), token->isGeoOther()?"TRUE":"FALSE" );
              } 
              else if ( this->isGeoMid() )
              {
                 token->setGeoMid();
              }
              else if ( this->isGeoStart() )
              {
		token->setGeoStart();
                //printf("set token %s to GEOSTART %s\n", token->getTokenString().c_str(), token->isGeoStart()?"TRUE":"FALSE" );
		this->setGeoMid();
                /* if proceeding text block is another address, we need set it to end */
                if ( tokens->count() > 0 )
                {
                  GeoToken * prevToken = (GeoToken*)( tokens->get( tokens->count() -1 ) );
                  if ( prevToken->isGeoMid() )
                  {
                     prevToken->setGeoEnd();
                  }
                }

	      } 
              else if ( this->isGeoEnd() )
	      {
		this->setGeoOther();
	      }
              /*else
              {
                token->setGeoOther();
              }*/
			  
		
              /*if ( tok = PUNCTUATION )
              {
                string tokenString = token->getTokenString();
 
                if ( tokenString== '.' || tokenString == '!' || tokenString == '?' )
                {
                  token->setLineEnd();
                }
              }*/

              tokens->add( token );
              //printf("%s -> %s.\n", webtext, this->isLineStart()?"LineStart":this->isLineMid()?"LineMid":"LineEnd" );
            }
            else
            {
              /* stop word, discarded */
              this->setGeoOther();
              //printf("Discarded %s -> %d.\n", webtext, tok );
            }
            //this->resetLineStatus();  /* reset line status to be line_start_pending ( not line_start and line_end )*/

    //        printf("%s -> %d.\n", webtext, tok );
        }
            break;

        /* blocks that should be discarded by the tokenizer */        

	case B_SCRIPT:
            setScript( true );
            //printf("script start: %s.\n", webtext );
            break;
	case B_STYLE:
            setStyle( true );
            //printf("style start: %s.\n", webtext );
            break;

	case B_DOCHDR:
            setDocHeader( true );
            //printf("docheader start: %s.\n", webtext );
            break;
	case B_HEAD:
            setHead( true );
            //printf("head start: %s.\n", webtext );
            break;
        case B_COMMENT:
            setComment( true );
            //printf("comment start: %s.\n", webtext );
            break;

           // this->setStopWord( true ); /* start of the block that should be discarded by the tokenizer */
           // printf("%s -> start stopword %d.\n", webtext, tok );

	case E_SCRIPT:
            setScript( false );
            //printf("script end: %s.\n", webtext );
            break;
	case E_STYLE:
            setStyle( false );
            //printf("style end: %s.\n", webtext );
            break;
	case E_DOCHDR:
            setDocHeader( false );
            //printf("docheader end: %s.\n", webtext );
            break;
	case E_HEAD:
            setHead( false );
            //printf("head end: %s.\n", webtext );
            break;
        case E_COMMENT:
            setComment( false );
            //printf("comment end: %s.\n", webtext );
            break;
            //this->setStopWord( false ); /* end of the block that should be discarded by the tokenizer */
            //printf("%s -> end stopword %d.\n", webtext, tok );
	    //break;

        case B_TITLE:
             break;
        case E_TITLE:
            //printf("title %s -> %d.\n", webtext, tok );
            break;
        case LINK:
            //printf("LINK %s -> %d.\n", webtext, tok );
            break;
        case SPACE:
            //printf("SPACE -> %d.\n", tok );
            break;
        case B_AHREF:
            //printf("B_AHREF %s -> %d.\n", webtext, tok );
            break;
        case E_AHREF:
            //printf("E_AHREF %s -> %d.\n", webtext, tok );
            break;
        case CR:
            //printf("%s ->%d.\n", webtext, tok );
            break;
        case OTHERTAG:
            // printf("other tag: %s.\n", webtext );
            break;

    default:
            //printf("default -> %s, %d.\n",webtext, tok );
	    break;
//#endif
    }
 

  }
  /* layout format and GEO(START/MID/END/OTHER) feture setting for last token */
  int count = tokens->count();
  if ( count > 0 )
  {
      GeoToken * token = (GeoToken*)( tokens->get( count -1 ) );
      if ( token->isLineStart() )
      {
        token->setLineJustOneToken();
      }
      else if ( token->isLineMid() )
      {
        token->setLineEnd();     /* change last token state to "end of line" */
      }
#if 0
      if ( token->isGeoMid() ) /* may not needed */
      {
        token->setGeoEnd();
      }
#endif
  }
  return true;
}
