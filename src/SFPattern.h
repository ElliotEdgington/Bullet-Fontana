#ifndef SFPATTERN_H
#define SFPATTERN_H

#include <memory>   // Pull in std::shared_ptr
#include <iostream> // Pull in std::cerr, std::endl
#include <list>     // Pull in list
#include <sstream>
#include <cmath>

using namespace std;

#include "SFCommon.h"
#include "SFProjectile.h"
#include "SFMath.h"

class SFPattern {
public:
  SFPattern(const shared_ptr<SFWindow>);
  virtual ~SFPattern();

  list<shared_ptr<SFProjectile> > CreateExplosion(Point2, int);
  list<shared_ptr<SFProjectile> > FireAt(Point2,Point2);
  list<shared_ptr<SFProjectile> > SprayAt(Point2, Point2);
  list<shared_ptr<SFProjectile> > FireAngle(Point2,int);

private:
  shared_ptr<SFWindow>   sf_window;
};

#endif
