/**
 * Split filelist into testingFileList, and n fold training and validating file lists  by given ratio and fold
 * E.g.: parameters 0.2   5  will output 0.2 testing, 0.8 will be used to output 5 fold training and validating file lists;
 * @output files: one testingFileList and n training and validating filelists
 */
#include "Config.h"
#include <stdio.h>
#include "mystring.h"
#include <stdlib.h>
#include "vector.h"

float ratio = 0.0;
Vector<String*> files;
FILE * fp = NULL;
const char * trainingFileName = "splitFileListTraining";
const char * testingFileName = "splitFileListTesting";
const char * validatingFileName = "splitFileListValidating";
/* randomly reorder files */
void shuffleFiles()
{
   srand( time(NULL) );
   Vector<String*> newFiles;
   int count = files.count();
   for ( int i=0; i<count; i++ )
    {
      int index = rand() % files.count();
      newFiles.add( files[index] );
      files.removeAt( index );
    }
  files.clear();
  for ( int i=0; i<count; i++ )
    {
      files.add( newFiles[i] );
    }
  newFiles.clear();
}
void outputTestingFiles( )
{
  srand( time(NULL) );
  int index = 0;
  int count = (int)( (float)(files.count()) * ratio );
  printf("count %d\n", count );
  assert( count < files.count() );
  printf("********** Testing File List Total: %d files**********\n", count);
  fp = fopen( testingFileName, "w+" );
  if ( fp )
  {
    while ( count-- > 0 )
    {
      index = rand() % files.count();
      //     printf("Random number %d\n", index );
      String * s = files[index];
      fprintf( fp, "%s\n", s->c_str() );
      delete s;
      files.removeAt(index);
    }
    fclose( fp );
  }
}

void outputValidatingFiles( int fold, int index )
{
  char fileName[ MY_PATH_MAX ];
  int count = files.count() / fold;
  printf("********** Validating File List %d Total: %d files**********\n", index+1, count );
  sprintf( fileName, "%s%d", validatingFileName, index+1 );
  fp = fopen( fileName, "w+" );
  if ( fp )
  {
    for ( int i=index*count; i<files.count() && i<(index+1)*count; i++ )
    {
      String * s = files[i];
      fprintf( fp, "%s\n", s->c_str() );
    }
    fclose( fp );
  }
}

void outputTrainingFiles( int fold, int index )
{
  char fileName[ MY_PATH_MAX ];
  int count = files.count() / fold;
  printf("********** Training File List %d Total: %d files**********\n", index+1, files.count()- count );
  sprintf( fileName, "%s%d", trainingFileName, index+1 );
  fp = fopen( fileName, "w+" );
  if ( fp )
  {
    for ( int i=0; i<files.count(); i++ )
    {
      if ( i<index*count || i >= (index+1)*count )
	{
	  String * s = files[i];
	  fprintf( fp, "%s\n", s->c_str() );
	}
    }
    fclose( fp );
  }
}

void outputNFoldCrossValidationFiles( int fold )
{
  for ( int i=0; i<fold; i++ )
    {
      outputValidatingFiles( fold, i );
      outputTrainingFiles( fold, i );
    }
}

void clearFiles()
{
  for ( int i=0; i<files.count(); i++ )
    {
      delete files[i];
    }
  files.clear();
}

void splitFileList( int fold )
{
  for ( int i=0; i<10; i++ )
    shuffleFiles();
  outputTestingFiles();
  outputNFoldCrossValidationFiles( fold );
  clearFiles();
}
int main( int argc, char ** argv )
{
  /* delay at least 1 second, to get a different random seed */
  //  for ( double i=0; i<399999999; i++ )
  for ( double i=0; i<0; i++ )
    {
      float j=100.0*100.0;
    }

    char fileName[ MY_PATH_MAX ];
    if (argc < 2) 
    {
      fprintf( stderr, "\nOOPS! Usage: %s fileList, splitRatio, fold-n\n", argv[0] );
      fprintf( stderr, "E.g.: %s 0.2 5 will output 0.2 testing, 0.8 will be used to output 5 fold training and validating file lists\n", argv[0] );
      return 0;
    } 


    if (argc>2)
    {
      sscanf( argv[2], "%f", &ratio );
      //printf("Ratio: %f\n", ratio );
    }
    int fold=0;
    if ( argc>3 )
      {
	sscanf( argv[3], "%d", &fold );
      }

	/* reading list of file names */
    FILE * fileList = fopen( argv[1],"r");
    if ( NULL != fileList )
    {

	  /* read in each line */
	  while( fgets( fileName, MY_PATH_MAX, fileList ) != NULL )
	  {

	    if ( strlen( fileName ) > 0 )
	    {
	      String * s = new String(fileName);
	      s->trimEnd("\n");
	      files.add( s );
	    }
       }
       fclose( fileList );
    }
    else
    {
      fprintf( stderr, "Can not open file %s.\n", argv[1] );
    }
    splitFileList( fold );

    return 0;
} 
