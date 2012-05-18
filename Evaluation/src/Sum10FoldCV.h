/**
 * Read in the output file by GetExtracitonResult, sum all the accuracy results for each fold of 
 * 10 fold cross validations
 * Output the average accuracy results for the whole set of 10 fold cross validations
 */

#ifndef SUM10FOLDCV_H
#define SUM10FOLDCV_H

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../../GenData/src/mystring.h"
#include "../../GenData/src/Config.h"

#define MAX_BUFFER_LEN            1024

class Sum10FoldCV
{
 public:
  Sum10FoldCV();
  ~Sum10FoldCV();
  /* open the 10 fold CV results */
  void openResult( const char * fileName );
  bool parseResult( const char * result );
  void outputSummary();
 private:

  int fold; /* total number of fold in the result file */
  float tmpPrecision, tmpRecall, tmpF1;
  float precision, recall, f1;
  float precisionStart, recallStart, f1Start;
  float precisionMid, recallMid, f1Mid;
  float precisionEnd, recallEnd, f1End;
  float precisionOther, recallOther, f1Other;
  int totalAddress, truePositive, falsePositive;
  
};

#endif
 
