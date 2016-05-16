#include "SFGUI.h"

int SFGUI::SFGUIID=0;

SFGUI::SFGUI(SFGUITYPE type, std::shared_ptr<SFWindow> window):
                                        type(type), sf_window(window),
                                        sprite_width(0), sprite_height(0)
                                        {
  this->id   = ++SFGUIID;

  font = TTF_OpenFont("assets/Monospace.ttf", 24);
  if(!font){
    cerr << "Could not load font "<< endl;
    throw SF_ERROR_LOAD_ASSET;
  }


  text_color = {0,0,0};
  switch (type) {
  case SFGUI_FISH:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/player.png");
    break;
  case SFGUI_GIRL:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/player.png");
    break;
  case SFGUI_SCORE:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/alien.png");
    break;
  case SFGUI_PIRATE:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/player.png");
    break;
  case SFGUI_TEXTBOX:
    text = make_shared<string>("hello world");
    break;
  }

  if(!sprite && !text) {
    cerr << "Could not load asset of type " << type << endl;
    throw SF_ERROR_LOAD_ASSET;
  }

  // Get texture width & height
  if(!SFGUI_TEXTBOX){
    SDL_QueryTexture(sprite, NULL, NULL, &sprite_width, &sprite_height);
  }else{
      sprite_width = 300; sprite_height = 25;
  }

  bbox = make_shared<SFBoundingBox>(SFBoundingBox(Vector2(0.0f, 0.0f), sprite_width, sprite_height));

}

SFGUI::SFGUI(const SFGUI& a) {
  sprite = a.sprite;
  sf_window = a.sf_window;
  bbox   = a.bbox;
  type   = a.type;
}

SFGUI::~SFGUI() {
  bbox.reset();
  if(sprite) {
    SDL_DestroyTexture(sprite);
    sprite = nullptr;
  }
  if(font){
    TTF_CloseFont(font);
  }
}

void SFGUI::SetPosition(Point2 & point) {
    Vector2 v(point.getX(), point.getY());
    bbox->SetCentre(v);
}

Point2 SFGUI::GetPosition() {
    return Point2(bbox->centre->getX(), bbox->centre->getY());
}

SFGUIId SFGUI::GetId() {
    return id;
}

void SFGUI::SetText(string s){
  text.reset();
  text = make_shared<string>(s);
}

void SFGUI::OnRender() {
  //couldnt get around this another way? SDL doesnt like supporting dynamic text
  SDL_Surface *textbox;
  const char * s = text->c_str();
  if(type == SFGUI_TEXTBOX){
    textbox = TTF_RenderText_Solid(font, s, text_color);
    sprite = SDL_CreateTextureFromSurface(sf_window->getRenderer(),textbox);
  }

  // 1. Get the SDL_Rect from SFBoundingBox
  SDL_Rect rect;

  Vector2 gs = (*(bbox->centre) + (*(bbox->extent_x) * -1)) + (*(bbox->extent_y) * 1);
  Vector2 ss = GameSpaceToScreenSpace(sf_window->getRenderer(), gs);
  rect.x = ss.getX();
  rect.y = ss.getY();
  rect.w = bbox->extent_x->getX() * 2;
  rect.h = bbox->extent_y->getY() * 2;

  // 2. Blit the sprite onto the level
  SDL_RenderCopy(sf_window->getRenderer(), sprite, NULL, &rect);
  SDL_FreeSurface(textbox);
  //delete s;
}
