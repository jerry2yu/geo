%option noyywrap
%option never-interactive
%option prefix="web" 
%option outfile="text2token.c"
%{

/**
 * File: text2token.l
 * --------------------
 * scan html file and get a cvector of tokens with features
 * Update: Feb 2, 2005 by Zheyuan Yu
 */
#define YYLMAX 8192*100 
#include "text2token.h"

/* features */
#define FEATURE_START    200

#define PHONE            200
#define USZIP            201
#define CAZIP            202
#define EMAIL            203
#define INITCAP          204
#define ALLCAPS          205
#define CONTAINDIGITS    206
#define ALLDIGITS        207
#define ACRONYM          208
#define WORD             209
#define PUNCTUATION      210
#define DATETIME         211

/* html tags */
#define B_DOC            101
#define E_DOC            102
#define B_DOCHDR         103
#define E_DOCHDR         104
#define F_DOCNO          105
#define B_DOCOLDNO       106
#define E_DOCOLDNO       107
#define B_SCRIPT         108
#define E_SCRIPT         109
#define COMMENT          110


#define CONTRACTION      112

#define ACRONYM2         113
#define UNKNOWN          114
#define B_STYLE          115
#define E_STYLE          116
#define B_TITLE          117
#define E_TITLE          118
#define B_DATE           119
#define E_DATE           120
#define F_HEAD           121



#define B_LINE           125
#define E_LINE           126

#define TOTAL_TAGS       40
#define CLOSE            1000

/* define address tags */

#define GEO              1
#define SN               2
#define NS               3
#define UN               4
#define UNIT             5
#define ST               6
#define STREET           7
#define SD               8
#define FL               9
#define CITY             10
#define STATE            11
#define COUNTRY          12
#define POB              13
#define PON              14
#define POS              15
#define POSN             16
#define ZIP              17
#define NAME             18
#define C                19
#define BUILDING         20
#define BN               21
#define OTHER            22

/* tags */

#define DOC              23
#define DOCNO            24
#define DOCHDR           25
#define DOCOLDNO         26
#define SCRIPT           27
#define TAG              28
#define SYMBOL           29
#define STYLE            30
#define HEAD             31




int NGRAM=3; //default 3ngram

//  load dictionary for US States, Street Suffixes, Street Directions ...
SimpleExtract se;

long webloc;


%}
BR   (B|b)(R|r)
DIV   (D|d)(I|i)(V|v)
H1   (H|h)(1|1)
H2   (H|h)(2|2)
H3   (H|h)(3|3)
H4   (H|h)(4|4)
H5   (H|h)(5|5)
H6   (H|h)(6|6)
HR   (H|h)(R|r)
TD   (T|t)(D|d)
LI  (L|l)(I|i)
HEAD   (H|h)(E|e)(A|a)(D|d)
STYLE [sS][tT][yY][lL][eE]
SCRIPT [sS][cC][rR][iI][pP][tT]
TITLE [tT][iI][tT][lL][eE]
DATE [dD][aA][tT][eE]
DIGIT   [0-9]
PHONESEPARATOR ([(]|[)]|[ ]|[.]|[-]|[/])
USZIP [0-9]{5}([- ][0-9]{4})?
CAZIP [A-Za-z][0-9][A-Za-z][ ]?[0-9][A-Za-z][0-9]
GEO     (G|g)(E|e)(O|o)
C   (C|c)
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
MONTH   (Jan(uary)?|Feb(ruary)?|Ma(r(ch)?|y)|Apr(il)?|Ju((ly?)|(ne?))|Aug(ust)?|Oct(ober)?|(Sept|Nov|Dec)(ember)?)
TIME   (([0-9]{1,2}[ ]*[:][ ]*[0-9]{1,2}([ ]*[:][ ]*[0-9]{1,2})?[ ]*(AM|PM|am|pm|Am|Pm)?))
TEL (([T|t][E|e][L|l])?|([F|f][A|a][X|x])?|([P|p][H|h][O|o][N|n][E|e])?|([T|t][E|e][L|l][E|e][P|p][H|h][O|o][N|n][E|e])?)

