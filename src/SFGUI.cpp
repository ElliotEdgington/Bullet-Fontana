#include "SFGUI.h"

int SFGUI::SFGUIID=0;

SFGUI::SFGUI(SFGUITYPE type, std::shared_ptr<SFWindow> window):
                                        type(type), sf_window(window),
                                        sprite_width(0), sprite_height(0)
                                        {
  this->id   = ++SFGUIID;
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
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/player.png");
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
