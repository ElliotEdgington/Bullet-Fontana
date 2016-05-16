#include "SFWall.h"

SFWall::SFWall(std::shared_ptr<SFWindow> window) : SFAsset(window){
  sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/player.png");
  move_speed = 4.0f;
  health = 3;
  LoadSprite();
}

SFWall::~SFWall(){}
