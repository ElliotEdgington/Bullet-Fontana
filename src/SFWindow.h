#ifndef SFWINDOW_H
#define SFWINDOW_H

#include <SDL2/SDL.h>

class SFWindow {
 public:
  SFWindow(SDL_Window*, SDL_Renderer*, SDL_Surface*);
  SDL_Window* getWindow();
  SDL_Renderer* getRenderer();
  SDL_Surface* getSurface();
 private:
  SDL_Window*   window;
  SDL_Renderer* renderer;
  SDL_Surface* surface;
};

#endif
