
#include "GetExtractionResult.h"
#define TEST_START_END_MATCHING  /* uncomment to test START and END of address label matching */
#define OUTPUT_FILE_NAME       "result.tmp" /* ouput the result to the tmp file for later summarization */

GetExtractionResult::GetExtractionResult() : 
  startMatched(false), totalMatches(0), totalAddresses(0), isPredictionStart(false), isLabelStart(false), geoPredictionFound(0),
  truePositive(0), falsePositive(0), startIndex(0),totalEnd(0),fp(0)
{
  startCount=startWrongCount=startRightCount=
  endCount=endWrongCount=endRightCount=
  midCount=midWrongCount=midRightCount=
  otherCount=otherWrongCount=otherRightCount=0;
  totalPrecision=totalRecall=totalFmeasure=0.0;
}

GetExtractionResult::~GetExtractionResult()
{
}

#if 0
void GetExtractionResult::matchExtraction( string & prediction, string & label )
{
  if ( label == "START" )
    {
      if ( prediction == label )
	{
	  startMatched = true;
	}
      else
	{
	  startMatched = false;
	}
    }
  else if ( label == "END" )
    {
      ++totalAddresses;/* total number of lablled addresses */
      if ( startMatched && prediction == label )
	{
	  ++totalMatches;/* total number of extract address matches */
	}
      startMatched = false;
    }
 
}
#endif

void GetExtractionResult::outputResultByClass( int totalCount, int rightCount, int wrongCount, const char * className )
{
  float precision = 0==(rightCount+wrongCount)? 0: (float)rightCount /(float)(rightCount+wrongCount);
  float recall = totalCount==0?0:(float)rightCount /(float)totalCount;
  float fmeasure = rightCount == 0 ? 0.0 : (float)( 2 * precision * recall )/ (float)(precision+recall);
  printf("%s: Precision %f, Recall %f F-measure %f\n", className, precision, recall, fmeasure );
  if ( fp )
    {
      fprintf( fp, "%f %f %f\n",precision, recall, fmeasure );
    }
  totalPrecision+=precision;
  totalRecall+=recall;
  totalFmeasure+=fmeasure;
}

void GetExtractionResult::outputResult()
{
  //totalPredictedPositve = truePositive + falsePositive;
  //totalPositive = totalAddresses;
  float precision = (truePositive+falsePositive) == 0 ? 0: (float)truePositive / (float)(truePositive+falsePositive );
  float recall = (float)truePositive / (float)totalAddresses;
  float fmeasure = truePositive==0?0.0:(float)(2 * precision * recall ) / (float)(precision+recall);


  fp = fopen( OUTPUT_FILE_NAME, "a+" );
  if ( fp )
    {
      fprintf( fp, "%f %f %f\n", precision, recall, fmeasure );
      fprintf( fp, "%d %d, %d\n", totalAddresses, truePositive, falsePositive );
    }
      
  printf("Precision %f, Recall %f, F-Meassure %f, TotalAddresses %d, True Positive %d, False Positive %d\n", precision, recall, fmeasure, totalAddresses, truePositive, falsePositive );

  /* get average recall,precsion,f1 by class */
  //printf("Startcount %d, %d, %d\n", startCount, startRightCount, startWrongCount );
  outputResultByClass( startCount, startRightCount, startWrongCount, "START" );
  outputResultByClass( midCount, midRightCount, midWrongCount, "MID" );
  outputResultByClass( endCount, endRightCount, endWrongCount, "END" );
  outputResultByClass( otherCount, otherRightCount, otherWrongCount, "OTHER" );
  printf("Average: Precision %f, Recall %f, F-Measure %f\n", totalPrecision/4, totalRecall/4, totalFmeasure/4  );
  if ( fp ) fclose( fp );


}

