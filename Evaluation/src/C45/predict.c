/*************************************************************************/
/*									 */
/*  Main routine for constructing sets of production rules from trees	 */
/*  -----------------------------------------------------------------	 */
/*									 */
/*************************************************************************/


#include "defns.i"
#include "types.i"
//#define SHOW_INFO /* show debug info */



	/*  External data.  Note: uncommented variables have the same meaning
	    as for decision trees  */

short		MaxAtt, MaxClass, MaxDiscrVal;

ItemNo		MaxItem;

Description	*Item;

DiscrValue	*MaxAttVal;

char		*SpecialStatus;

String		*ClassName,
		*AttName,
		**AttValName,
		FileName = "DF";

short		VERBOSITY = 0,
		TRIALS;

Boolean		UNSEENS	  = false,
		SIGTEST	  = false,	/* use significance test in rule pruning */
		SIMANNEAL = false;	/* use simulated annealing */

float		SIGTHRESH   = 0.05,
		CF	    = 0.25,
		REDUNDANCY  = 1.0;	/* factor that guesstimates the
					   amount of redundancy and
					   irrelevance in the attributes */

PR		*Rule;			/* current rules */

RuleNo		NRules = 0,		/* number of current rules */
		*RuleIndex;		/* rule index */

short		RuleSpace = 0;		/* space allocated for rules */

ClassNo		DefaultClass;		/* current default class */

RuleSet		*PRSet;			/* sets of rulesets */

float		AttTestBits,		/* bits to encode tested att */
		*BranchBits;		/* ditto attribute value */



    main(Argc, Argv)
/*  ----  */
    int Argc;
    char *Argv[];
{
    int o;
    extern char *optarg;
    extern int optind;
    Boolean FirstTime=true;
#ifdef SHOW_TEXT
    PrintHeader("rule generator");
#endif

    /*  Process options  */

    while ( (o = getopt(Argc, Argv, "f:uv:c:r:F:a")) != EOF )
    {
	if ( FirstTime )
	{
	  // printf("\n    Options:\n");
	    FirstTime = false;
	}

	switch (o)
	{
	    case 'f':	FileName = optarg;
#ifdef SHOW_TEXT
	      printf("\tFile stem <%s>\n", FileName);
#endif
			break;
	    case 'u':	UNSEENS = true;
#ifdef SHOW_TEXT
	      	printf("\tRulesets evaluated on unseen cases\n");
#endif
			break;
	    case 'v':	VERBOSITY = atoi(optarg);
#ifdef SHOW_TEXT
	      	printf("\tVerbosity level %d\n", VERBOSITY);
#endif
			break;
	    case 'c':	CF = atof(optarg);
#ifdef SHOW_TEXT
	      printf("\tPruning confidence level %g%%\n", CF);
#endif
			Check(CF, 0, 100);
			CF /= 100;
			break;
	    case 'r':	REDUNDANCY = atof(optarg);
#ifdef SHOW_TEXT
	      printf("\tRedundancy factor %g\n", REDUNDANCY);
#endif
			Check(REDUNDANCY, 0, 10000);
			break;
	    case 'F':	SIGTHRESH = atof(optarg);
#ifdef SHOW_TEXT
	      printf("\tSignificance test in rule pruning, ");
	      printf("threshold %g%%\n", SIGTHRESH);
#endif
			Check(SIGTHRESH, 0, 100);
		 	SIGTHRESH /= 100;
			SIGTEST = true;
			break;
	    case 'a':	SIMANNEAL = true;
#ifdef SHOW_TEXT
	      printf("\tSimulated annealing for selecting rules\n");
#endif
			break;
	    case '?':	
#ifdef SHOW_TEXT
	      printf("unrecognised option\n");
#endif
			exit(1);
	}
    }

    /*  Initialise  */

    GetNames();
    GetData(".data");
#ifdef SHOW_TEXT
    printf("\nRead %d cases (%d attributes) from %s\n",
	   MaxItem+1, MaxAtt+1, FileName);
#endif

    GenerateLogs();

    /*  Construct rules  */

    GenerateRules();

    /*  Evaluations  */
    #ifdef SHOW_TEXT
    printf("\n\nEvaluation on training data (%d items):\n", MaxItem+1);
    #endif 
    EvaluateRulesets(true);
    

    /*  Save current ruleset  */

    SaveRules();

    if ( UNSEENS )
    {
	GetData(".test");
#ifdef SHOW_TEXT
		printf("\nEvaluation on test data (%d items):\n", MaxItem+1);
#endif
	EvaluateRulesets(false);
    }

    exit(0);
}