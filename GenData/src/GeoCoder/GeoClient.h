#ifndef GEOCLIENT_H_
#define GEOCLIENT_H_
extern "C"
{
#include "io.h"
#include "geo.h"
}
#include <stdio.h>
#include <string.h>

class GeoClient
{
public:
	GeoClient();
	~GeoClient();
	
	bool open()
	{
		index = io_open( "C:\\project\\research\\geocoder\\geocoder\\Debug\\all-map" );
		return NULL != index;
	}

	void close()
	{
		io_close( index );
	}

private:
	io_file * index;
};

#endif