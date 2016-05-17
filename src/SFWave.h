#ifndef SFWAVE_H
#define SFWAVE_H

#include <memory>   // Pull in std::shared_ptr
#include <iostream> // Pull in std::cerr, std::endl
#include <list>     // Pull in list
#include <sstream>
#include <cmath>
#include <string>

using namespace std;

#include "SFCommon.h"
#include "SFBasic_Enemy.h"

class SFWave {
public:
  SFWave(const shared_ptr<SFWindow>,int,int);
  virtual ~SFWave();
  list<shared_ptr<SFBasic_Enemy> > GetWave(int);

private:
  list<shared_ptr<SFBasic_Enemy> > AddColumnOf(int,int,int,ENEMYTYPE,int);
  list<shared_ptr<SFBasic_Enemy> > AddRowOf(int,int,int,ENEMYTYPE,int);
  list<shared_ptr<SFBasic_Enemy> > AddDiagonalOf(int,int,int,ENEMYTYPE,int,int);
  list<shared_ptr<SFBasic_Enemy> > GetWave1();
  list<shared_ptr<SFBasic_Enemy> > GetWave2();
  list<shared_ptr<SFBasic_Enemy> > GetWave3();
  list<shared_ptr<SFBasic_Enemy> > GetWave4();

  shared_ptr<SFWindow>   sf_window;
  int                    w_width;
  int                    w_height;
};

#endif
