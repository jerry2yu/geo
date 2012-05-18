#ifndef _GEO_CONFIG_
#define _GEO_CONFIG_

#define DEF_VECTOR_SIZE 104729
#define DEF_FREQ_HASHSET_SIZE 104729
#define DEF_CATEGORIES_SIZE 29
#define DEF_ADDRESS_PER_PAGE 7
#define SHOW_COLLECT_DATA_INTERFACE	1 /*>=1:  show user input to let user judge where extraction is correct, 0: no*/
#define GEO_URL "http://192.168.50.32/cgi-bin/geo.cgi?geo="
#define GEO_CGI "geo"
#define HOME_PAGE "http://192.168.50.32/geo.html"
/* paths to save downloaded and tagged web pages */
#define PATH_WEBPAGE_TAGGED_CORRECT "/home/workspace/geo/data/geodata/tagged/correct/"
#define PATH_WEBPAGE_TAGGED_INCORRECT "/home/workspace/geo/data/geodata/tagged/incorrect/"
#define PATH_WEBPAGE_ORIGINAL "/home/workspace/geo/data/geodata/original/"
#define PATH_WEBPAGE_URL "/home/workspace/geo/data/geodata/url"
#define LIST_FILES_URL "http://192.168.50.32/cgi-bin/listFiles.cgi"

#endif


