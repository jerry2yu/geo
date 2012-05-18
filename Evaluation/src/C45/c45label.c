/*
 * c45label.c, by Michael Eng <meng@ed.ac.uk>
 * 
 * based on bconsult.c v1.9, by Johann Petrak
 *   Copyright (1997) Austrian Research Institute
 *   for Artificial Intelligence, Vienna, Austria
 *
 * which in turn is based on J.R. Quinlan's original code for C4.5
 *
 * Use the following rule to build:
 * c45label: c45label.o rules.o userint.o $(core)
 *   cc -o c45label c45label.o classify.o $(core)
 *
 * you will also need to patch classify.c to include CategoryWt/3
 *
 */

#include "defns.i"
#include "types.i"

/*  External data, described in extern.i  */

unsigned int NrMVClass = 0;
short		MaxAtt, MaxClass, MaxDiscrVal = 2;
ItemNo		MaxItem;
Description	*Item;
DiscrValue	*MaxAttVal;
char		*SpecialStatus;

String		*ClassName,
  *ClassAttr,
  *AttName,
  **AttValName,
  ModelSuffix = 0,
  FileName = "DF";

short		VERBOSITY = 0,
  IGNOREMVCLASS = 0,
  TRIALS    = 10;

Boolean         PRINTTREE = true,
  PRUNETREE  = true;

Boolean		GAINRATIO  = true,
  SUBSET     = false,
  BATCH      = true,
  UNSEENS    = false,
  PROBTHRESH = false,
  TRACE      = false,
  TRAIN      = false,
  CONFIDENCE = false,  /* include confidence in output */
  UNPRUNED   = false;

ItemNo		MINOBJS   = 2,
  TARGETCOLUMN = 0,
  WINDOW    = 0,
  INCREMENT = 0;

float		CF = 0.25;

Tree		*Pruned;

Tree   DecisionTree,			/* tree being used */
  GetTree();

Boolean		AllKnown = true;

char suffixbuf[256];


/*
 * the important part, to write the labels to the output file
 */

WriteLabs(Extension)
     String Extension;
{

  char InFn[500];
  char Fn[500];
  FILE *TRf = 0;
  
  ClassNo RealClass, PrunedClass, Category();

  float confidence;
  short t;

  ItemNo *ConfusionMat, i, PrunedErrors;

  /* output file name */
  strcpy(Fn, FileName);
  strcat(Fn, Extension);

  /* input file name */
  strcpy(InFn, FileName);
  strcat(InFn, TRAIN ? ".data" : ".test");
  
  FILE *InFd = fopen(InFn, "r");
  char inLine[500];
  char* pinLine;
  if ( ! ( TRf = fopen(Fn, "w") ) ) Error(0, Fn, " for writing");
  
  ForEach(i, 0, MaxItem) {
    
    /* get line from input file, strip trailing newline */
    fgets(inLine, 500, InFd);
    inLine[strlen(inLine)-1] = '\0';
    
    /* get class and its' confidence */
    PrunedClass = CategoryWt(Item[i], DecisionTree,&confidence);

    /* write it to the output, together with what we are classifying */
#if 0
    if (CONFIDENCE) 
      fprintf(TRf,"%s,%s,%.5f\n",inLine,ClassName[PrunedClass],confidence);
    else
      fprintf(TRf,"%s,%s\n",inLine,ClassName[PrunedClass]);
#endif
      fprintf( TRf, "%d %s %f %s\n", i, ClassName[PrunedClass], CONFIDENCE ? confidence : 0.0,  ClassName[Class(Item[i])] );
   /* if (i%(MaxItem/10)==0) {
      printf("... %i to go\n",(MaxItem-i));
    }
*/
    
  }

  //printf("all done.\n\n");
  
  fclose(TRf);
  
}


    main(Argc, Argv)
