#include "SFProjectile.h"

SFProjectile::SFProjectile(BULLETTYPE type ,std::shared_ptr<SFWindow> window) :
                               SFAsset(window), initialDirection(Vector2(0,0)){
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
  //checks if off screen
  int w, h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);
  float p_x = GetPosition().getX();
  float p_y = GetPosition().getY();
  if(p_x > w || p_y > h || p_x < 0 || p_y < 0){
    SetNotAlive();
  }
}

void SFProjectile::SetDirection(Vector2 & v){
  initialDirection = v;
}
