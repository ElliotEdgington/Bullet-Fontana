#include "SFGUI.h"

int SFGUI::SFGUIID=0;

//this class was originally going to be for images on screen to tell story
//but run out of time so im repurposing it to text/rect class
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


  color = {0,0,0};
  switch (type) {
  case SFGUI_WIN:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/alien.png");
    break;
  case SFGUI_LOSE:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/alien.png");
    break;
  case SFGUI_BAR:
    SDL_Surface *rect;
    rect = SDL_CreateRGBSurface(0,400,10,32,0,0,0,0);
    SDL_FillRect(rect, NULL, SDL_MapRGB(rect->format, 235, 0, 0));
    sprite = SDL_CreateTextureFromSurface(sf_window->getRenderer(),rect);
    SDL_FreeSurface(rect);
    break;
  case SFGUI_TEXTBOX:
    SDL_Surface *textbox;
    textbox = TTF_RenderText_Solid(font, "text not set", color);
    sprite = SDL_CreateTextureFromSurface(sf_window->getRenderer(),textbox);
    SDL_FreeSurface(textbox);
    break;
  }

  if(!sprite) {
    cerr << "Could not load asset of type " << type << endl;
    throw SF_ERROR_LOAD_ASSET;
  }

  // Get texture width & height
  SDL_QueryTexture(sprite, NULL, NULL, &sprite_width, &sprite_height);
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

int SFGUI::GetWidth(){
  return sprite_width;
}

void SFGUI::SetWidth(int w){
  SDL_Surface *rect;
  rect = SDL_CreateRGBSurface(0,w,10,32,0,0,0,0);
  SDL_FillRect(rect, NULL, SDL_MapRGB(rect->format, 235, 0, 0));
  sprite = SDL_CreateTextureFromSurface(sf_window->getRenderer(),rect);
  SDL_FreeSurface(rect);

  SDL_QueryTexture(sprite, NULL, NULL, &sprite_width, &sprite_height);
  bbox = make_shared<SFBoundingBox>(SFBoundingBox(Vector2(GetPosition().getX(),GetPosition().getY()), sprite_width, sprite_height));
}



void SFGUI::SetText(string s){
  //couldnt get around this another way? SDL doesnt like supporting dynamic text
  SDL_Surface *textbox;
  const char * str = s.c_str();
  textbox = TTF_RenderText_Solid(font, str, color);
  sprite = SDL_CreateTextureFromSurface(sf_window->getRenderer(),textbox);

  // resets bbox so it displays correctly
  SDL_QueryTexture(sprite, NULL, NULL, &sprite_width, &sprite_height);
  bbox = make_shared<SFBoundingBox>(SFBoundingBox(Vector2(GetPosition().getX(),GetPosition().getY()), sprite_width, sprite_height));
  SDL_FreeSurface(textbox);
}

void SFGUI::OnRender() {
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
}
