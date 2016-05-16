#include "SFPower_Up.h"

SFPower_Up::SFPower_Up(POWERTYPE type ,std::shared_ptr<SFWindow> window) : SFAsset(window){
  sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/coin.png");
  move_speed = 3.0f;


  //Loads ___ sprite
  switch(type){
    case POWER_HEALTH:
      break;
    case POWER_BULLETS:
      break;
    case POWER_SHIELD:
      break;
    case POWER_WALL:
      break;
  }
  LoadSprite();
}

SFPower_Up::~SFPower_Up(){}
