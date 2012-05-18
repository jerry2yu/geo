
#include "Sum10FoldCV.h"

#define INPUT_FILE_NAME       "result.tmp" /* the 10 fold Cross Validation result file that output by getExtractionResult for summarization */

Sum10FoldCV::Sum10FoldCV() :
  fold(0),
  tmpPrecision(0.0), tmpRecall(0.0), tmpF1(0.0),
  precision(0.0), recall(0.0), f1(0.0),
  precisionStart(0.0), recallStart(0.0), f1Start(0.0),
  precisionMid(0.0), recallMid(0.0), f1Mid(0.0),
  precisionEnd(0.0), recallEnd(0.0), f1End(0.0),
  precisionOther(0.0), recallOther(0.0), f1Other(0.0),
  totalAddress(0), truePositive(0), falsePositive(0)
{
}

Sum10FoldCV::~Sum10FoldCV()
{
}

bool Sum10FoldCV::parseResult( const char * result )
{
  bool ret = false;
    if ( result && strlen( result ) > 0 )
    {
      //      printf("%s\n", String(result).trimEnd("\n").c_str() );
      string str = result;
      str.trimEnd("\n");
      int index = str.indexOf( " " );
	
      if ( index != -1 )
	{
	  string precision = str.substring( 0, index );
	  sscanf( precision.c_str(), "%f", &tmpPrecision );
	  int len = str.indexOf( " ", index+1 );
	  if ( len != -1 )
	    {
	      string recall = str.substring( index+1, len-index-1 );
	      string f1 = str.substring( len+1 ).trimEnd(" ");
	      sscanf( recall.c_str(), "%f", &tmpRecall );
	      sscanf( f1.c_str(), "%f", &tmpF1 );
	      // printf("Precision %f, Recall: %f f1 %f.\n", tmpPrecision, tmpRecall, tmpF1 );
	      ret = true;

	    }
	}
    }
    return ret;
}

void Sum10FoldCV::outputSummary()
{
  printf("fold %d\n", fold );
  printf("Precision %f Recall %f F1 %f\n", precision/fold, recall/fold, f1/fold );
  printf("Total %d address True Positive %d False Positive %d\n", totalAddress/fold, truePositive/fold, falsePositive/fold );
  printf("Start Precision %f Recall %f F1 %f\n", precisionStart/fold, recallStart/fold, f1Start/fold );
  printf("Mid Precision %f Recall %f F1 %f\n", precisionMid/fold, recallMid/fold, f1Mid/fold );
  printf("End Precision %f Recall %f F1 %f\n", precisionEnd/fold, recallEnd/fold, f1End/fold );
  printf("Other Precision %f Recall %f F1 %f\n", precisionOther/fold, recallOther/fold, f1Other/fold );
  printf("Average Precision %f Recall %f F1 %f\n", 
	 (precisionStart+precisionMid+precisionEnd+precisionOther)/(4*fold),
	 (recallStart+recallMid+recallEnd+recallOther )/(4*fold),
	 (f1Start+f1Mid+f1End+f1Other)/(4*fold) );
}
void Sum10FoldCV::openResult( const char * fileName )
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
	    if (  parseResult( buffer ) )
	      {
		if ( row % 6 == 0 )
		  {
		    precision += tmpPrecision;
		    recall += tmpRecall;
		    f1 += tmpF1;
		  }
		if ( row %  6 == 1 )
		  {
		    /* true positive */
		    totalAddress += (int)tmpPrecision;
		    truePositive += (int)tmpRecall;
		    falsePositive += (int)tmpF1;
		    
		  }
		if ( row % 6  == 2 )
		  {
		    precisionStart += tmpPrecision;
		    recallStart += tmpRecall;
		    f1Start += tmpF1;
		  }
		if ( row % 6 == 3 )
		  {
		    precisionMid += tmpPrecision;
		    recallMid += tmpRecall;
		    f1Mid += tmpF1;
		  }
		if ( row % 6 == 4 )
		  {
		    precisionEnd += tmpPrecision;
		    recallEnd += tmpRecall;
		    f1End += tmpF1;
		  }
		if ( row % 6 == 5 )
		  {
		    precisionOther += tmpPrecision;
		    recallOther += tmpRecall;
		    f1Other += tmpF1;
		  }
		
		++row;
		//printf("row %d\n", row );
		if ( row % 6 == 0 )
		  ++fold;
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
      fprintf( stderr, "\nOOPS!  This program takes one argument which is 10 fold cross validation result file\n");
      return 0;
    } 

    Sum10FoldCV sum10FoldCV;
    sum10FoldCV.openResult( argv[1] );
    sum10FoldCV.outputSummary();



}