void GetExtractionResult::getClassPerformance( int index, string & prediction, string & label )
{
  if ( label == "OTHER" )
    {
      ++otherCount;
    }
  if ( label == "MID" )
    {
      ++midCount;
    }
  if ( label =="END" )
    {
      ++endCount;
    }
  if ( label == "START" )
    {
      ++startCount;
    }
  if ( prediction == "OTHER" )
    {
      label == "OTHER" ? ++otherRightCount : ++otherWrongCount;
    }
  if ( prediction == "MID" )
    {
      label == "MID" ? ++midRightCount : ++midWrongCount;
    }
  if ( prediction == "START" )
    {
      label == "START" ? ++startRightCount : ++startWrongCount;
    }
  if ( prediction == "END" )
    {
      label == "END" ? ++endRightCount  : ++endWrongCount;
    }

}
void GetExtractionResult::scanWekaOutput( int index, string & prediction, string & label )
{
  //   printf("scannWekaOutput %d %s - %s\n", index, prediction.c_str(), label.c_str() );
  /* count number of labelled addresses */
  if ( label == "START" )
    {
      ++totalAddresses;
    }
#ifdef TEST_START_END_MATCHING
  if ( label == "END" )
    {
      ++totalEnd;
      if ( totalEnd != totalAddresses )
	{
	  printf("not match start with end\n");
	  exit(0);
	}
    }
#endif  

  if ( prediction == "MID" )
    {
      ++geoPredictionFound;
    }
  if ( prediction == "START" )
    {
      startMatched = label == "START";
      startIndex = index;
      isPredictionStart = true;
      ++geoPredictionFound;
    }
  else if ( prediction == "END" )
    {
      ++geoPredictionFound;
      if ( isPredictionStart )
	{ 
	  if ( geoPredictionFound >= 3 && index - startIndex < 20 )
	    { /* in the segment, there is at least 3 tokens classifed as START, MID or END and total number tokens in the segment should less than 20 */
	      //printf("%d-%d\n", startIndex, index );
	      /* found a positive prediction */  
	      if ( startMatched && label == "END" )
		{ /* both start and end matched, true positive */
		  ++truePositive;
		}
	      else
		{ /* either start or end not matched, false positive */
		  ++falsePositive;
		}

	      geoPredictionFound=0; /* reset counter */
	    }
	  isPredictionStart = false;

	}

    }
  
  

}

void GetExtractionResult::openWekaPrediction( const char * fileName )
  {
    assert( fileName );
    int row = 0;
    /* reading weka prediction file name */
    FILE * fp = fopen( fileName,"r");
    if ( NULL != fp )
    {
      char buffer[ MAX_BUFFER_LEN ];

	  /* read in each line */
	  while( fgets( buffer, MAX_BUFFER_LEN, fp ) != NULL )
	  {

	    if ( strlen( buffer ) > 0 )
	    {
	      //	      printf("%s\n", String(buffer).trimEnd("\n").c_str() );
	      string str = buffer;
	      str.trimEnd("\n");
	      int index = str.indexOf( " " );
	      if ( index != -1 )
		{
		  int len = str.indexOf( " ", index+1 );
		  if ( len != -1 )
		    {
		      string prediction = str.substring( index+1, len-index-1 );
		      //printf("%s.", prediction.c_str() );
		      index = str.indexOf( " ", len+1 );
		      if ( index != -1 )
			{
			  string label = str.substring( index+1 ).trimEnd(" ");
			  //printf("-%s.\n", label.c_str() );
			  //printf("Processing %d - %s\n", row, buffer);
			  scanWekaOutput( row, prediction, label );
			  /* calculate average precision, recall, and f-measure by class */
			  getClassPerformance( row, prediction, label );
			  ++row;
			}
		    }
		}
	    }
       }
       fclose( fp );
    }
    else
    {
      fprintf( stderr, "Can not open file %s.\n", fileName );
    }

  }

int main( int argc, char ** argv )
{
    char fileName[ MY_PATH_MAX ];
    if (argc < 2) 
    {
      fprintf( stderr, "\nOOPS!  This program takes one argument which is Weka prediction file name\n");
      return 0;
    } 

    GetExtractionResult getExtractionResult;
    getExtractionResult.openWekaPrediction( argv[1] );
    getExtractionResult.outputResult();


    //    printf("Total %d addresses corrected predicted by the classifier from total %d labelled addresses\n", getExtractionResult.totalMatches , getExtractionResult.totalAddresses );


}
