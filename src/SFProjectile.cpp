#include "SFProjectile.h"

SFProjectile::SFProjectile(BULLETTYPE type ,std::shared_ptr<SFWindow> window) :
                               SFAsset(window), initialDirection(Vector2(0,0)){
  sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/projectile.png");
  move_speed = 7.0f;
  health = 1;
  this->type = type;
  switch (type) {
  case P_PLAYER:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/projectile.png");
    break;
  case P_ENEMY_LONG:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/player.png");
    break;
  case P_ENEMY_ROUND:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/alien.png");
    move_speed = 1.0f;
    break;
  }

  LoadSprite();
}

SFProjectile::~SFProjectile(){}

void SFProjectile::UpdateMovement(){
  switch (type) {
  case P_PLAYER:
    GoNorth();
    break;
  case P_ENEMY_LONG:
    GoSouth();
    break;
  case P_ENEMY_ROUND:
    GoDirection(initialDirection);
    break;
    }
}

void SFProjectile::SetDirection(Vector2 & v){
  initialDirection = v;
}
