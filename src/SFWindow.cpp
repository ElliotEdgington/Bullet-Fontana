#include "SFWindow.h"

SFWindow::SFWindow(SDL_Window * w, SDL_Renderer * r, SDL_Surface * s):
                                  window(w), renderer(r), surface(s) {
}

SDL_Window * SFWindow::getWindow() {
  return window;
}

SDL_Renderer * SFWindow::getRenderer() {
  return renderer;
}

SDL_Surface * SFWindow::getSurface() {
  return surface;
}
