/**
 * Read in Weka prediction data, and compare the predicted label with the actual label.
 * Only both START and END matches will be counted as a correct address extraction
 */
#ifndef GET_EXTRACTION_RESULT
#define GET_EXTRACTION_RESULT			\

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../../GenData/src/mystring.h"
#include "../../GenData/src/Config.h"

#define MAX_BUFFER_LEN            1024

class GetExtractionResult
{
 public:
  GetExtractionResult();
  ~GetExtractionResult();
  void openWekaPrediction( const char * fileName );
  //  void matchExtraction( string & prediction, string & label );
  /**
   * @param index - current position of token
   */
  void scanWekaOutput( int index, string & prediction, string & label );
  /**
   * calculate the average precision, recall and f-measure by class
   */
  void getClassPerformance( int index, string & prediction, string & label );
  void outputResultByClass( int totalCount, int rightCount, int wrongCount, const char * className );
  void outputResult();
  bool startMatched;
  int totalMatches;   /* total number of correctly predicted addresses */
  int totalAddresses; /*total number of labelled addresses */
  bool isPredictionStart, isLabelStart; /* mark for the prediction and label state, true: there is START found in current scanning processing*/
  int geoPredictionFound; /* total number of geo prediction found ( START, MID, END )*/
  int truePositive;
  int falsePositive;
  int startIndex;/*last position that prediction START is found */
  int totalEnd;
  

  /* for partial matching evaluation */
  int startCount;
  int startRightCount;
  int startWrongCount;
  int endCount;
  int endRightCount;
  int endWrongCount;
  int midCount;
  int midRightCount;
  int midWrongCount;
  int otherCount;
  int otherRightCount;
  int otherWrongCount;
  float totalPrecision;
  float totalRecall;
  float totalFmeasure;
 private:
  FILE * fp;
  
};

#endif
 
