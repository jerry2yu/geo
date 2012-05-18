#ifndef _GEO_CONFIG_
#define _GEO_CONFIG_

#define DEF_VECTOR_SIZE 104729
#define DEF_FREQ_HASHSET_SIZE 104729
#define DEF_CATEGORIES_SIZE 29
#define DEF_ADDRESS_PER_PAGE 7
#define SHOW_COLLECT_DATA_INTERFACE	1 /*>=1:  show user input to let user judge where extraction is correct, 0: no*/
/**
 * defines the cgi path, cgi application name and extractor html name
 */
#define GEO_URL "http://localhost/cgi-bin/geo?geo="
#define GEO_CGI "geo"
#define HOME_PAGE "http://localhost/geo.html"
#define LIST_FILES_URL "http://localhost/cgi-bin/listFiles"

/**
 * paths to save downloaded and tagged web pages 
 */
// folder of correctly tagged html files
#define PATH_WEBPAGE_TAGGED_CORRECT "/home/jerry2yu/research/geo/data/geodata/tagged/correct/"

// fold of incorretly tagged html files
#define PATH_WEBPAGE_TAGGED_INCORRECT "/home/jerry2yu/research/geo/data/geodata/tagged/incorrect/"

// fold of original html files
#define PATH_WEBPAGE_ORIGINAL "/home/jerry2yu/research/geo/data/geodata/original/"

// a file that contains URLs of all collected web pages
#define PATH_WEBPAGE_URL "/home/jerry2yu/research/geo/data/geodata/url"

/**
 * Dictionary files
 */
#define US_STATES_DATA "/home/jerry/projects/geo/geoextractor/DataSet/dictionary/USStates"
#define SUD_DATA       "/home/jerry/projects/geo/geoextractor/DataSet/dictionary/SecondaryUnitDesignator"
#define STREET_SUFFIXES_DATA "/home/jerry/projects/geo/geoextractor/DataSet/dictionary/StreetSuffixes"
#define OTHER_DATA          "/home/jerry/projects/geo/geoextractor//DataSet/dictionary/others"
#define STREET_DIRECTIONS_DATA "/home/jerry/projects/geo/geoextractor/DataSet/dictionary/StreetDirections"


#endif


