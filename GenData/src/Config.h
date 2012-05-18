#ifndef CONFIG_H_
#define CONFIG_H_
#include <limits.h>
/**
 * Defines all configurations in this file
 */

/* default number of ngram */
#define NGRAM_NUM_DEFAULT       3

#define MY_PATH_MAX             2048                 /* max number of characters in a path name */
#ifdef WIN32
#define GEO_MAP_FILE        "C:\\project\\research\\geo\\data\\geocoder\\all-map"
#define US_STATE_FILE "C:\\project\\research\\geo\\data\\USStates"
#define STREET_DIRECTION_FILE "C:\\project\\research\\geo\\data\\StreetDirections"
#define STREET_SUFFIX_FILE "C:\\project\\research\\geo\\data\\StreetSuffixes"
#define SECONDARY_UNIT_DESIGNATOR_FILE "C:\\project\\research\\geo\\data\\SecondaryUnitDesignator"
#define OTHERS_FILE "C:\\project\\research\\geo\\data\\others"
#else
#define GEO_MAP_FILE        "../../data/geocoder/all-map"
/* dictionary of US states */
#define US_STATE_FILE "../../data/dictionary/USStates"
/* dictionary of Street Directions, such as South, north */
#define STREET_DIRECTION_FILE "../../data/dictionary/StreetDirections"
/* dictionary of Street Suffixes, such as AVE, Road */
#define STREET_SUFFIX_FILE "../../data/dictionary/StreetSuffixes"
/* dictionary of Secondary Unit Designator, such as APT, FLOOR */
#define SECONDARY_UNIT_DESIGNATOR_FILE "../../data/dictionary/SecondaryUnitDesignator"
/* dictionary of other tokens, such University, POB ... */
#define OTHERS_FILE "../../data/dictionary/others"
#endif













#endif /*CONFIG_H_*/
