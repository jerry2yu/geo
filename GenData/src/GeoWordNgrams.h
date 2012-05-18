#ifndef GEOWORDNGRAMS_H_
#define GEOWORDNGRAMS_H_

#include "GeoToken.h"
#include "WordNgrams.h"

class GeoWordNgrams : public WordNgrams
{
 public:
  GeoWordNgrams( int newNgramN );
  virtual ~GeoWordNgrams();
  virtual void output();
};

#endif