%%
"<DOC>"                                                       { webloc += webleng; return B_DOC; }
"</DOC>"                                                      { webloc += webleng; return E_DOC; }
"<DOCHDR>"                                                    { webloc += webleng; return B_DOCHDR; }
"</DOCHDR>"                                                   { webloc += webleng; return E_DOCHDR; }
"<DOCNO>"(.|\n)*"</DOCNO>"                                    { webloc += webleng; return F_DOCNO; }
"<DOCOLDNO>"                                                  { webloc += webleng; return B_DOCOLDNO; }
"</DOCOLDNO>"                                                 { webloc += webleng; return E_DOCOLDNO; }

"<"[ ]*{STYLE}[^<>]*\>                                        { webloc += webleng; return B_STYLE; }
"</"[ ]*{STYLE}[^<>]*\>                                       { webloc += webleng; return E_STYLE; } 
"<"{SCRIPT}[^<>]*\>                                           { webloc += webleng; return B_SCRIPT; } 
"</"{SCRIPT}[^<>]*\>                                          { webloc += webleng; return E_SCRIPT; }
"<"[ ]*{TITLE}[^<>]*\>                                        { webloc += webleng; return B_TITLE; }
"</"[ ]*{TITLE}[^<>]*\>                                       { webloc += webleng; return E_TITLE; }
"<"[ ]*{DATE}[^<>]*\>                                         { webloc += webleng; return B_DATE; }
"</"[ ]*{DATE}[^<>]*\>                                        { webloc += webleng; return E_DATE; }
"<!"[^<>]*\>                                                  { webloc += webleng; return COMMENT; }
"<"[ ]*{HEAD}[^<>]*\>(.|\n)*"</"[ ]*{HEAD}[^<>]*\>            { webloc +=webleng; return F_HEAD;}
   
   /* ADDRESS Tags */
\<\/{GEO}\>   {webloc += webleng; return CLOSE+GEO; }
\<{GEO}[^>]*(([U|u][S|s])|([U|u][K|k])|([C|c][A|a]))[^>]*\>   { webloc += webleng; return GEO; }
\<\/{SN}\>                                                    { webloc += webleng; return CLOSE+SN; }
\<{SN}\>                                                      { webloc += webleng; return SN; }
\<\/{NS}\>                                                    { webloc += webleng; return CLOSE+NS; }
\<{NS}\>                                                      { webloc += webleng; return NS; }
\<\/{UN}\>                                                    { webloc += webleng; return CLOSE+UN; }
\<{UN}\>                                                      { webloc += webleng; return UN; }
\<\/{UNIT}\>                                                  { webloc += webleng; return CLOSE+UNIT;}
\<{UNIT}\>                                                    { webloc += webleng; return UNIT;} 
\<\/{ST}\>                                                    { webloc += webleng; return CLOSE+ST; }
\<{ST}\>                                                      { webloc += webleng; return ST; }
\<\/{STREET}\>                                                { webloc += webleng; return CLOSE+STREET; }
\<{STREET}\>                                                  { webloc += webleng; return STREET; }
\<\/{SD}\>                                                    { webloc += webleng; return CLOSE+SD; }
\<{SD}\>                                                      { webloc += webleng; return SD; }
\<\/{FL}\>                                                    { webloc += webleng; return CLOSE+FL; }
\<{FL}\>                                                      { webloc += webleng; return FL; }
\<\/{CITY}\>                                                  { webloc += webleng; return CLOSE+CITY; }
\<{CITY}\>                                                    { webloc += webleng; return CITY; }
\<\/{STATE}\>                                                 { webloc += webleng; return CLOSE+STATE; }
\<{STATE}\>                                                   { webloc += webleng; return STATE; }
\<\/{COUNTRY}\>                                               { webloc += webleng; return CLOSE+COUNTRY; }
\<{COUNTRY}\>                                                 { webloc += webleng; return COUNTRY; }
\<\/{POB}\>                                                   { webloc += webleng; return CLOSE+POB; }
\<{POB}\>                                                     { webloc += webleng; return POB; }
\<\/{PON}\>                                                   { webloc += webleng; return CLOSE+PON; }
\<{PON}\>                                                     { webloc += webleng; return PON; }
\<\/{POS}\>                                                   { webloc += webleng; return CLOSE+POS; }
\<{POS}\>                                                     { webloc += webleng; return POS; }
\<\/{POSN}\>                                                  { webloc += webleng; return CLOSE+POSN; }
\<{POSN}\>                                                    { webloc += webleng; return POSN; }
\<\/{ZIP}\>                                                   { webloc += webleng; return CLOSE+ZIP; }
\<{ZIP}\>                                                     { webloc += webleng; return ZIP; }
\<\/{NAME}\>                                                  { webloc += webleng; return CLOSE+NAME; }
\<{NAME}\>                                                    { webloc += webleng; return NAME; }
\<\/{C}\>                                                     { webloc += webleng; return CLOSE+C; }
\<{C}\>                                                       { webloc += webleng; return C; }
\<\/{BUILDING}\>                                              { webloc += webleng; return CLOSE+BUILDING; }
\<{BUILDING}\>                                                { webloc += webleng; return BUILDING; }
\<\/{BN}\>                                                    { webloc += webleng; return CLOSE+BN; }
\<{BN}\>                                                      { webloc += webleng; return BN; }
\<\/{OTHER}\>                                                 { webloc += webleng; return CLOSE+OTHER; }
\<{OTHER}\>                                                   { webloc += webleng; return OTHER; }


   /*html tags*/
