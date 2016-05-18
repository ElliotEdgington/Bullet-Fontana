#include "SFPower_Up.h"

SFPower_Up::SFPower_Up(POWERTYPE type ,std::shared_ptr<SFWindow> window) : SFAsset(window), type(type){
  move_speed = 0.5f;
  health = 1;


  //Loads ___ sprite
  switch(type){
    case POWER_HEALTH:
      sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/power_health.png");
      break;
    case POWER_BULLETS:
      sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/power_bullet.png");
      break;
    case POWER_WALL:
      sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/power_wall.png");
      break;
  }
  LoadSprite();
}

SFPower_Up::~SFPower_Up(){}

POWERTYPE SFPower_Up::GetType(){
  return type;
}
