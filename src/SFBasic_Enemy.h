#ifndef SFBASIC_ENEMY_H
#define SFBASIC_ENEMY_H

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

enum ENEMYTYPE {E_STRAIGHT, E_WAVE ,E_DIAGONAL_RIGHT,E_DIAGONAL_LEFT};

class SFBasic_Enemy : public SFAsset{
public:
  SFBasic_Enemy(ENEMYTYPE ,const std::shared_ptr<SFWindow>);
  virtual ~SFBasic_Enemy();
  void UpdateMovement();
  void SetPosition(Point2 &);
  ENEMYTYPE GetType();
  bool GetKilled();

private:
  ENEMYTYPE   type;
  Point2      startPos;
  int         direction;
  int         waver;
  bool        killed;
};


#endif
