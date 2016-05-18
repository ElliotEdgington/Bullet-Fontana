#include "SFWall.h"

SFWall::SFWall(std::shared_ptr<SFWindow> window) : SFAsset(window){
  sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/wall.png");
  move_speed = 4.0f;
  health = 4;
  LoadSprite();
}

SFWall::~SFWall(){}
