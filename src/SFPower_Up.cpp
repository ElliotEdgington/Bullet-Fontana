#include "SFPower_Up.h"

SFPower_Up::SFPower_Up(POWERTYPE type ,std::shared_ptr<SFWindow> window) : SFAsset(window), type(type){
  sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/coin.png");
  move_speed = 0.5f;
  health = 1;


  //Loads ___ sprite
  switch(type){
    case POWER_HEALTH:
      break;
    case POWER_BULLETS:
      break;
    case POWER_WALL:
      break;
  }
  LoadSprite();
}

SFPower_Up::~SFPower_Up(){}

POWERTYPE SFPower_Up::GetType(){
  return type;
}