\<\/{BR}[^<>]*\>                                              { webloc += webleng; return E_LINE; }
\<{BR}[^<>]*\>                                                { webloc += webleng; return B_LINE; }
\<\/{DIV}[^<>]*\>                                             { webloc += webleng; return E_LINE; }
\<{DIV}[^<>]*\>                                               { webloc += webleng; return B_LINE; }
\<\/{TD}[^<>]*\>                                              { webloc += webleng; return E_LINE; } 
\<{TD}[^<>]*\>                                                { webloc += webleng; return B_LINE; }
\<\/{H6}[^<>]*\>                                              { webloc += webleng; return E_LINE; }
\<{H6}[^<>]*\>                                                { webloc += webleng; return B_LINE; }
   
\<\/{H5}[^<>]*\>                                              { webloc += webleng; return E_LINE; }
\<{H5}[^<>]*\>                                                { webloc += webleng; return B_LINE; }
   
\<\/{H4}[^<>]*\>                                              { webloc += webleng; return E_LINE; }
\<{H4}[^<>]*\>                                                { webloc += webleng; return B_LINE; }
   
\<\/{H3}[^<>]*\>                                              { webloc += webleng; return E_LINE; }
\<{H3}[^<>]*\>                                                { webloc += webleng; return B_LINE; }
   
\<\/{H2}[^<>]*\>                                              { webloc += webleng; return E_LINE; }
\<{H2}[^<>]*\>                                                { webloc += webleng; return B_LINE; }
   
\<\/{H1}[^<>]*\>                                              { webloc += webleng; return E_LINE; }
\<{H1}[^<>]*\>                                                { webloc += webleng; return B_LINE; }

\<\/{LI}[^<>]*\>                                              { webloc += webleng; return E_LINE; }
\<{LI}[^<>]*\>                                                { webloc += webleng; return B_LINE; }



