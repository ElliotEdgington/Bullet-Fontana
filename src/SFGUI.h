#ifndef SFGUI_H
#define SFGUI_H

#include <string>
#include <cstring>
#include <memory>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


using namespace std;

#include "SFCommon.h"
#include "SFEvent.h"
#include "SFWindow.h"
#include "SFBoundingBox.h"
#include "SFMath.h"

/**
*/
enum SFGUITYPE {SFGUI_SCORE, SFGUI_FISH, SFGUI_PIRATE, SFGUI_GIRL,SFGUI_TEXTBOX};

class SFGUI {
public:
  SFGUI(const SFGUITYPE, const std::shared_ptr<SFWindow>);
  SFGUI(const SFGUI&);
  virtual ~SFGUI();

  virtual void      SetPosition(Point2 &);
  virtual Point2    GetPosition();
  virtual SFGUIId   GetId();
  virtual void      SetText(string);
  virtual void      OnRender();

  //virtual void      SetAppear();
  //virtual bool      IsAppear();

private:
  // it would be nice if we could make this into a smart pointer,
  // but, because we need to call SDL_FreeSurface on it, we can't.
  // (or we could use a std::shared_ptr with a custom Deleter, but
  // that's a little too much right now)
  SDL_Texture               * sprite;
  TTF_Font                  * font;
  SDL_Color                   text_color;
  shared_ptr<string>          text;
  shared_ptr<SFBoundingBox>   bbox;
  SFGUITYPE                   type;
  SFGUIId                     id;
  std::shared_ptr<SFWindow>   sf_window;

  int sprite_width;
  int sprite_height;

  static int SFGUIID;
};

#endif
