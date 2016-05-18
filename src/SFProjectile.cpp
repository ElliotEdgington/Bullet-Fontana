#include "SFProjectile.h"

SFProjectile::SFProjectile(BULLETTYPE type ,std::shared_ptr<SFWindow> window) :
                               SFAsset(window), initialDirection(Vector2(0,0)){
  move_speed = 7.0f;
  health = 1;
  this->type = type;

  //A few unused types here for expansion purposes.
  switch (type) {
    case P_PLAYER:
      sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/playerbullet.png");
      break;
    case P_EXPLOSION:
      move_speed = 1.0f;
      sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/projectile_explosion.png");
      break;
    case P_FIREAT:
      sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/projectile_FireAt.png");
      move_speed = 1.0f;
      break;
    case P_SPRAY:
      sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/projectile_spray.png");
      move_speed = 1.0f;
      break;
    case P_ANGLE:
      sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/projectile_FireRandom.png");
      move_speed = 1.0f;
    break;
  }

  LoadSprite();
}

SFProjectile::~SFProjectile(){}

void SFProjectile::UpdateMovement(){
  if(type == P_PLAYER){
    GoNorth();
  }else{
    GoDirection(initialDirection);
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
