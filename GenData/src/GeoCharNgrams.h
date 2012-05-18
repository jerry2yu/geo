#ifndef GEOCHARNGRAMS_H_
#define GEOCHARNGRAMS_H_

#include "CharNgrams.h"
#include "GeoToken.h"

class GeoCharNgrams : public CharNgrams
{
 public:
  GeoCharNgrams( int newNgramN );
  virtual ~GeoCharNgrams();
  virtual void output();
};

#endif