([+][ ]*)?[(]?({DIGIT}{1,5}{PHONESEPARATOR}+)*{DIGIT}{2,4}{PHONESEPARATOR}+{DIGIT}{3,4}{PHONESEPARATOR}?{DIGIT}{0,4}({PHONESEPARATOR}+{DIGIT}{0,6})?{DIGIT}      {webloc += webleng; return PHONE;}
{USZIP}                                                       { webloc += webleng; return USZIP; }
{CAZIP}                                                       { webloc += webleng; return CAZIP; }
[A-Za-z0-9][_\.\-a-zA-Z0-9]*@[A-Za-z0-9]+([_\.\-a-zA-Z0-9]+)*      { webloc += webleng; return EMAIL; }
[A-Z][a-z]*                                                   { webloc += webleng; return INITCAP; }
[A-Z][A-Z]+                                                   { webloc += webleng; return ALLCAPS; }
([0-9]+[[A-Za-z]+[A-Za-z0-9]*)|([A-Za-z]+[0-9]+[A-Za-z]*)     { webloc += webleng; return CONTAINDIGITS; }
{DIGIT}+                                                      { webloc += webleng; return ALLDIGITS; }
(([0-9]{1,2}[ ]+{MONTH}[ ,]+[0-9]{4})|({MONTH}[ ]+[0-9]{1,2}[ ,]+[0-9]{4})|([0-9]{1,2}[/-][0-9]{1,2}[/-][0-9]{4}))?([ -]*{TIME})?      {webloc += webleng; return DATETIME; }



[a-zA-Z0-9]+                                                  { webloc += webleng; return WORD; }
[A-Z][A-Z]+((\')?[s])?                                        { webloc += webleng; return ACRONYM2; }
[a-zA-Z0-9]+\'[a-zA-Z]+                                       { webloc += webleng; return CONTRACTION; }
[A-Z]\.([A-Z]\.)+                                             { webloc += webleng; return ACRONYM; }
[\n]                                                          { webloc += webleng; /* zap newline */ }



"<"[/]?[a-zA-Z][^<>]*">"                                      { webloc += webleng; /* zap tags */ }
"<!"[^-][^>]*">"                                              { webloc += webleng; /* zap other tags*/ }
[&nbsp;]                                                      { webloc += webleng; return PUNCTUATION; }
[&][a-zA-Z]+[;]                                               { webloc += webleng; /* zap symbols */ }
[&][#][0-9]*[;]                                               { webloc += webleng; /* zap symbols */ } 
[,]|[.]|[!]|[?]|[;]|[:]|[+]|[-]|[']|[(]|[)]|["]               { webloc += webleng; return PUNCTUATION;}


.                                                             { webloc += webleng; return UNKNOWN; }
%%

//#define OUTER 0

int count_ng=0, count_term=0;

/*flag for tags
1: between the tag 0: out of the tag
*/
int tags[TOTAL_TAGS];
int line_flag;
/* name of features */
const char* name_local[20] = {"PHONE","USZIP","CAZIP","EMAIL","INITCAP","ALLCAPS","CONTAINDIGITS","ALLDIGITS","ACRONYM","WORD","PUNCTUATION", "DATETIME"};
const char* name_layout[20] = {"LINE_START","LINE_IN","LINE_END"};
const char* name_label[4] = {"out","begin","in","end"};
const char* name_punctuation[14]= {"COMMA","PERIOD","EXCLAMATION","QUESTION","SEMICOLON","COLON","PLUS","MINUS","APOSTROPHE","LEFT_PARENTHESIS","RIGHT_PARENTHESIS","DOUBLE_QUOTE","SPACE","NOTPUNCTUATION"};



#define TOTAL_GEO_TAGS 23
const char name_label_tag[TOTAL_GEO_TAGS][20] = {"NOTGEO","GEO","SN","NS","UN","UNIT","ST","STREET","SD","FL","CITY","STATE","COUNTRY","POB","PON","POS","POSN","ZIP","NAME","C","BUILDING","BN","OTHER"};

/**
 * vector to hold features
 * label: 0: NOTGEO 1: ISGEO 
 * last update: Dec 10, 2004
 */

cvector* tokens;

/**
 * insert feature (local, layout) to the vector 
 */
void insert_feature_to_vector(const char* term, Token *token )
{
   char startLetter; int punc;

   if (strlen(term)==1)  //single letter, we test whether its' a punctuation
   {
      startLetter=term[0];
      switch (startLetter) {
         case ',': 
            punc=0;
            break;
         case '.':
            punc=1;
            break;
         case '!':
            punc=2;
            break;
         case '?':
            punc=3;
            break;
         case ';':
            punc=4;
            break;
         case ':':
            punc=5;
            break;
         case '+':
            punc=6;
            break;
         case '-':
            punc=7;
            break;
         case '\'':
            punc=8;
            break;
         case '(':
            punc=9;
            break;
         case ')':
            punc=10;
            break;
         case '"':
            punc=11;
            break;
         case ' ':
            punc=12;
            break;
         default:
            punc=13;
            break;
         }
   }
   else
      punc=13; //not punctuation
      
   token->punctuation=punc;
    if (token->label_tag>0)
      token->category = 1;
    else
        token->category = token->label_tag;
   VectorAppend(tokens, token);

   
   //cout<<f->term<<": "<<f->punctuation<<name_punctuation[f->punctuation]<<endl;

}

/**
 * inserts token into feature vector
 * @param position: position of the token in the text
 * @param term: the token term
 * @param local: id of local format feature
 */
void insert_feature(int position, const char* term, int local)
{

   if (!tags[SCRIPT]&&!tags[STYLE]&&!tags[DOCHDR])
   {
      int label=0;//0: out of address 1:begin of address 2: in 3: end
      int label_tag=0;//geo tags: e.g. GEO, STREET 
      int layout=0;
      
      int is_usstate=0; //token a us state, default 0: not
      int is_sud=0; //token is Secondary Unit Designator, such as APT, FLOOR, default 0: not
      int is_ss=0; // token is Street Suffixes, such as AVE, Road, default 0: not
      int is_sd=0; // token is Street Direction, such as South, North. default 0: not
      int is_others=0; // token is other words, such as POB, university. default 0: not

      // get label_tag, e.g. STREET, CITY

      if ((tags[GEO])&&(!tags[C]))//if it's USA,UK,CA GEO tag, and not C (comment) tag, then get geo tag.
      for (int i=(GEO+1);i<=OTHER;i++)
      {
         if (tags[i])
         {
            label_tag=i;
            break;
         }
      }
      //cout<<"FIND "<<term<<" "<<name_label_tag[label_tag]<<endl;

      if ((tags[GEO]==1)&&!tags[C])
      {
         label=1;//address start
         tags[GEO]=2;
      }
      else if ((tags[GEO]==2)&&!tags[C])
      {
         label=2; // in the middle of an address
      }
      //if (tags[GEO]&&(!tags[C])) label=1;
      
      switch (line_flag)
      {
         case 0:  //LINE_START
            layout=0; 
            line_flag=1; //change it to LINE_IN
            break;
         case 1:  // LINE_IN
            layout=1;
            break;
         case -1:
            layout=0; //LINE_START
            line_flag=1; //change it to LINE_IN
            break;
      }   
      
      //  dictionary feature lookup
      if ( se.is_usstate (term) )
         is_usstate=1;
      if ( se.is_sud (term) )
         is_sud=1;
      if (se.is_ss (term) )
         is_ss=1;
      if (se.is_sd (term) )
         is_sud=1;
      if (se.is_others (term) )
         is_others=1;

      Token token;
      token.term= (char*)malloc(strlen(term)+1);
      strcpy(token.term,term);
      token.local=local;
      token.layout=layout;
      token.label=label;
      token.label_tag=label_tag;
   
      //features for dictionaries
      token.is_usstate=is_usstate;
      token.is_sud=is_sud;
      token.is_ss=is_ss;
      token.is_sd=is_sd;
      token.is_others=is_others;
      token.position = position;
      //printf("%s, %d, position: %d<br>\n", token.term, tags[SCRIPT], token.position);
      
      /* insert feature into vector */
      insert_feature_to_vector(term, &token);
   }

}

/**
 * match and extract features
 */
void flex_match()
{
   /* initialize flags */
   for (int i=0;i<TOTAL_TAGS;i++)
     tags[i]=0;
   line_flag=-1;
   
   int tok=0;

   while ((tok = weblex())) 
   {
      //printf("%s %d\n",webtext, tok);
    
      /* set tags */
      if ((tok>=GEO)&&(tok<=OTHER))
      {
         //cout<<"Open "<<name_label_tag[tok]<<endl;
         tags[tok]=1;
      }
      
      if ((tok>=(GEO+CLOSE))&&(tok<=(OTHER+CLOSE)))
      {
         //cout<<"End "<<name_label_tag[tok-CLOSE]<<endl;
         tags[tok-CLOSE]=0;
      }

      switch (tok) 
      {
      case PHONE:
         //printf("phone: %s<br>\n",webtext);
         //insert_feature(webloc-webleng, webtext, PHONE);
         break;

      case USZIP:
         insert_feature(webloc-webleng, webtext, USZIP);
         break;
    
      case CAZIP:
         insert_feature(webloc-webleng, webtext, CAZIP);
         break;

      case INITCAP:
         insert_feature(webloc-webleng, webtext, INITCAP);
         break;
   
      case ALLCAPS:
         insert_feature(webloc-webleng, webtext, ALLCAPS);
         break;
   
      case CONTAINDIGITS:
         insert_feature(webloc-webleng, webtext, CONTAINDIGITS);
         break;

      case ALLDIGITS:
         insert_feature(webloc-webleng, webtext, ALLDIGITS);
         break;

      case EMAIL:
         //insert_feature(webloc-webleng, webtext, EMAIL);
         break;
      
      case ACRONYM:
         insert_feature(webloc-webleng, webtext, ACRONYM);
         break;
      
      case WORD:
         insert_feature(webloc-webleng, webtext, WORD);
         break;

      case PUNCTUATION:
         //      insert_feature(webloc-webleng, webtext, PUNCTUATION);
         break;

      case DATETIME:
         //printf("datetime: %s\n",webtext);
         break;
      case B_LINE:
         //if it is line_in, change it to line_end
         if ( (tokens->ItemsCount>0) && (((Token*)VectorNth(tokens,tokens->ItemsCount-1))->layout==1) )
         {
            ((Token*)VectorNth(tokens,tokens->ItemsCount-1))->layout=2;
         }
         
         line_flag=0; //start of line
      
         //if ((V.size()>0)&&(V[V.size()-1]->layout==1)) //if it is line_in, change it to line_end
         //   V[V.size()-1]->layout=2;
         //line_flag=0;//start of line

         break;
         
      case E_LINE:
         //if it is line_in, change it to line_end
         if ((tokens->ItemsCount>0) && (((Token*)VectorNth(tokens,tokens->ItemsCount-1))->layout==1))
         {
            ((Token*)VectorNth(tokens,tokens->ItemsCount-1))->layout=2;
         }
         
         line_flag=0;
       
         //if ((V.size()>0)&&(V[V.size()-1]->layout==1)) //if it is line_in, change it to line_end
         //   V[V.size()-1]->layout=2;
         //line_flag=0;

         break;

      case GEO:
         tags[GEO]=1;
         break;      
         
      case GEO+CLOSE:
         tags[GEO]=0;
         //if (V.size()>0) 
         //   V[V.size()-1]->label=3;
         break;

      case C:
         tags[C]=1;
         break;
   
      case C+CLOSE:
         tags[C]=0;
         break;

      case B_SCRIPT:
         tags[SCRIPT]=1;
         break;
         
      case E_SCRIPT:
         tags[SCRIPT]=0;
         break;
       
      case B_STYLE:
         tags[STYLE]=1;
         break;
      
      case E_STYLE:
         tags[STYLE]=0;
         break;
   
      case COMMENT:
         //tags[COMMENT]=1;
         //printf("COMMENT: %s\n",webtext);
         break;
         
      case B_DOCHDR:
         tags[DOCHDR]=1;
         break;
      
      case E_DOCHDR:
         tags[DOCHDR]=0;
         break;
         
      case F_HEAD:
         break;
      
       default:
          break;
      
      }
   }

}



/**
 * usage: getTokens ("/home2/zyu/text2ngram/address.html",tokenVector);
 * -------------------------------
 * read in file stream, parse the html file, extract and save all tokens into tokenVector
 * update: Feb 2, 2005 by Jerry Yu
 */

 void getTokens (char* fileName, cvector *tokensVector)
 
 {
   //set global token vector point to tokensVector
   tokens=tokensVector;
   
   yyin = fopen(fileName, "r");
   if (!yyin)
   {
      printf("Error opening input stream %s\n", fileName);
      return;
   }
   //set_string(source);

   flex_match();
  
   if (yyin) fclose(yyin);

   //  printf("count_term: %d, count_ng: %d\n",count_term,count_ng);
 }

 void getTokensFromText ( const char* text, cvector *tokensVector) 
 {
   char * newText = NULL;

   if ( text )
   {
      //duplicate the text to new text
      int len = strlen(text);
      newText = (char*) malloc( len + 2 );
  
      if ( newText )
      { 
         memcpy ( newText, text, len + 1 ); 
         //append a '\0' to the end of string to make sure it is end with two '\0' for flex to scan
         newText[ len+1 ]= '\0';

         YY_BUFFER_STATE buf_state = web_scan_string( newText );
    
         // set global token vector point to tokensVector
         tokens = tokensVector;

         flex_match();

         web_delete_buffer(buf_state);

         free ( newText );
      }
   }
}
