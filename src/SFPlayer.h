#ifndef SFPLAYER_H
#define SFPLAYER_H

#include <string>
#include <memory>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;

#include "SFCommon.h"
#include "SFEvent.h"
#include "SFWindow.h"
#include "SFBoundingBox.h"
#include "SFMath.h"
#include "SFAsset.h"

class SFPlayer : public SFAsset{
public:
  SFPlayer(const std::shared_ptr<SFWindow>);
  virtual ~SFPlayer();

  void GoNorth();
  void GoSouth();
  void GoEast();
  void GoWest();

};


#endif
