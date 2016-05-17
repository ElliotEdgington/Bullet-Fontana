#include "SFBasic_Enemy.h"

SFBasic_Enemy::SFBasic_Enemy(ENEMYTYPE type, std::shared_ptr<SFWindow> window) :
                                        SFAsset(window), startPos(Point2(0,0)),
                                        direction(1), waver(0), type(type), killed(true){
  move_speed = 3.5f;
  health = 3;
  switch (type) {
  case E_STRAIGHT:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/alien.png");
    move_speed = 1.0f;
    health = 6;
    break;
  case E_WAVE:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/alien.png");
    move_speed = 1.0f;
    health = 12;
    waver = 50;
    break;
  case E_DIAGONAL_LEFT:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/alien.png");
    move_speed = 1.5f;
    health = 5;
  case E_DIAGONAL_RIGHT:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/alien.png");
    move_speed = 1.5f;
    health = 5;
    break;
  }

  LoadSprite();
}

SFBasic_Enemy::~SFBasic_Enemy(){}

void SFBasic_Enemy::UpdateMovement(){
  switch (type) {
  case E_STRAIGHT:
    GoSouth();
    break;
  case E_WAVE:
    {
      GoSouth();
      float s_x = startPos.getX();
      float p_x = GetPosition().getX();
      if(s_x + waver < p_x || s_x - waver > p_x) direction*=-1;
      if(direction == 1) GoEast();
      else GoWest();
    }
    break;
  case E_DIAGONAL_LEFT:
    GoSouth();
    GoWest();
    break;
  case E_DIAGONAL_RIGHT:
    GoSouth();
    GoEast();
    break;
  }

  //enemies get deleted once they go off the bottom of the screen.
  //-10 is a buffer to make sure theyre 100% off screen
  if(GetPosition().getY() < -10){
     SetNotAlive();
     killed = false;
   }
}

void SFBasic_Enemy::SetPosition(Point2 & point) {
  Vector2 v(point.getX(), point.getY());
  bbox->SetCentre(v);
  startPos = point;
}

ENEMYTYPE SFBasic_Enemy::GetType(){
  return type;
}

// killed is used to determine whether the player killed the enemy
// or the enemy just despawned off screen
bool SFBasic_Enemy::GetKilled(){
  return killed;
}
