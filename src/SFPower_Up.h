#ifndef SFPower_Up_H
#define SFPower_Up_H

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


enum POWERTYPE {POWER_HEALTH,POWER_WALL,POWER_BULLETS};

class SFPower_Up : public SFAsset{
public:
  SFPower_Up(POWERTYPE type,const std::shared_ptr<SFWindow>);
  virtual ~SFPower_Up();
  POWERTYPE GetType();

private:
  POWERTYPE type;
};


#endif
