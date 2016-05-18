#ifndef SFBOSS_H
#define SFBOSS_H

#include <string>
#include <memory>
#include <iostream>
#include <list>     // Pull in list


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;

#include "SFCommon.h"
#include "SFEvent.h"
#include "SFWindow.h"
#include "SFBoundingBox.h"
#include "SFMath.h"
#include "SFAsset.h"
#include "SFProjectile.h"
#include "SFPattern.h"
#include "SFPlayer.h"


class SFBoss : public SFAsset{
public:
  SFBoss(const shared_ptr<SFWindow>,const shared_ptr<SFPattern>,const shared_ptr<SFPlayer>,int);
  virtual ~SFBoss();
  list <shared_ptr<SFProjectile>> UpdateBoss();
  int GetHealth();
  void SetHealth(int);
  void SetPhase(int);
private:
  list <shared_ptr<SFProjectile>> Phase1();
  list <shared_ptr<SFProjectile>> Phase2();
  list <shared_ptr<SFProjectile>> Phase3();

  void AddRotation(int);

  int w_height;
  shared_ptr<SFPattern> pattern;
  shared_ptr<SFPlayer> player;

  int p_dir;
  int phase;
  int sub_phase;
  int phase_timer;
};


#endif
