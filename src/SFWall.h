#ifndef SFWall_H
#define SFWall_H

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

class SFWall : public SFAsset{
public:
  SFWall(const std::shared_ptr<SFWindow>);
  virtual ~SFWall();
};

#endif
