#include "SFPlayer.h"

SFPlayer::SFPlayer(std::shared_ptr<SFWindow> window) : SFAsset(window){
  sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/player.png");
  move_speed = 4.0f;
  health = 100;
  LoadSprite();
}
SFPlayer::~SFPlayer(){}


void SFPlayer::AddHealth(int h){
  health += h;
  if(health > 100) health = 100;
}

//overriding movement so I can add limitations.

void SFPlayer::GoNorth(){
  int w, h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);
  Vector2 c = *(bbox->centre) + Vector2(0.0f, move_speed);

  if(!(c.getY() > h - sprite_height/2)){
    bbox->centre.reset();
    bbox->centre = make_shared<Vector2>(c);
  }
}

void SFPlayer::GoSouth(){
  Vector2 c = *(bbox->centre) + Vector2(0.0f, -move_speed);

  if(!(c.getY() < sprite_height/2)){
    bbox->centre.reset();
    bbox->centre = make_shared<Vector2>(c);
  }
}

void SFPlayer::GoEast(){
  int w, h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);

  Vector2 c = *(bbox->centre) + Vector2(move_speed, 0.0f);
  if(!(c.getX() > w - sprite_width/2)) {
    bbox->centre.reset();
    bbox->centre = make_shared<Vector2>(c);
  }
}

void SFPlayer::GoWest(){
  Vector2 c = *(bbox->centre) + Vector2(-move_speed, 0.0f);
  if(!(c.getX() < sprite_width/2)) {
    bbox->centre.reset();
    bbox->centre = make_shared<Vector2>(c);
  }
}
