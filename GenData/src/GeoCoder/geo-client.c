/* Copyright 2002 Daniel Egnor.  See LICENSE file.
 *
 * This program uses a "map file" produced by processing TIGER/Line data.
 * It accepts street addresses on standard input, attempts to resolve them
 * to a geographical location, and outputs the result.  This is pretty much
 * just for testing.
 * 
 * The actual work is all done in geo-find.c; this is just a command line
 * wrapper to its geo_find() function. */

#include "io.h"
#include "geo.h"
#include <stdio.h>
#include <string.h>

#ifdef GEO_DEBUG
int main(int argc,char *argv[]) 
{
	struct io_file *index;
	char input[82];
	if (2 != argc) 
	{
		fprintf(stderr,"usage: %s map-file\n",argv[0]);
		return 2;
	}

	index = io_open(argv[1]);
	while (fgets(input,sizeof input,stdin)) 
	{
		struct geo_location loc;
		/* geocoding the input address */
		if (!geo_find(index,input,(int)(strlen(input)),&loc))
			printf("FAILURE: %s\n",input);
		else 
		{
			printf("SUCCESS: %d/%s/%s/%s/%d\n",
			       loc.at.address,
			       loc.street_name,loc.city_name,loc.state_name,
			       loc.zip_code);
			printf("   Side = %c\n",loc.side);
			printf("  Start = %d @ %.8g, %.8g\n",
				loc.before.address,
				loc.before.longitude,loc.before.latitude);
			printf("     At = %d @ %.8g, %.8g\n",
				loc.at.address,
				loc.at.longitude,loc.at.latitude);
			printf("    End = %d @ %.8g, %.8g\n",
				loc.after.address,
				loc.after.longitude,loc.after.latitude);
		}
	}

	return 0;
}
#endif
