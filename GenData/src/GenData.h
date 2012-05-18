#ifndef GENDATA_H_
#define GENDATA_H_

#include <stdio.h>
#include "GeoTokenizer.h"
#include "WekaWordNgrams.h"
#include "C45WordNgrams.h"
#include "GeoCharNgrams.h"
#include "POS/BrillTagger.h"
#include "GeoCoder/GeoCoder.h"
#include "WekaHandler.h"
#include "Dictionary.h"
#include "Config.h"
#include "RE.h" /* Rule Based Extraction */

class GenData
{
 public:
  GenData();
  ~GenData();

  /**
   * process the given text file
   * @param fileName - name of the text file to be processed
   * @param ngramNumber - ngrams num 
   * @return true if succeeded, otherwise false
   */
  bool process( const char * fileName, int ngramNumber );

  void clear();

 private:
  Vector<IToken*> tokens;
  BrillTagger brillTagger; /* Brill Part of Speech Tagger */
  GeoCoder geoCoder;  /* Dan's Geocoder */
  Dictionary dic;
  RE re;
};

#endif
