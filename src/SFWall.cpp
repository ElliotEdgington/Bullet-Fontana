#include "SFWall.h"

SFWall::SFWall(std::shared_ptr<SFWindow> window) : SFAsset(window){
  sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/alien.png");
  move_speed = 4.0f;
  health = 2;
  LoadSprite();
}

SFWall::~SFWall(){}