/*  ----  */
    int Argc;
    char *Argv[];
{
    int o;
    extern char *optarg;
    extern int optind;
    Boolean FirstTime=true;
    short Best, BestTree();

    //printf("BEWARE this is Michael's Special Edition hacked version");
    PrintHeader("label examples using tree");

    /*  Process options  */

    while ( (o = getopt(Argc, Argv, "f:v:tcuT:lM:hI:")) != EOF )
    {
	if ( FirstTime && o != 'h')
	{
//	    printf("\n    Options:\n");
	    FirstTime = false;
	}

	switch (o)
	{
	case 'f':   FileName = optarg;
//		    printf("\tFile stem <%s>\n", FileName);
		    break;
	case 'M':   ModelSuffix = optarg;
		    printf("\tModel name <%s>\n", ModelSuffix);
		    break;
	case 'v':   VERBOSITY = atoi(optarg);
		    printf("\tVerbosity level %d\n", VERBOSITY);
		    break;
	case 't':   TRACE = true;
		    printf("\tPrint tree\n", VERBOSITY);
		    break;
	case 'u':   UNPRUNED = true;
		    printf("\tUse unpruned tree\n", VERBOSITY);
		    break;
	case 'l':   TRAIN = true;
		    printf("\tUse training (learning) set\n", VERBOSITY);
		    break;
	case 'c':   CONFIDENCE = true;
		    printf("\tInclude confidence in output file\n", VERBOSITY);
		    break;
	case 'T':   TARGETCOLUMN = atoi(optarg);
	            printf("\tTarget attribute in column %d\n", TARGETCOLUMN);
		    break;
	case 'I':   IGNOREMVCLASS = atoi(optarg);
	            if (IGNOREMVCLASS == 0)
		      printf("\tStandard handling of MVs as class values\n");
		    else if  (IGNOREMVCLASS == 1)
		      printf("\tIgnore cases with MV as class value\n");
		    else if  (IGNOREMVCLASS == 2)
		      printf("\tSubstitute first class label instead of MVs\n");
		    else {
		      printf("%s not a valid argument for option -I\n",optarg);
		      exit(1);
		    }
		    break;
	  case 'h': printf("usage: %s [options]\n", Argv[0]);
	            printf("  options: [-f<fstem>] [-M<name>] [-u] [-l] [-v<n>] [-t] [-c] [-T<n>] \n");
		    printf("  -f<fstem>: filestem (DF)\n");
		    printf("  -M<name>:  create <fstem><name>.tree/.unpruned ('')\n");
		    printf("  -u:        use unpruned tree (use pruned)\n");
		    printf("  -l:        use training set (test set)\n");
		    printf("  -v<n>:     verbosity level (0)\n");
		    printf("  -t:        print tree (dont print tree)\n");
		    printf("  -T<n>:     target is attribute n, use changed namesfile (last attr)\n");
		    printf("  -I<n>:     n=0: treat MV in class as error (default)\n");
		    printf("             n=1: ignore records w/ MV in class\n");
		    printf("             n=2: substitute first class label  (treat as error)\n");
		    printf("  -c:        include confidence in output file\n");
		    printf("  -h:        show this help info\n");
		    printf("\n\n  The program will create a file <fstem>.classif that contains\n  the assigned class labels plus its' entry in the input.\n");
		    exit(1);
		    break;
	case '?':   printf("unrecognised option, use -h for help\n");
		    exit(1);
	}
    }

    /*  Initialise  */

    GetNames();
    
    GetData(TRAIN ? ".data" : ".test");
    printf("\nRead %d cases (%d attributes) from %s.%s\n",
	   MaxItem+1, MaxAtt+1, FileName, (TRAIN ? "data" : "test"));

  /*  if (IGNOREMVCLASS == 1)
      printf("\nIgnored %d records with '?' as class label\n", NrMVClass);
    else if ((IGNOREMVCLASS == 2) || (NrMVClass > 0))
      printf("\nReplaced '?' in %d cases with first class label\n", NrMVClass);
*/
    /* read the tree */
    DecisionTree = GetTree((UNPRUNED ? ".unpruned" : ".tree"));
    if ( TRACE ) PrintTree(DecisionTree);

    /*GetData(".test");*/
//    printf("\nGenerate labels file from %s data (%d items):\n", 
//	   (TRAIN ? "training" : "test"), MaxItem+1);
    
    if (ModelSuffix == 0)
      sprintf(suffixbuf,".classif");
    else
      sprintf(suffixbuf,"%s.classif",ModelSuffix);
    WriteLabs(suffixbuf);


    exit(0);
}




