#ifndef _GEO_TOKEN_
#define _GEO_TOKEN_

/***************************
* Token structure with features
* label: 0: NOTGEO 1: ISGEO 
* last update: Jan 31, 2004
***************************/
typedef struct {
  char* term;
  int local;
  int layout;
  int punctuation;
  int label; 
  int label_tag; /* tags: e.g. GEO, STREET */
  // followings are dictionary feature
  int is_usstate; //token a us state, default 0: not
  int is_sud; //token is Secondary Unit Designator, such as APT, FLOOR, default 0: not
  int is_ss; // token is Street Suffixes, such as AVE, Road, default 0: not
  int is_sd; // token is Street Direction, such as South, North. default 0: not
  int is_others; // token is other words, such as POB, university. default 0: not
  int category;
  int position; //token position in the text
} Token;

#endif
