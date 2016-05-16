#ifndef SFPROJECTILE_H
#define SFPROJECTILE_H

#include <string>
#include <memory>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;

#include "SFCommon.h"
#include "SFEvent.h"
#include "SFWindow.h"
#include "SFBoundingBox.h"
#include "SFMath.h"
#include "SFAsset.h"

enum BULLETTYPE {P_PLAYER,P_ENEMY_LONG, P_ENEMY_ROUND};

class SFProjectile : public SFAsset{
public:
  SFProjectile(BULLETTYPE,const std::shared_ptr<SFWindow>);
  virtual ~SFProjectile();

  void UpdateMovement();
  void SetDirection(Vector2 &);

private:
  Vector2       initialDirection;
  BULLETTYPE    type;
};


#endif
